#include "RZRendererPCH.h"

namespace rczEngine
{
	HRESULT rczEngine::VXGIImplement::InitVXGI()
	{
		g_pRendererInterface = new NVRHI::RendererInterfaceD3D11(&g_ErrorCallback, Gfx::GfxCore::Pointer()->GetDeviceContext());

		pass.CreatePipeline(eRenderingPipelines::Forward);

		if (FAILED(CreateVXGIObject()))
			return E_FAIL;

		Gfx::GfxCore::Pointer()->ClearDepthTargetView();

		g_bInitialized = true;

		m_TargetDepth = g_pRendererInterface->getHandleForTexture(Gfx::GfxCore::Pointer()->GetDefaultDepthStencyl()->GetTexture()->m_Texture);
		m_TargetNormal = g_pRendererInterface->getHandleForTexture(RacrozRenderer::Pointer()->m_Normals->GetTextureCore()->m_Texture);


		int w, h;
		Gfx::GfxCore::Pointer()->GetScreenSize(w, h);
		rTarget = RacrozRenderer::Pointer()->CreateRenderTargetAndTexture_WidthHeight("Main", texture, 1, w, h);
		m_Width = w;
		m_Height = h;

		auto racrozRenderer = RacrozRenderer::Pointer();
		m_gfx = Gfx::GfxCore::Pointer();

		SpecularResult = racrozRenderer->CreateRenderTargetAndTexture_WidthHeight("Specular Result", SpecularResultTex, 1, m_Width, m_Height, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		DiffuseResult = racrozRenderer->CreateRenderTargetAndTexture_WidthHeight("Diffuse Result", DiffuseResultTex, 1, m_Width, m_Height, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		SSAOResult = racrozRenderer->CreateRenderTargetAndTexture_WidthHeight("SSAO Result", SSAOResultTex, 1, m_Width, m_Height, Gfx::eFORMAT::FORMAT_R8_UNORM);

		ResVault::Pointer()->InsertResource(SpecularResultTex);
		ResVault::Pointer()->InsertResource(DiffuseResultTex);
		ResVault::Pointer()->InsertResource(SSAOResultTex);

		OldDepth = std::make_shared<Gfx::DepthStencyl>();

		OldNormal = racrozRenderer->CreateRenderTargetAndTexture_WidthHeight("Prev Normal", OldNormalTex, 1, m_Width, m_Height, Gfx::FORMAT_R32G32B32A32_FLOAT);
		m_gfx->CreateDepthStencyl(*OldDepth, m_Width, m_Height);

		DebugResult = RacrozRenderer::Pointer()->CreateRenderTargetAndTexture_WidthHeight("-------------------------", DebugResultTex, 1, m_Width, m_Height, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		ResVault::Pointer()->InsertResource(DebugResultTex);

		m_Albedo = g_pRendererInterface->getHandleForTexture(DebugResultTex->GetTextureCore()->m_Texture);


		return S_OK;		
	}

	void VXGIImplement::DestroyVXGI()
	{
		ReleaseResources(g_pGI);

		if (g_pGI)
		{
			if (g_pGITracer) g_pGI->destroyTracer(g_pGITracer);
			g_pGITracer = NULL;

			VFX_VXGI_DestroyGIObject(g_pGI);
			g_pGI = NULL;
		}
	}

	void VXGIImplement::VoxelizeScene()
	{
		SetVoxelizationParameters();

		auto activeCamera = CameraManager::Pointer()->GetActiveCamera();
		auto centerPoint = CalculateVoxelCenterPoint(activeCamera);

		auto view = activeCamera.lock()->GetViewMatrix();
		auto proj = activeCamera.lock()->GetProjMatrix();

		VXGI::UpdateVoxelizationParameters params;
		params.clipmapAnchor = VXGI::float3(centerPoint.m_elements);
		params.finestVoxelSize = g_fVoxelSize;
		params.indirectIrradianceMapTracingParameters.irradianceScale = g_fMultiBounceScale;
		params.indirectIrradianceMapTracingParameters.useAutoNormalization = true;
		params.indirectIrradianceMapTracingParameters.lightLeakingAmount = VXGI::LightLeakingAmount::MODERATE;

		bool performOpacityVoxelization = false;
		bool performEmittanceVoxelization = false;

		g_pGI->prepareForVoxelization(
			params,
			performOpacityVoxelization,
			performEmittanceVoxelization);

		if (performOpacityVoxelization || performEmittanceVoxelization)
		{
			VXGI::VoxelizationViewParameters viewParams;
			g_pGI->getVoxelizationViewParameters(viewParams);
			VXGI::float4x4 voxelizationMatrix = viewParams.viewMatrix * viewParams.projectionMatrix;
		
			memcpy(&pass.VoxelizeViewMatrix, &viewParams.viewMatrix, sizeof(Matrix4));
			memcpy(&pass.VoxelizeProjMatrix, &viewParams.projectionMatrix, sizeof(Matrix4));

			const uint32_t maxRegions = 128;
			uint32_t numRegions = 0;
			VXGI::Box3f regions[maxRegions];
		
			if (VXGI_SUCCEEDED(g_pGI->getInvalidatedRegions(regions, maxRegions, numRegions)))
			{
				DrawSceneForVoxelization();
			}
		}

		g_pGI->finalizeVoxelization();
	}

	void VXGIImplement::RenderChannels()
	{
		if (g_RenderingMode == RenderingMode::OPACITY_VOXELS ||
			g_RenderingMode == RenderingMode::EMITTANCE_VOXELS ||
			g_RenderingMode == RenderingMode::IRRADIANCE_VOXELS)
		{
			RenderDebugChannels();
		}
		else
		{
			ComputeIndirectChannels();
		}
	}

	void rczEngine::VXGIImplement::DrawSceneForVoxelization()
	{
		g_pRendererInterface->beginRenderingPass();

		g_pGI->beginVoxelizationDrawCallGroup();

		VXGIMaterialAdapter materialInfo;
		materialInfo.geometryShader = m_pVoxelizationGS;
		materialInfo.pixelShader = m_pVoxelizationPS;

		NVRHI::DrawCallState state;

		if (VXGI_FAILED(g_pGI->getVoxelizationState(materialInfo, true, state)))
			__debugbreak();

		state.renderState.rasterState.frontCounterClockwise = false;
		state.renderState.clearDepthTarget = false;
		state.renderState.clearColorTarget = false;

		g_pRendererInterface->clearState();
		g_pRendererInterface->applyState(state);
		
		pass.PreRenderPass();
		pass.RenderPass();
		
		///Iterate through all meshes, for each mesh:
			///Maybe builds drawcalls?
			///Check bounds to see if it is contained.
			///Voxelize by calling getVoxelizationState
			///Check if repeated material.
			///Bind shadow map to PS
			///Bind samplers to PS
			///Bind global constant buffer to PS
			///Update the global constant buffer
			///Bind diffuse texture to PS
			///Add draw call for this mesh.
			///Draw all the real drawcalls.
		
		g_pRendererInterface->clearState();

		g_pGI->endVoxelizationDrawCallGroup();

		g_pRendererInterface->endRenderingPass();


		m_gfx->SetDefaultRenderTarget();
		m_gfx->SetViewPortDefault();
		m_gfx->SetBlendStateDefault();
		//m_gfx->SetDefaulDepthStencylState();
	}

	Vector3 rczEngine::VXGIImplement::CalculateVoxelCenterPoint(WeakPtr<CameraCmp> activeCamera)
	{
		const Vector3 centerPoint = activeCamera.lock()->GetPosition() + activeCamera.lock()->GetViewDir() * g_fVoxelSize * float(g_nMapSize) * 0.25f;
		return centerPoint;
	}

	void rczEngine::VXGIImplement::ComputeIndirectChannels()
	{
		m_gfx->UnbindPSShaderResource(8);
		m_gfx->UnbindPSShaderResource(9);

		auto activeCamera = CameraManager::Pointer()->GetActiveCamera();

		VXGI::IBasicViewTracer::InputBuffers inputBuffers;
		inputBuffers.gbufferViewport = NVRHI::Viewport(float(m_Width), float(m_Height));
		inputBuffers.gbufferDepth = m_TargetDepth;
		inputBuffers.gbufferNormal = m_TargetNormal;

		int32 w, h;
		Gfx::GfxCore::Pointer()->GetScreenSize(w, h);

		inputBuffers.gbufferViewport = NVRHI::Viewport(float(w), float(h));

		memcpy(&inputBuffers.viewMatrix, &activeCamera.lock()->GetViewMatrix(), sizeof(Matrix4));
		memcpy(&inputBuffers.projMatrix, &activeCamera.lock()->GetProjMatrix(), sizeof(Matrix4));

		NVRHI::TextureHandle indirectDiffuse = NULL;
		NVRHI::TextureHandle indirectSpecular = NULL;
		NVRHI::TextureHandle indirectConfidence = NULL;
		NVRHI::TextureHandle ssao = NULL;

		g_pGITracer->beginFrame();

		VXGI::BasicDiffuseTracingParameters diffuseParams;
		VXGI::BasicSpecularTracingParameters specularParams;
		diffuseParams.enableTemporalReprojection = g_bTemporalFiltering;
		diffuseParams.enableTemporalJitter = g_bTemporalFiltering;
		diffuseParams.quality = g_fQuality;
		diffuseParams.directionalSamplingRate = g_fSamplingRate;
		diffuseParams.irradianceScale = g_fDiffuseScale;

		///AO
		diffuseParams.ambientRange = g_fVxaoRange;
		diffuseParams.interpolationWeightThreshold = 0.1f;
		diffuseParams.ambientScale = g_fVxaoScale;
		
		specularParams.irradianceScale = g_fSpecularScale;
		specularParams.filter = g_bTemporalFiltering ? VXGI::SpecularTracingParameters::FILTER_TEMPORAL : VXGI::SpecularTracingParameters::FILTER_SIMPLE;
		specularParams.enableTemporalJitter = g_bTemporalFiltering;
		specularParams.enableConeJitter = true;

		if (g_bEnableGI)
		{
			g_pGITracer->computeDiffuseChannelBasic(diffuseParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectDiffuse, indirectConfidence);
			g_pGITracer->computeSpecularChannelBasic(specularParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectSpecular);
		}

		///SSAO
		VXGI::SsaoParamaters ssaoParams;
		ssaoParams.radiusWorld = g_ssaoRadius;

		g_pGITracer->computeSsaoChannelBasic(ssaoParams, inputBuffers, ssao);

		g_InputBuffersPrev = inputBuffers;
		g_InputBuffersPrevValid = true;

		auto resource = g_pRendererInterface->getResourceForTexture(indirectSpecular);
		Gfx::GfxCore::Pointer()->GetDeviceContext()->CopyResource(SpecularResultTex->m_TextureCore.m_Texture, resource);
		
		auto resource2 = g_pRendererInterface->getResourceForTexture(indirectDiffuse);
		Gfx::GfxCore::Pointer()->GetDeviceContext()->CopyResource(DiffuseResultTex->m_TextureCore.m_Texture, resource2);

		auto resourceSSAO = g_pRendererInterface->getResourceForTexture(ssao);
		Gfx::GfxCore::Pointer()->GetDeviceContext()->CopyResource(SSAOResultTex->m_TextureCore.m_Texture, resourceSSAO);

		auto resourceDepth = g_pRendererInterface->getResourceForTexture(m_TargetDepth);
		Gfx::GfxCore::Pointer()->GetDeviceContext()->CopyResource(OldDepth->GetTexture()->m_Texture, resourceDepth);

		auto resourceNormal = g_pRendererInterface->getResourceForTexture(m_TargetNormal);
		Gfx::GfxCore::Pointer()->GetDeviceContext()->CopyResource(OldNormal->GetTextureCore()->m_Texture, resourceNormal);

		g_InputBuffersPrev.gbufferNormal = g_pRendererInterface->getHandleForTexture(OldNormal->GetTextureCore()->m_Texture);
		g_InputBuffersPrev.gbufferDepth = g_pRendererInterface->getHandleForTexture(OldDepth->GetTexture()->m_Texture);

		g_pRendererInterface->clearState();

		m_gfx->SetBlendStateDefault();
		m_gfx->SetDefaulDepthStencylState();

	};

	void rczEngine::VXGIImplement::RenderDebugChannels()
	{
		auto activeCamera = CameraManager::Pointer()->GetActiveCamera();

		VXGI::IBasicViewTracer::InputBuffers inputBuffers;
		inputBuffers.gbufferDepth = m_TargetDepth;
		inputBuffers.gbufferNormal = m_TargetNormal;

		inputBuffers.preViewTranslation = VXGI::float3(activeCamera.lock()->GetPosition().m_elements);

		int32 w, h;
		Gfx::GfxCore::Pointer()->GetScreenSize(w, h);

		inputBuffers.gbufferViewport = NVRHI::Viewport(float(w), float(h));
		auto view = activeCamera.lock()->GetViewMatrix();
		auto proj = activeCamera.lock()->GetProjMatrix();

		memcpy(&inputBuffers.viewMatrix, &view, sizeof(Matrix4));
		memcpy(&inputBuffers.projMatrix, &proj, sizeof(Matrix4));

		VXGI::DebugRenderParameters params;
		if (g_RenderingMode == RenderingMode::OPACITY_VOXELS)
			params.debugMode = VXGI::DebugRenderMode::OPACITY_TEXTURE;
		else if (g_RenderingMode == RenderingMode::EMITTANCE_VOXELS)
			params.debugMode = VXGI::DebugRenderMode::EMITTANCE_TEXTURE;
		else
			params.debugMode = VXGI::DebugRenderMode::INDIRECT_IRRADIANCE_TEXTURE;
		
		params.viewMatrix = *(VXGI::float4x4*)&view;
		params.projMatrix = *(VXGI::float4x4*)&proj;
		params.viewport = inputBuffers.gbufferViewport;
		params.destinationTexture = m_Albedo;
		params.level = -1;
		params.blendState.blendEnable[0] = false;
		params.blendState.srcBlend[0] = NVRHI::BlendState::BLEND_SRC_ALPHA;
		params.blendState.destBlend[0] = NVRHI::BlendState::BLEND_INV_SRC_ALPHA;
		g_pGI->renderDebug(params);				
		bool convertToLdr = g_RenderingMode != RenderingMode::OPACITY_VOXELS;

		Gfx::GfxCore::Pointer()->SetBlendStateDefault();
	}

	HRESULT rczEngine::VXGIImplement::CreateVXGIObject()
	{
		///Create Global Illumination
		///Create Shader Compiler
		///Set Voxelization Parameters
		///Created Basic Tracer

		VXGI::GIParameters params;
		params.rendererInterface = g_pRendererInterface;
		params.errorCallback = &g_ErrorCallback;

		VXGI::ShaderCompilerParameters comparams;
		comparams.errorCallback = &g_ErrorCallback;
		comparams.graphicsAPI = g_pRendererInterface->getGraphicsAPI();
		comparams.d3dCompilerDLLName = "d3dcompiler_47.dll";

		if (VXGI_FAILED(VFX_VXGI_CreateGIObject(params, &g_pGI)))
		{
			Logger::Pointer()->Log("Failed to create VXGI Object.");
			return E_FAIL;
		}

		if (VXGI_FAILED(VFX_VXGI_CreateShaderCompiler(comparams, &g_pGICompiler)))
		{
			Logger::Pointer()->Log("Failed to create shader compiler.");
			return E_FAIL;

		}

		if (VXGI_FAILED(CreateVoxelizationGS(g_pGICompiler, g_pGI)))
			return E_FAIL;

		if (VXGI_FAILED(CreateVoxelizationPS(g_pGICompiler, g_pGI)))
			return E_FAIL;

		//if (VXGI_FAILED(CreateTransparentGeometryPS(g_pGICompiler, g_pGI)))
		//	return E_FAIL;

		if (VXGI_FAILED(SetVoxelizationParameters()))
		{
			Logger::Pointer()->Log("Failed to init VXGI voxelization.");
			return E_FAIL;
		}

		if (VXGI_FAILED(g_pGI->createBasicTracer(&g_pGITracer, g_pGICompiler)))
		{
			Logger::Pointer()->Log("Failed to create VXGI Tracer.");
			return E_FAIL;
		}

		///El shadow map es d24s8

		return S_OK;
	}

	HRESULT rczEngine::VXGIImplement::CreateVoxelizationGS(VXGI::IShaderCompiler * pCompiler, VXGI::IGlobalIllumination * pGI)
	{
		NVRHI::ShaderRef m_pDefaultVS;

		auto blobVertex = RacrozRenderer::Pointer()->m_DefaultVertexShader.m_ShaderBlob;

		m_pDefaultVS = g_pRendererInterface->createShader(NVRHI::ShaderDesc(NVRHI::ShaderType::SHADER_VERTEX), blobVertex->GetBufferPointer(), blobVertex->GetBufferSize());

		VXGI::IBlob* blob = nullptr;
		
		if (VXGI_FAILED(pCompiler->compileVoxelizationGeometryShaderFromVS(&blob, blobVertex->GetBufferPointer(), blobVertex->GetBufferSize())))
			return E_FAIL;

		if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pVoxelizationGS, blob->getData(), blob->getSize())))
			return E_FAIL;

		blob->dispose();

		return S_OK;
	}

	HRESULT rczEngine::VXGIImplement::CreateVoxelizationPS(VXGI::IShaderCompiler * pCompiler, VXGI::IGlobalIllumination * pGI)
	{
		FileStream f;
		f.open("./Shaders/PBR_Voxelize.hlsl");

		std::stringstream buffer;
		buffer << f.rdbuf();

		f.close();

		String data = buffer.str();
		uint32_t size = data.length();

		// Enumerate resource slots (constant buffers, textures, samplers, potentially UAVs)
		// that are used by the user part of the voxelization pixel shader
		VXGI::ShaderResources resources;
		resources.unorderedAccessViewCount = 1;
		resources.unorderedAccessViewSlots[0] = 0;

		resources.constantBufferCount = 7;
		resources.constantBufferSlots[0] = 0; ///Sometimes
		resources.constantBufferSlots[1] = 1; ///OnResize
		resources.constantBufferSlots[2] = 2; ///EveryFrame
		resources.constantBufferSlots[3] = 3; ///Lights
		resources.constantBufferSlots[4] = 4; ///Material
		resources.constantBufferSlots[5] = 5; ///Camera
		resources.constantBufferSlots[6] = 6; ///Renderer Config

		resources.textureCount = 8;
		resources.textureSlots[0] = 0; //Albedo
		resources.textureSlots[1] = 1; //Normal
		resources.textureSlots[2] = 2; //Metallic
		resources.textureSlots[3] = 3; //Roughness
		resources.textureSlots[4] = 4; //AO
		resources.textureSlots[5] = 12; //Environment
		resources.textureSlots[6] = 6; //Opacity
		resources.textureSlots[7] = 7; //LUT

		resources.samplerCount = 4;
		resources.samplerSlots[0] = 0; //Linear Wrap
		resources.samplerSlots[1] = 1; //Aniso Wrap
		resources.samplerSlots[2] = 2; //Linear Clamp
		resources.samplerSlots[3] = 3; //Aniso Clamp

		VXGI::IBlob* blob = nullptr;

		//if (VXGI_FAILED(pCompiler->compileVoxelizationDefaultPixelShader(&blob)))
		//	return E_FAIL;

		if (VXGI_FAILED(pCompiler->compileVoxelizationPixelShader(&blob, (char*)data.c_str(), size, "PS_Main", resources)))
			return E_FAIL;

		if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pVoxelizationPS, blob->getData(), blob->getSize())))
			return E_FAIL;

		blob->dispose();

		return S_OK;
	}

	HRESULT rczEngine::VXGIImplement::CreateTransparentGeometryPS(VXGI::IShaderCompiler * pCompiler, VXGI::IGlobalIllumination * pGI)
	{
		FileStream f;
		f.open("./Shaders/PBR_Voxelize.hlsl");

		std::stringstream buffer;
		buffer << f.rdbuf();

		f.close();

		String data = buffer.str();
		uint32_t size = data.length();

		// Enumerate resource slots (constant buffers, textures, samplers, potentially UAVs)
		// that are used by the user part of the voxelization pixel shader
		VXGI::ShaderResources resources;
		resources.unorderedAccessViewCount = 1;
		resources.unorderedAccessViewSlots[0] = 0;

		resources.constantBufferCount = 7;
		resources.constantBufferSlots[0] = 0; ///Sometimes
		resources.constantBufferSlots[1] = 1; ///OnResize
		resources.constantBufferSlots[2] = 2; ///EveryFrame
		resources.constantBufferSlots[3] = 3; ///Lights
		resources.constantBufferSlots[4] = 4; ///Material
		resources.constantBufferSlots[5] = 5; ///Camera
		resources.constantBufferSlots[6] = 6; ///Renderer Config

		resources.textureCount = 8;
		resources.textureSlots[0] = 0; //Albedo
		resources.textureSlots[1] = 1; //Normal
		resources.textureSlots[2] = 2; //Metallic
		resources.textureSlots[3] = 3; //Roughness
		resources.textureSlots[4] = 4; //AO
		resources.textureSlots[5] = 12; //Environment
		resources.textureSlots[6] = 6; //Opacity
		resources.textureSlots[7] = 7; //LUT

		resources.samplerCount = 4;
		resources.samplerSlots[0] = 0; //Linear Wrap
		resources.samplerSlots[1] = 1; //Aniso Wrap
		resources.samplerSlots[2] = 2; //Linear Clamp
		resources.samplerSlots[3] = 3; //Aniso Clamp

		VXGI::IBlob* blob = nullptr;

		if (VXGI_FAILED(pCompiler->compileConeTracingPixelShader(&blob, (char*)data.c_str(), size, "PS_Main", resources)))
			return E_FAIL;

		if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pTransparentGeometryPS, blob->getData(), blob->getSize())))
			return E_FAIL;

		blob->dispose();

		return S_OK;
	}

	void rczEngine::VXGIImplement::AllocateViewDependentResources(UINT width, UINT height, UINT sampleCount)
	{
		m_Width = width;
		m_Height = height;
		m_SampleCount = sampleCount;

		NVRHI::TextureDesc gbufferDesc;
		gbufferDesc.width = m_Width;
		gbufferDesc.height = m_Height;
		gbufferDesc.isRenderTarget = true;
		gbufferDesc.useClearValue = true;
		gbufferDesc.sampleCount = m_SampleCount;
		gbufferDesc.disableGPUsSync = true;

		gbufferDesc.format = NVRHI::Format::RGBA8_UNORM;
		gbufferDesc.format = NVRHI::Format::RGBA16_FLOAT;
		gbufferDesc.format = NVRHI::Format::D24S8;
	}

	void rczEngine::VXGIImplement::ReleaseResources(VXGI::IGlobalIllumination * pGI)
	{
		if (m_pVoxelizationGS)
		{
			pGI->destroyUserDefinedShaderSet(m_pVoxelizationGS);
			m_pVoxelizationGS = NULL;
		}

		if (m_pVoxelizationPS)
		{
			pGI->destroyUserDefinedShaderSet(m_pVoxelizationPS);
			m_pVoxelizationPS = NULL;
		}

		if (m_pTransparentGeometryPS)
		{
			pGI->destroyUserDefinedShaderSet(m_pTransparentGeometryPS);
			m_pTransparentGeometryPS = NULL;
		}
	}

	VXGI::Status::Enum rczEngine::VXGIImplement::SetVoxelizationParameters()
	{
		static VXGI::VoxelizationParameters previousParams;

		VXGI::VoxelizationParameters voxelizationParams;
		voxelizationParams.mapSize = VXGI::uint3(g_nMapSize);
		voxelizationParams.enableMultiBounce = g_bEnableMultiBounce;
		voxelizationParams.persistentVoxelData = !g_bEnableMultiBounce;
		voxelizationParams.useEmittanceInterpolation = g_bEnableMultiBounce;

		if (previousParams != voxelizationParams)
		{
			VXGI::Status::Enum Status = g_pGI->validateVoxelizationParameters(voxelizationParams);
			if (VXGI_SUCCEEDED(Status))
			{
				Status = g_pGI->setVoxelizationParameters(voxelizationParams);
			}

			previousParams = voxelizationParams;

			return Status;
		}

		return VXGI::Status::OK;
	}
}
#pragma once
#include <GFSDK_NVRHI_D3D11.h>
#include <GFSDK_VXGI.h>
#include <GFSDK_VXGI_MathTypes.h>
#include <GFSDK_NVRHI.h>
#include <nvapi.h>

#define API_STRING "D3D11"
	NVRHI::RendererInterfaceD3D11* g_pRendererInterface = NULL;
#define FLUSH_COMMAND_LIST 

#define CREATE_SHADER(type, code, ret) m_RendererInterface->createShader(NVRHI::ShaderDesc(NVRHI::ShaderType::SHADER_##type), &code, sizeof(code), ret)

namespace rczEngine
{
	enum class RenderingMode
	{
		NORMAL,
		DIFFUSE_CHANNEL,
		SPECULAR_CHANNEL,
		OPACITY_VOXELS,
		EMITTANCE_VOXELS,
		IRRADIANCE_VOXELS
	};

	class RendererErrorCallback : public NVRHI::IErrorCallback
	{
		void signalError(const char* file, int line, const char* errorDesc)
		{
			char buffer[4096];
			int length = (int)strlen(errorDesc);
			length = std::min(length, 4000); // avoid a "buffer too small" exception for really long error messages
			sprintf_s(buffer, "%s:%i\n%.*s", file, line, length, errorDesc);

			OutputDebugStringA(buffer);
			OutputDebugStringA("\n");
			MessageBoxA(NULL, buffer, "ERROR", MB_ICONERROR | MB_OK);
		}
	};

	RendererErrorCallback g_ErrorCallback;

	__declspec(align(16)) struct GlobalConstants
	{
		VXGI::float4x4 worldMatrix;
		VXGI::float4x4 viewProjMatrix;
		VXGI::float4x4 viewProjMatrixInv;
		VXGI::float4x4 lightMatrix;
		VXGI::float4 cameraPos;
		VXGI::float4 lightDirection;
		VXGI::float4 diffuseColor;
		VXGI::float4 lightColor;
		VXGI::float4 ambientColor;
		float rShadowMapSize;
		uint32_t enableIndirectDiffuse;
		uint32_t enableIndirectSpecular;
		float transparentRoughness;
		float transparentReflectance;
	};

	struct MeshMaterialInfo : public VXGI::MaterialInfo
	{
		NVRHI::TextureHandle    diffuseTexture;
		NVRHI::TextureHandle    specularTexture;
		NVRHI::TextureHandle    normalsTexture;
		VXGI::float3          diffuseColor;

		MeshMaterialInfo() :
			diffuseTexture(NULL),
			specularTexture(NULL),
			normalsTexture(NULL),
			diffuseColor(0.f)
		{}
	};

	typedef std::function<void(const MeshMaterialInfo&)> MaterialCallback;

	class RZ_EXP VXGIImplement
	{
	public:

		virtual HRESULT InitVXGI()
		{
			g_pRendererInterface = new NVRHI::RendererInterfaceD3D11(&g_ErrorCallback, Gfx::GfxCore::Pointer()->GetDeviceContext());

			if (FAILED(CreateVXGIObject()))
				return E_FAIL;

			g_bInitialized = true;

			return S_OK;
		}

		virtual void DestroyVXGI()
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

		bool IsNvDeviceID(UINT id)
		{
			return id == 0x10DE;
		}

		//IDXGIAdapter*
		//	DeviceManager::GetDXGIAdapter()
		//{
		//	if (!m_Device)
		//		return NULL;
		//
		//	IDXGIDevice* pDXGIDevice = NULL;
		//	m_Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDXGIDevice));
		//
		//	IDXGIAdapter* pDXGIAdapter = NULL;
		//	pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&pDXGIAdapter));
		//
		//	pDXGIDevice->Release();
		//
		//	return pDXGIAdapter;
		//}

		virtual void Render()
		{
			// Render the shadow map before calling g_pGI->updateGlobalIllumination
			// because that function will voxelize the scene using the shadow map.
			// The coordinates passed to RenderShadowMap are at the center of the top opening of Sponza. 
			// That's where the light's projection center will be.
			//Render The Shadow Map

			SetVoxelizationParameters();

			if (g_bEnableGI || g_RenderingMode != RenderingMode::NORMAL)
			{
				XMVECTOR centerPt = eyePt + viewForward * g_fVoxelSize * float(g_nMapSize) * 0.25f;

				static VXGI::Frustum lightFrusta[2];
				lightFrusta[0] = g_pSceneRenderer->GetLightFrustum();

				VXGI::UpdateVoxelizationParameters params;
				params.clipmapAnchor = VXGI::float3(centerPt.m128_f32);
				params.finestVoxelSize = g_fVoxelSize;
				params.indirectIrradianceMapTracingParameters.irradianceScale = g_fMultiBounceScale;
				params.indirectIrradianceMapTracingParameters.useAutoNormalization = true;
				params.indirectIrradianceMapTracingParameters.lightLeakingAmount = VXGI::LightLeakingAmount::MODERATE;

				if (memcmp(&lightFrusta[0], &lightFrusta[1], sizeof(VXGI::Frustum)) != 0)
				{
					params.invalidatedFrustumCount = 2;
					params.invalidatedLightFrusta = lightFrusta;
					lightFrusta[1] = lightFrusta[0];
				}

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

					const uint32_t maxRegions = 128;
					uint32_t numRegions = 0;
					VXGI::Box3f regions[maxRegions];

					if (VXGI_SUCCEEDED(g_pGI->getInvalidatedRegions(regions, maxRegions, numRegions)))
					{
						NVRHI::DrawCallState emptyState;
						g_pSceneRenderer->RenderForVoxelization(emptyState, g_pGI, regions, numRegions, voxelizationMatrix, NULL);
					}
				}

				g_pGI->finalizeVoxelization();
			}

			VXGI::float3 cameraPos(eyePt.m128_f32);
			g_pSceneRenderer->RenderToGBuffer(viewProjMatrix, cameraPos, g_bDrawTransparent);

			VXGI::IBasicViewTracer::InputBuffers inputBuffers;
			g_pSceneRenderer->FillTracingInputBuffers(inputBuffers);
			memcpy(&inputBuffers.viewMatrix, &viewMatrix, sizeof(viewMatrix));
			memcpy(&inputBuffers.projMatrix, &projMatrix, sizeof(projMatrix));

			if (g_RenderingMode == RenderingMode::OPACITY_VOXELS ||
				g_RenderingMode == RenderingMode::EMITTANCE_VOXELS ||
				g_RenderingMode == RenderingMode::IRRADIANCE_VOXELS)
			{
				// Voxel texture visualization is rendered over the albedo channel, no GI

				NVRHI::TextureHandle gbufferAlbedo = g_pSceneRenderer->GetAlbedoBufferHandle();

				VXGI::DebugRenderParameters params;
				if (g_RenderingMode == RenderingMode::OPACITY_VOXELS)
					params.debugMode = VXGI::DebugRenderMode::OPACITY_TEXTURE;
				else if (g_RenderingMode == RenderingMode::EMITTANCE_VOXELS)
					params.debugMode = VXGI::DebugRenderMode::EMITTANCE_TEXTURE;
				else
					params.debugMode = VXGI::DebugRenderMode::INDIRECT_IRRADIANCE_TEXTURE;
				params.viewMatrix = *(VXGI::float4x4*)&viewMatrix;
				params.projMatrix = *(VXGI::float4x4*)&projMatrix;
				params.viewport = inputBuffers.gbufferViewport;
				params.destinationTexture = gbufferAlbedo;
				params.destinationDepth = inputBuffers.gbufferDepth;
				params.level = -1;
				params.blendState.blendEnable[0] = true;
				params.blendState.srcBlend[0] = NVRHI::BlendState::BLEND_SRC_ALPHA;
				params.blendState.destBlend[0] = NVRHI::BlendState::BLEND_INV_SRC_ALPHA;

				g_pGI->renderDebug(params);

				bool convertToLdr = g_RenderingMode != RenderingMode::OPACITY_VOXELS;
				g_pSceneRenderer->Blit(gbufferAlbedo, mainRenderTarget, convertToLdr);
			}
			else
			{
				NVRHI::TextureHandle indirectDiffuse = NULL;
				NVRHI::TextureHandle indirectSpecular = NULL;
				NVRHI::TextureHandle indirectConfidence = NULL;
				VXGI::float3 ambientColor(g_fAmbientScale);

				g_pGITracer->beginFrame();

				VXGI::BasicDiffuseTracingParameters diffuseParams;
				VXGI::BasicSpecularTracingParameters specularParams;
				diffuseParams.enableTemporalReprojection = g_bTemporalFiltering;
				diffuseParams.enableTemporalJitter = g_bTemporalFiltering;
				diffuseParams.quality = g_fQuality;
				diffuseParams.directionalSamplingRate = g_fSamplingRate;
				diffuseParams.irradianceScale = g_fDiffuseScale;
				specularParams.irradianceScale = g_fSpecularScale;
				specularParams.filter = g_bTemporalFiltering ? VXGI::SpecularTracingParameters::FILTER_TEMPORAL : VXGI::SpecularTracingParameters::FILTER_SIMPLE;
				specularParams.enableTemporalJitter = g_bTemporalFiltering;
				specularParams.enableConeJitter = true;


				switch (g_RenderingMode)
				{
				case RenderingMode::NORMAL:
					if (g_bEnableGI)
					{
						if (g_fDiffuseScale > 0)
							g_pGITracer->computeDiffuseChannelBasic(diffuseParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectDiffuse, indirectConfidence);

						if (g_fSpecularScale > 0)
							g_pGITracer->computeSpecularChannelBasic(specularParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectSpecular);
					}

					g_pSceneRenderer->Shade(indirectDiffuse, indirectSpecular, indirectConfidence, mainRenderTarget, viewProjMatrix, ambientColor * 0.5f);

					if (g_bDrawTransparent)
					{
						g_pSceneRenderer->RenderTransparentScene(g_pGI, mainRenderTarget, viewProjMatrix, cameraPos, g_TransparentRoughness, g_TransparentReflectance);
					}
					break;

				case RenderingMode::DIFFUSE_CHANNEL:
					diffuseParams.irradianceScale = 1.0f;
					g_pGITracer->computeDiffuseChannelBasic(diffuseParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectDiffuse, indirectConfidence);
					g_pSceneRenderer->Blit(indirectDiffuse, mainRenderTarget, true);
					break;

				case RenderingMode::SPECULAR_CHANNEL:
					specularParams.irradianceScale = 1.0f;
					g_pGITracer->computeSpecularChannelBasic(specularParams, inputBuffers, g_InputBuffersPrevValid ? &g_InputBuffersPrev : nullptr, indirectSpecular);
					g_pSceneRenderer->Blit(indirectSpecular, mainRenderTarget, true);
					break;
				}

			}

			g_InputBuffersPrev = inputBuffers;
			g_InputBuffersPrevValid = true;
		}

		void RenderToGBuffer()
		{
			///Save the previous depth and normal
			std::swap(m_TargetNormal, m_TargetNormalPrev);
			std::swap(m_TargetDepth, m_TargetDepthPrev);

			NVRHI::DrawCallState state;

			///Change the material

			state.PS.shader = m_pAttributesPS;
			NVRHI::BindSampler(state.PS, 0, m_pDefaultSamplerState);

			state.renderState.targetCount = 2;
			state.renderState.targets[0] = m_TargetAlbedo;
			state.renderState.targets[1] = m_TargetNormal;
			state.renderState.clearColorTarget = true;
			state.renderState.depthTarget = m_TargetDepth;
			state.renderState.clearDepthTarget = true;
			state.renderState.viewportCount = 1;
			state.renderState.viewports[0] = NVRHI::Viewport(float(m_Width), float(m_Height));

			GlobalConstants constants = {};
			constants.viewProjMatrix = viewProjMatrix;
			constants.cameraPos = cameraPos;

			RenderSceneCommon(m_pScene, state, NULL, NULL, 0, constants, &onChangeMaterial, false);
		}

		HRESULT CreateVXGIObject()
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

			if (VXGI_FAILED(CreateTransparentGeometryPS(g_pGICompiler, g_pGI)))
				return E_FAIL;

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

		HRESULT CreateVoxelizationGS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI)
		{
			FileStream f;
			f.open("./Shaders/VXGI/Shaders.hlsl");
			
			std::stringstream buffer;
			buffer << f.rdbuf();

			f.close();

			String str = buffer.str();

			VXGI::IBlob* blob = nullptr;

			if (VXGI_FAILED(pCompiler->compileVoxelizationGeometryShaderFromVS(&blob, str.c_str(), str.size())))
				return E_FAIL;

			if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pVoxelizationGS, blob->getData(), blob->getSize())))
				return E_FAIL;

			blob->dispose();

			return S_OK;
		}

		HRESULT CreateVoxelizationPS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI)
		{
			FileStream f;
			f.open("./Shaders/VXGI/VoxelizationPS.hlsl");

			std::stringstream buffer;
			buffer << f.rdbuf();

			f.close();

			String data = buffer.str();
			uint32_t size = sizeof(data.size());

			// Enumerate resource slots (constant buffers, textures, samplers, potentially UAVs)
			// that are used by the user part of the voxelization pixel shader
			VXGI::ShaderResources resources;
			resources.constantBufferCount = 1;
			resources.constantBufferSlots[0] = 0;
			resources.textureCount = 2;
			resources.textureSlots[0] = 0;
			resources.textureSlots[1] = 1;
			resources.samplerCount = 2;
			resources.samplerSlots[0] = 0;
			resources.samplerSlots[1] = 1;

			VXGI::IBlob* blob = nullptr;

			if (VXGI_FAILED(pCompiler->compileVoxelizationPixelShader(&blob, (char*)data.c_str(), size, "main", resources)))
				return E_FAIL;

			if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pVoxelizationPS, blob->getData(), blob->getSize())))
				return E_FAIL;

			blob->dispose();

			return S_OK;
		}

		HRESULT CreateTransparentGeometryPS(VXGI::IShaderCompiler* pCompiler, VXGI::IGlobalIllumination* pGI)
		{
			FileStream f;
			f.open("./Shaders/VXGI/TransparentGeometryPS.hlsl");

			std::stringstream buffer;
			buffer << f.rdbuf();

			f.close();

			String data = buffer.str();
			uint32_t size = sizeof(data.size());

			// Enumerate resource slots (constant buffers, textures, samplers, potentially UAVs)
			// that are used by the user part of the voxelization pixel shader
			VXGI::ShaderResources resources;
			resources.constantBufferCount = 1;
			resources.constantBufferSlots[0] = 0;

			VXGI::IBlob* blob = nullptr;

			if (VXGI_FAILED(pCompiler->compileConeTracingPixelShader(&blob, (char*)data.c_str(), size, "main", resources)))
				return E_FAIL;

			if (VXGI_FAILED(pGI->loadUserDefinedShaderSet(&m_pTransparentGeometryPS, blob->getData(), blob->getSize())))
				return E_FAIL;

			blob->dispose();

			return S_OK;
		}

		void AllocateViewDependentResources(UINT width, UINT height, UINT sampleCount)
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
			gbufferDesc.clearValue = NVRHI::Color(0.f);
			gbufferDesc.debugName = "GbufferAlbedo";
			m_RendererInterface->createTexture(gbufferDesc, NULL, &m_TargetAlbedo);

			gbufferDesc.format = NVRHI::Format::RGBA16_FLOAT;
			gbufferDesc.clearValue = NVRHI::Color(0.f);
			gbufferDesc.debugName = "GbufferNormals";
			m_RendererInterface->createTexture(gbufferDesc, NULL, &m_TargetNormal);
			m_RendererInterface->createTexture(gbufferDesc, NULL, &m_TargetNormalPrev);

			gbufferDesc.format = NVRHI::Format::D24S8;
			gbufferDesc.clearValue = NVRHI::Color(1.f, 0.f, 0.f, 0.f);
			gbufferDesc.debugName = "GbufferDepth";
			m_RendererInterface->createTexture(gbufferDesc, NULL, &m_TargetDepth);
			m_RendererInterface->createTexture(gbufferDesc, NULL, &m_TargetDepthPrev);
		}

		void ReleaseResources(VXGI::IGlobalIllumination* pGI)
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

		VXGI::Status::Enum SetVoxelizationParameters()
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

	private:
		RenderingMode g_RenderingMode = RenderingMode::NORMAL;

		VXGI::IUserDefinedShaderSet* m_pVoxelizationGS;
		VXGI::IUserDefinedShaderSet* m_pVoxelizationPS;
		VXGI::IUserDefinedShaderSet* m_pTransparentGeometryPS;

		///Global objects
		VXGI::IGlobalIllumination* g_pGI = NULL;
		VXGI::IShaderCompiler* g_pGICompiler = NULL;
		VXGI::IBasicViewTracer* g_pGITracer = NULL;

		///Input buffers
		VXGI::IBasicViewTracer::InputBuffers g_InputBuffersPrev;
		bool g_InputBuffersPrevValid = false;

		float g_fCameraClipNear = 1.0f;
		float g_fCameraClipFar = 10000.0f;
		float g_fVoxelSize = 8.0f;
		float g_fAmbientScale = 0.0f;
		float g_fDiffuseScale = 1.0f;
		float g_fSpecularScale = 1.0f;
		bool g_bEnableMultiBounce = true;
		float g_fMultiBounceScale = 1.0f;
		float g_fLightSize = 2500.0f;
		bool g_bEnableGI = true;
		bool g_bRenderHUD = true;
		bool g_bInitialized = false;
		float g_fSamplingRate = 1.0f;
		float g_fQuality = 0.1f;
		int g_nMapSize = 128;
		bool g_bDrawTransparent = false;
		float g_TransparentRoughness = 0.1f;
		float g_TransparentReflectance = 0.1f;
		bool g_bTemporalFiltering = true;


		///////// Scene Renderer

		NVRHI::IRendererInterface* m_RendererInterface;

		NVRHI::InputLayoutRef    m_pInputLayout;
		NVRHI::ShaderRef         m_pDefaultVS;
		NVRHI::ShaderRef         m_pFullScreenQuadVS;

		NVRHI::ShaderRef         m_pAttributesPS;
		NVRHI::ShaderRef         m_pBlitPS;
		NVRHI::ShaderRef         m_pBlitLdrPS;
		NVRHI::ShaderRef         m_pCompositingPS;

		NVRHI::ConstantBufferRef m_pGlobalCBuffer;

		NVRHI::SamplerRef        m_pDefaultSamplerState;
		NVRHI::SamplerRef        m_pComparisonSamplerState;

		UINT                    m_Width;
		UINT                    m_Height;
		UINT                    m_SampleCount;

		VXGI::float3            m_LightDirection;
		VXGI::float4x4          m_LightViewMatrix;
		VXGI::float4x4          m_LightProjMatrix;
		VXGI::float4x4          m_LightViewProjMatrix;

		NVRHI::TextureRef       m_TargetAlbedo;
		NVRHI::TextureRef       m_TargetNormal;
		NVRHI::TextureRef       m_TargetDepth;
		NVRHI::TextureRef       m_TargetNormalPrev;
		NVRHI::TextureRef       m_TargetDepthPrev;

		NVRHI::TextureRef       m_ShadowMap;

		NVRHI::TextureRef       m_NullTexture;
	};
};
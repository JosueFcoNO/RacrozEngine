#include <RZRendererPCH.h>

namespace rczEngine
{
	RacrozRenderer*& RacrozRenderer::_Instance()
	{
		static RacrozRenderer* instance = nullptr;
		return instance;
	}

	void RacrozRenderer::Start()
	{
		(_Instance()) = new RacrozRenderer;
	}

	RacrozRenderer* RacrozRenderer::Pointer()
	{
		return _Instance();
	}

	void RacrozRenderer::ShutDown()
	{
		delete _Instance();
	}

	void RacrozRenderer::InitRenderer()
	{
		m_gfx = Gfx::GfxCore::Pointer();
		m_res = ResVault::Pointer();

		///Save the screen size.
		m_gfx->GetScreenSize(m_Width, m_Height);

		///Set the primitive Topology to TriangleList
		m_gfx->SetPrimitiveTopology(Gfx::eTOPOLOGY::TOPO_TRIANGLELIST);

		///Create the Rasterizers.
		InitRasterizers();

		///Create the sampler States
		InitSamplerStates();

		///Init Shaders
		InitGeometryShaders();

		///Init the screen quad
		m_ScreenQuad.InitScreenQuad(m_gfx);

		m_ActiveSkyBox = std::make_shared<SkyBox>();

		m_CameraBuffer.CreateConstantBuffer(sizeof(CameraData), Gfx::USAGE_DEFAULT, m_gfx);

		m_BlurPass.CreatePipeline(eRenderingPipelines::Deferred);
	}

	void RacrozRenderer::CreatePipeline(const String& name, eRenderingPipelines renderingMode)
	{
		switch (renderingMode)
		{
		case eRenderingPipelines::Forward:
			m_Pipelines[name] = std::make_shared<PipelineForward>();
			m_Pipelines[name]->InitRenderPipeline("Forward", m_Width, m_Height, this);
			break;
		case eRenderingPipelines::Deferred:
			m_Pipelines[name] = std::make_shared<PipelineDeferred>();
			m_Pipelines[name]->InitRenderPipeline("Deferred", m_Width, m_Height, this);
			break;
		case eRenderingPipelines::ForwardPlus:
			//SetForwardPlus();
			break;
		case eRenderingPipelines::Debug:
			m_Pipelines[name] = std::make_shared<PipelineDebug>();
			m_Pipelines[name]->InitRenderPipeline("Debug", m_Width, m_Height, this);
			break;
		default:
			break;
		}

		if (renderingMode == eRenderingPipelines::Deferred)
		{
			m_VXGI.InitVXGI();

			auto pass = m_Pipelines[name]->GetPass(2);
			pass->AddTexture2D(m_VXGI.SSAOResultTex, 5);
			pass->AddTexture2D(m_VXGI.DiffuseResultTex, 8);
			pass->AddTexture2D(m_VXGI.SpecularResultTex, 9);
		}
	}

	void RacrozRenderer::PrepareGI(bool prepareScene)
	{
		m_GIPrepared = true;

		if (prepareScene)
			PrepareDrawableObjects(SceneManager::Pointer()->GetActiveScene());
		
		m_VXGI.VoxelizeScene();
	};

	void RacrozRenderer::Render(const String& name, StrPtr<Scene> sceneGraph)
	{
		PrepareDrawableObjects(sceneGraph);

		if (!m_GIPrepared)
		{
			PrepareGI();
		}

		SetSamplerStates();
		m_Pipelines[name]->DoRender();
	}

	WeakPtr<RenderPipeline> RacrozRenderer::GetPipeline(const String& name)
	{
		return m_Pipelines[name];
	}

	void RacrozRenderer::Destroy()
	{
		m_CameraBuffer.Destroy();
	}

	ResourceHandle RacrozRenderer::CreateCubeMap(const String& name, Scene * sceneGraph, RenderPipeline* renderPipeline, int width, int height)
	{
		///PipelineDeferred defCubeMap;
		///defCubeMap.InitRenderPipeline("cubeMap", width, height, this);
		///
		///m_gfx->SetViewPort(width, height);
		///
		///const Vector3 Targets[6] =
		///{
		///	{ 0, 0, -1},
		///	{ 0, 0, 1 },
		///	{ 1, 0, 0 },
		///	{ -1, 0, 0 },
		///	{ 0, 1, 0 },
		///	{ 0, -1, 0 }
		///};
		///
		///Vector3 Ups[6];
		///
		///Ups[0].Set(0, 1, 0);
		///Ups[1].Set(0, 1, 0);
		///Ups[2].Set(0, 1, 0);
		///Ups[3].Set(0, 1, 0);
		///Ups[4].Set(0, 0, 1);
		///Ups[5].Set(0, 0, -1);
		///
		///StrPtr<Gfx::RenderTarget> renderTargets[6];
		///StrPtr<Texture2D> Textures[6];
		///
		///auto format = Gfx::FORMAT_R16G16B16A16_FLOAT;
		///
		///renderTargets[0] = CreateRenderTexture("0", Textures[0], 1, width, height, format);
		///renderTargets[1] = CreateRenderTexture("1", Textures[1], 1, width, height, format);
		///renderTargets[2] = CreateRenderTexture("2", Textures[2], 1, width, height, format);
		///renderTargets[3] = CreateRenderTexture("3", Textures[3], 1, width, height, format);
		///renderTargets[4] = CreateRenderTexture("4", Textures[4], 1, width, height, format);
		///renderTargets[5] = CreateRenderTexture("5", Textures[5], 1, width, height, format);
		///
		///auto scene = sceneGraph;
		///auto ptr = CameraManager::Pointer();
		///auto cameraCmp = scene->FindActorsWithComponent(CMP_CAMERA_WALK)[0].lock()->GetComponent(CMP_CAMERA_WALK).lock();
		/////ptr->SetActiveCamera(1, m_gfx);
		///auto camera = &CastStaticPtr<CameraCmp>(cameraCmp)->m_CameraCore;
		///camera->SetFov(90.0f);
		///camera->SetAspectRatio(1.0f);
		///camera->SetPosition({ 0, 0, 0 });
		///camera->SetFarClip(100.0f);
		///camera->SetNearClip(0.01f);
		///
		///auto passes = defCubeMap.GetPassesOrder();
		///
		///for (int k = 0; k < 6; ++k)
		///{
		///	camera->Reset({ 0,0,0 }, Targets[k], Ups[k]);
		///
		///	for (int32 i = 0; i < passes.size(); ++i)
		///	{
		///		if (passes[i] == "PostProcess")
		///		{
		///			StartPostProcessing();
		///			continue;
		///		}
		///
		///		auto pass = m_Passes[passes[i]];
		///
		///		pass->PreRenderPass();
		///
		///		if (passes[i] == "cubeMapColorCorrection")
		///		{
		///			m_gfx->AddRenderTarget(*renderTargets[k], 0);
		///			m_gfx->SetNumberOfRenderTargets(1);
		///			m_gfx->SetRenderTargets(false, &defCubeMap.depth);
		///		}
		///
		///		pass->RenderPass();
		///		pass->PostRenderPass();
		///	}
		///}
		///
		///StrPtr<CubeMap> cubemap = std::make_shared<CubeMap>();
		///cubemap->SetFilePath(String("CubemapGenerated/") + name);
		///cubemap->SetName(name);
		///
		///m_gfx->CreateCubeMapFrom6Tex2DCore(
		///	Textures[0]->m_TextureCore,
		///	Textures[1]->m_TextureCore,
		///	Textures[2]->m_TextureCore,
		///	Textures[3]->m_TextureCore,
		///	Textures[4]->m_TextureCore,
		///	Textures[5]->m_TextureCore,
		///	cubemap->m_TextureCore,
		///	format);
		///
		///ResourceHandle handle = m_res->InsertResource(cubemap);
		///
		///StrPtr<SkyBox> skyBox = std::make_shared<SkyBox>();
		///skyBox->InitSkyBox(handle, m_gfx, m_res);
		///
		///CameraManager::Pointer()->SetActiveCamera(1, m_gfx);
		///
		///camera->SetFov(45.0f);
		///camera->SetAspectRatio(1.0f);
		///camera->SetFarClip(100.0f);
		///camera->SetNearClip(0.01f);
		///camera->Reset({ 0,0,5 }, { 0,0,0 }, { 0,1,0 });
		///
		///ChangeSkyBox(skyBox);
		///
		///m_gfx->SetViewPortDefault();

		return 0;
	}

	void RacrozRenderer::PrepareDrawableObjects(StrPtr<Scene> sceneGraph)
	{
		///m_gfx->SetRSStateDefault();
		///m_gfx->SetBlendStateDefault();
	
		m_ObjectsToDraw.clear();

		auto CurrentCamera = CameraManager::Pointer()->GetActiveCamera().lock();
		auto pos = CurrentCamera->GetPosition();
		auto dir = CurrentCamera->GetViewDir();

		for (auto gameObjectPair : sceneGraph->m_SceneActorMap)
		{
			auto gameObj = gameObjectPair.second;
			Set<int> hashesEntered;

			for (auto cmp : gameObj->GetRenderComponents())
			{
				auto currentCmp = cmp.lock();

				switch (currentCmp->GetComponentType())
				{
				case eComponentID::CMP_MODEL_RENDERER:
				{
					auto modelRenderer = CastDynamicPtr<ModelRenderer>(currentCmp);

					for (auto matHandle : modelRenderer->m_Materials)
					{
						auto material = ResVault::Pointer()->GetResource<Material>(matHandle.second).lock();

						MaterialRenderInfo renderInfo;
						renderInfo.InitFromMaterial(*material);
						renderInfo.componentID = eComponentID::CMP_MODEL_RENDERER;

						auto hashRender = MaterialRenderInfo::CalculateRenderHash(renderInfo);

						if (hashesEntered.find(hashRender) == hashesEntered.end())
						{
							hashesEntered.insert(hashRender);
						}
						else
						{
							continue;
						}

						auto VectorToObj = (gameObj->GetAccumulatedPosition() - pos);
						float Magnitude = VectorToObj.Magnitude();

						if (m_ObjectsToDraw.find(hashRender) == m_ObjectsToDraw.end())
						{
							m_ObjectsToDraw[hashRender] = MMap<float, WeakGameObjPtr>();
						}
						
						m_ObjectsToDraw[hashRender].insert(Pair<float, WeakGameObjPtr>(Magnitude, gameObj));
					}
					break;
				}
				case eComponentID::CMP_SKINNED_MODEL_RENDERER:
				{
					//auto skinModelRenderer = CastDynamicPtr<SkinnedModelRenderer>(currentCmp);
					//
					//for (auto matHandle : skinModelRenderer->m_Materials)
					//{
					//	auto material = ResVault::Pointer()->GetResource<Material>(matHandle.second).lock();
					//
					//	MaterialRenderInfo renderInfo;
					//	renderInfo.InitFromMaterial(*material);
					//	renderInfo.componentID = eComponentID::CMP_MODEL_RENDERER;
					//
					//	auto hash = CalculateRenderHash(renderInfo);
					//
					//	m_ObjectsToDraw[hash].insert(gameObj);
					//}
					break;
				}
					///TODO: Terrain and planet
				default:
					break;
				}
			};
		}

		//for (auto it = ObjectMap.begin(); it != ObjectMap.end(); ++it)
		//{
		//	(*it).second.lock()->PreRender(sceneGraph);
		//	(*it).second.lock()->Render(sceneGraph, componentID, matType);
		//}
	}

	void RacrozRenderer::DrawObjectsByProperties(bool forward, eComponentID componentID, eMaterialType matType, eShadingType shading, eBlendType blendType, bool Tesselated = false, bool TwoSided = false, bool blendedMaterial = false, bool wireframe = false)
	{
		auto renderHash = MaterialRenderInfo::CalculateRenderHash(componentID, matType, shading, blendType, Tesselated, TwoSided, blendedMaterial, wireframe);
		auto scene = SceneManager::Pointer()->GetActiveScene();

		if (componentID == eComponentID::CMP_MODEL_RENDERER)
		{
			if (!Tesselated)
			{
				m_DefaultVertexShader.SetThisVertexShaderAndInputLayout(m_gfx);
			}
			else if (Tesselated)
			{
				m_TessVertexShader.SetThisVertexShaderAndInputLayout(m_gfx);
				m_TessHullShader.SetThisHullShader(m_gfx);
				m_TessDomainShader.SetThisDomainShader(m_gfx);
			}
		} 
		else if (componentID == eComponentID::CMP_SKINNED_MODEL_RENDERER)
		{
			if (!Tesselated)
			{
				m_SkinnedVertexShader.SetThisVertexShaderAndInputLayout(m_gfx);
			}
			else if (Tesselated)
			{
				///TODO: Tesselated skinned vertex.
				///m_TessVertexShader.SetThisVertexShaderAndInputLayout(m_gfx);
				///m_TessHullShader.SetThisHullShader(m_gfx);
				///m_TessDomainShader.SetThisDomainShader(m_gfx);
			}
		}

		if (forward)
		{
			for (auto it = m_ObjectsToDraw[renderHash].begin(); it != m_ObjectsToDraw[renderHash].end(); ++it)
			{
				it->second.lock()->PreRender();
				it->second.lock()->Render(componentID, renderHash);
			}
		}
		else
		{
			for (auto it = m_ObjectsToDraw[renderHash].rbegin(); it != m_ObjectsToDraw[renderHash].rend(); ++it)
			{
				it->second.lock()->PreRender();
				it->second.lock()->Render(componentID, renderHash);
			}
		}

		if (Tesselated) ///Clear the hull and domain to deactivate tesselation.
		{
			m_gfx->RemoveDomainShader();
			m_gfx->RemoveHullShader();
		}
	}

	void RacrozRenderer::DrawScreenQuad()
	{
		m_ScreenQuad.SetScreenQuadBuffers(m_gfx);
		m_ScreenQuadVS.SetThisVertexShaderAndInputLayout(m_gfx);
		m_gfx->DrawIndexed(6, 0, 0);
	}

	void RacrozRenderer::DoBlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height)
	{
		m_BlurPass.BlurPass(outRenderTarget, inTexture, width, height);
	}

	StrPtr<Gfx::RenderTarget> RacrozRenderer::CreateRenderTexture(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format)
	{
		///Allocate a new RenderTarget Object
		auto renderTarget = std::make_shared<Gfx::RenderTarget>();

		///Create the RenderTarget.
		m_gfx->CreateRenderTarget(*renderTarget, name, false, mipMaps, width, height, format);

		///Just in case the texture is already allocated. delete the texture. The pointers that referenced that texture will now reference this new one.
		if (out_Texture)
		{
			delete out_Texture.get();
		}

		out_Texture = std::make_shared<Texture2D>();
		out_Texture->CreateFromRenderTarget(*renderTarget);

		return renderTarget;
	}

	void RacrozRenderer::InitRasterizers()
	{
		///Create the Rasterizers
		m_RSWireframe.Init(Gfx::FILL_WIREFRAME, Gfx::CULL_NONE);
		m_RSWireframe.CreateRasterizerState(m_gfx);

		m_RSSolid.Init(Gfx::FILL_SOLID, Gfx::CULL_NONE);
		m_RSSolid.CreateRasterizerState(m_gfx);

		m_RSSolidCullNone.Init(Gfx::FILL_SOLID, Gfx::CULL_NONE);
		m_RSSolidCullNone.CreateRasterizerState(m_gfx);
	}

	void RacrozRenderer::InitSamplerStates()
	{
		///Create a LinearWrapSampler and set it on slot 0
		m_LinealWrapSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER);
		m_LinealWrapSampler.CreateSamplerState(m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 1
		m_AnisotropicWrapSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_ANISOTROPIC, Vector4(1, 1, 1, 1), 16);
		m_AnisotropicWrapSampler.CreateSamplerState(m_gfx);

		///Create a LinearWrapSampler and set it on slot 2
		m_LinealClampSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::COMPARISON_NEVER);
		m_LinealClampSampler.CreateSamplerState(m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 3
		m_AnisotropicClampSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_ANISOTROPIC, Vector4(1, 1, 1, 1), 16);
		m_AnisotropicClampSampler.CreateSamplerState(m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 3
		m_PointWrapSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_MIN_MAG_MIP_POINT, Vector4(1, 1, 1, 1), 16);
		m_PointWrapSampler.CreateSamplerState(m_gfx);
	}

	void RacrozRenderer::SetSamplerStates()
	{
		m_LinealWrapSampler.PSSetThisSamplerState(0, 1, m_gfx);
		m_AnisotropicWrapSampler.PSSetThisSamplerState(1, 1, m_gfx);
		m_LinealClampSampler.PSSetThisSamplerState(2, 1, m_gfx);
		m_AnisotropicClampSampler.PSSetThisSamplerState(3, 1, m_gfx);
		m_PointWrapSampler.PSSetThisSamplerState(4, 1, m_gfx);
	}

	void RacrozRenderer::InitGeometryShaders()
	{
		///Geometry Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_DefaultVertexShader, L"Shaders/GPassMetRough.hlsl");
		m_DefaultVertexShader.ReflectLayout(0, m_gfx);

		m_gfx->CompileAndCreateVertexShader(m_SimpleTransformVertexShader, L"Shaders/GPassLightDepth.hlsl");
		m_SimpleTransformVertexShader.ReflectLayout(0, m_gfx);


		///Geometry Skinned Vertex Shader
		m_gfx->CompileAndCreateVertexShader(m_SkinnedVertexShader, L"Shaders/SkinnedVertexShader.hlsl");
		m_SkinnedVertexShader.ReflectLayout(0, m_gfx);

		///Geometry Tesselated Shader
		m_gfx->CompileAndCreateDomainShader(m_TessDomainShader, L"Shaders/Tess/TessDS.hlsl");
		m_gfx->CompileAndCreateHullShader(m_TessHullShader, L"Shaders/Tess/TessHS.hlsl");
		m_gfx->CompileAndCreateVertexShader(m_TessVertexShader, L"Shaders/Tess/TessVS.hlsl");
		m_TessVertexShader.ReflectLayout(0, m_gfx);

		///Compile, create and reflect the ScreenQuad Vertex Shader.
		m_gfx->CompileAndCreateVertexShader(m_ScreenQuadVS, L"Shaders/PassVShader.hlsl");
		m_ScreenQuadVS.ReflectLayout(1, m_gfx);
	};

	////////////////////////////
	/// Camera | Viewport
	////////////////////////////

	void RacrozRenderer::ResetViewport()
	{
		m_gfx->SetViewPortDefault();
	}

	void RacrozRenderer::SetViewport(int width, int height)
	{
		m_gfx->SetViewPort(width, height);
	}

	void RacrozRenderer::UpdateCameraBuffersActiveCamera()
	{
		const auto camera = CameraManager::Pointer()->GetActiveCamera();

		if (!camera.expired())
		{
			const auto cameraLock = camera.lock();

			m_TempCameraData.FarPlane.m_x = cameraLock->GetFarPlane();
			m_TempCameraData.NearPlane.m_x = cameraLock->GetNearPlane();

			m_TempCameraData.PreviousProjectionMatrix = m_TempCameraData.ProjectionMatrix;
			m_TempCameraData.ProjectionMatrix = cameraLock->GetProjMatrix().GetTransposed();

			m_TempCameraData.PreviousViewMatrix = m_TempCameraData.ViewMatrix;
			m_TempCameraData.ViewMatrix = cameraLock->GetViewMatrix().GetTransposed();

			m_TempCameraData.ViewPosition = cameraLock->GetPosition();
			m_TempCameraData.ViewDirection = cameraLock->GetViewDir();
		}
	}

	void RacrozRenderer::UpdateCameraBuffers(const CameraData & cameraData)
	{
		m_TempCameraData = cameraData;
	}

	void RacrozRenderer::UpdateCameraBuffersPerspective(const Vector3 & position, const Vector3 & target, float fov, float nearPlane, float farPlane)
	{
		m_TempCameraData.FarPlane.m_x = farPlane;
		m_TempCameraData.NearPlane.m_x = nearPlane;

		m_TempCameraData.ViewDirection = (target - position).GetNormalized();
		m_TempCameraData.ViewPosition = position;

		m_TempCameraData.ViewMatrix = m_TempCameraData.PreviousViewMatrix = Matrix4::LookAtMatrix(position, Vector3(0, 1, 0), target).GetTransposed();
	
		m_TempCameraData.ProjectionMatrix = m_TempCameraData.PreviousProjectionMatrix = Matrix4::PerpsProjectedSpace(Math::DegreesToRad(fov), m_Width / m_Height, nearPlane, farPlane).GetTransposed();
	}

	void RacrozRenderer::UpdateCameraBuffersOrtographic(const Vector3 & position, const Vector3 & target, float width, float height, float nearPlane, float farPlane)
	{
		m_TempCameraData.FarPlane.m_x = width;
		m_TempCameraData.NearPlane.m_x = height;

		m_TempCameraData.ViewDirection = (target - position).GetNormalized();
		m_TempCameraData.ViewPosition = position;

		m_TempCameraData.ViewMatrix = m_TempCameraData.PreviousViewMatrix = Matrix4::LookAtMatrix(position, Vector3(0, 1, 0), target).GetTransposed();

		m_TempCameraData.ProjectionMatrix = m_TempCameraData.PreviousProjectionMatrix = Matrix4::OrthoProjectedSpace(width, height, nearPlane, farPlane).GetTransposed();
	}

	void RacrozRenderer::UpdateCameraBuffersMatrices(const Matrix4 & viewMatrix, const Matrix4 & projMatrix)
	{
		m_TempCameraData.ViewMatrix = m_TempCameraData.PreviousViewMatrix = viewMatrix.GetTransposed();

		m_TempCameraData.ProjectionMatrix = m_TempCameraData.PreviousProjectionMatrix = projMatrix.GetTransposed();
	}

	void RacrozRenderer::SetCameraBuffersInPipeline(int pixelShaderSlot, int vertexShaderSlot)
	{
		m_CameraBuffer.UpdateConstantBuffer(&m_TempCameraData, m_gfx);
		m_CameraBuffer.SetBufferInPS(pixelShaderSlot, m_gfx);
		m_CameraBuffer.SetBufferInVS(vertexShaderSlot, m_gfx);
		m_CameraBuffer.SetBufferInDS(vertexShaderSlot, m_gfx);
		m_CameraBuffer.SetBufferInHS(vertexShaderSlot, m_gfx);
	}
};
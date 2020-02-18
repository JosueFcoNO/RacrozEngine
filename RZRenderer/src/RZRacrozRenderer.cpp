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

		///Init the screen quad
		m_ScreenQuad.InitScreenQuad(m_gfx);

		///Compile, create and reflect the ScreenQuad Vertex Shader.
		m_gfx->CompileAndCreateVertexShader(m_ScreenQuadVS, L"Shaders/PassVShader.hlsl");
		m_ScreenQuadVS.ReflectLayout(1, m_gfx);

		m_ActiveSkyBox = std::make_shared<SkyBox>();
	}

	void RacrozRenderer::SetRenderingMode(eRenderingPipelines renderingMode)
	{
		switch (renderingMode)
		{
		case eRenderingPipelines::Forward:
			//SetForward();
			break;
		case eRenderingPipelines::Deferred:
			m_CurrentPipeline = std::make_shared<PipelineDeferred>();
			m_CurrentPipeline->InitRenderPipeline("Deferred", m_Width, m_Height, this);
			break;
		case eRenderingPipelines::ForwardPlus:
			//SetForwardPlus();
			break;
		case eRenderingPipelines::Debug:
			m_CurrentPipeline = std::make_shared<PipelineDebug>();
			m_CurrentPipeline->InitRenderPipeline("Debug", m_Width, m_Height, this);
			break;
		default:
			break;
		}

		m_BlurPass.SetRenderingMode(renderingMode);
	}

	void RacrozRenderer::Render(Scene * sceneGraph, ImGUIEditor * editor)
	{
		PrepareRender(sceneGraph);

		m_CurrentPipeline->DoRender();
	}

	WeakPtr<RenderPipeline> RacrozRenderer::GetCurrentPipeline()
	{
		return m_CurrentPipeline;
	}

	void RacrozRenderer::Destroy()
	{

	}

	//void RacrozRenderer::SetForward()
	//{
		/////Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		/////This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		//m_RTs["ColorCorrection"] = CreateRenderTargetAndTexture_XYScales("ColorCorrection", m_Textures["ColorCorrection"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);
		//m_RTs["ColorPBR"] = CreateRenderTargetAndTexture_XYScales("ColorPBR", m_Textures["ColorPBR"], 2, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT);
		//
		///////////////////
		/////Skybox PASS///
		///////////////////
		//
		//auto passSkyBox = CreatePass("SkyBox", ePasses::Skybox, m_CurrentRenderingMode);
		//
		//passSkyBox->AddRenderTarget(m_RTs["ColorPBR"], 0);
		//passSkyBox->AddRasterizerState(&m_RSSolidCullNone);
		//
		//m_PassesOrder.push_back("SkyBox");
		//
		///////////
		/////PBR///
		///////////
		//
		/////Create the PBR pass
		//auto passPBR = CreatePass("PBR", ePasses::PbrForward, m_CurrentRenderingMode);
		//
		//passPBR->AddRenderTarget(m_RTs["ColorPBR"], 0);
		//
		//m_PassesOrder.push_back("PBR");
		//
		/////Start the post processing.
		//m_PassesOrder.push_back("PostProcess");
		//
		////////////////////////
		/////COLOR CORRECTION///
		////////////////////////
		//
		/////Create the PBR pass
		//auto passColorCorrection = CreatePass("ColorCorrection", ePasses::GammaCorrection, m_CurrentRenderingMode);
		//
		//passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"], 0);
		//passColorCorrection->AddTexture2D(m_Textures["ColorPBR"], 0);
		//
		//m_PassesOrder.push_back("ColorCorrection");
	//}

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
		///renderTargets[0] = CreateRenderTargetAndTexture_WidthHeight("0", Textures[0], 1, width, height, format);
		///renderTargets[1] = CreateRenderTargetAndTexture_WidthHeight("1", Textures[1], 1, width, height, format);
		///renderTargets[2] = CreateRenderTargetAndTexture_WidthHeight("2", Textures[2], 1, width, height, format);
		///renderTargets[3] = CreateRenderTargetAndTexture_WidthHeight("3", Textures[3], 1, width, height, format);
		///renderTargets[4] = CreateRenderTargetAndTexture_WidthHeight("4", Textures[4], 1, width, height, format);
		///renderTargets[5] = CreateRenderTargetAndTexture_WidthHeight("5", Textures[5], 1, width, height, format);
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

	void RacrozRenderer::PrepareRender(Scene * sceneGraph)
	{
		return;

		m_ObjectsToRender.clear();

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

						if (m_ObjectsToRender.find(hashRender) == m_ObjectsToRender.end())
						{
							m_ObjectsToRender[hashRender] = MMap<float, WeakGameObjPtr>();
						}
						
						m_ObjectsToRender[hashRender].insert(Pair<float, WeakGameObjPtr>(Magnitude, gameObj));
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
					//	m_ObjectsToRender[hash].insert(gameObj);
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

	void RacrozRenderer::RenderObjs(bool forward, eComponentID componentID, eMaterialType matType, eShadingType shading, eBlendType blendType, bool Tesselated = false, bool TwoSided = false, bool blendedMaterial = false, bool wireframe = false)
	{
		auto renderHash = MaterialRenderInfo::CalculateRenderHash(componentID, matType, shading, blendType, Tesselated, TwoSided, blendedMaterial, wireframe);
		auto scene = SceneManager::Pointer()->GetActiveScene();

		if (forward)
		{
			for (auto it = m_ObjectsToRender[renderHash].begin(); it != m_ObjectsToRender[renderHash].end(); ++it)
			{
				it->second.lock()->PreRender();
				it->second.lock()->Render(componentID, renderHash);
			}
		}
		else
		{
			for (auto it = m_ObjectsToRender[renderHash].rbegin(); it != m_ObjectsToRender[renderHash].rend(); ++it)
			{
				it->second.lock()->PreRender();
				it->second.lock()->Render(componentID, renderHash);
			}
		}

	}

	void RacrozRenderer::RenderScreenAlignedQuad()
	{
		Gfx::GfxCore::Pointer()->DrawIndexed(6, 0, 0);
	}

	void RacrozRenderer::DoBlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height)
	{
		m_BlurPass.BlurPass(outRenderTarget, inTexture, width, height);
	}

	void RacrozRenderer::StartPostProcessing()
	{
		m_ScreenQuad.SetScreenQuadBuffers(m_gfx);
		m_ScreenQuadVS.SetThisVertexShaderAndInputLayout(m_gfx);
		//m_gfx->ClearDepthTargetView();
	}

	StrPtr<Gfx::RenderTarget> RacrozRenderer::CreateRenderTargetAndTexture_XYScales(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, float screenWidthScale, float screenHeightScale, Gfx::eFORMAT format)
	{
		return CreateRenderTargetAndTexture_WidthHeight(name, out_Texture, mipMaps, Math::Truncate(m_Width*screenWidthScale), Math::Truncate(m_Height*screenHeightScale), format);
	}

	StrPtr<Gfx::RenderTarget> RacrozRenderer::CreateRenderTargetAndTexture_WidthHeight(const String& name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format)
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
		m_LinealWrapSampler.PSSetThisSamplerState(0, 1, m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 1
		m_AnisotropicWrapSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_ANISOTROPIC, Vector4(1, 1, 1, 1), 16);
		m_AnisotropicWrapSampler.CreateSamplerState(m_gfx);
		m_AnisotropicWrapSampler.PSSetThisSamplerState(1, 1, m_gfx);

		///Create a LinearWrapSampler and set it on slot 2
		m_LinealClampSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_CLAMP, Gfx::COMPARISON_NEVER);
		m_LinealClampSampler.CreateSamplerState(m_gfx);
		m_LinealClampSampler.PSSetThisSamplerState(2, 1, m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 3
		m_AnisotropicClampSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_ANISOTROPIC, Vector4(1, 1, 1, 1), 16);
		m_AnisotropicClampSampler.CreateSamplerState(m_gfx);
		m_AnisotropicClampSampler.PSSetThisSamplerState(3, 1, m_gfx);

		///Create a AnisotropicWrapSampler and set it on slot 3
		m_PointWrapSampler.Init(Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::eTEXTURE_ADDRESS::TEXTURE_ADDRESS_WRAP, Gfx::COMPARISON_NEVER, Gfx::FILTER_MIN_MAG_MIP_POINT, Vector4(1, 1, 1, 1), 16);
		m_PointWrapSampler.CreateSamplerState(m_gfx);
		m_PointWrapSampler.PSSetThisSamplerState(4, 1, m_gfx);

	}
};
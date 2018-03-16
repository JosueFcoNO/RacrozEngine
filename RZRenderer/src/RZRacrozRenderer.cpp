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
		m_ScreenQuadVS.ReflectLayout(m_gfx);

		m_ActiveSkyBox = std::make_shared<SkyBox>();

		SetRenderingMode(DEFERRED);

		m_BlurPass.SetRenderingMode(DEFERRED);

#ifndef PROFILING
		Profiler::Pointer()->StartProfiler();
#endif

	}

	void RacrozRenderer::SetRenderingMode(RENDERING_MODE renderingMode)
	{
		if (m_CurrentRenderingMode == renderingMode)
		{
			return;
		}

		m_CurrentRenderingMode = renderingMode;

		switch (renderingMode)
		{
		case rczEngine::FORWARD:
			SetForward();
			break;
		case rczEngine::DEFERRED:
			SetDeferred();
			break;
		case rczEngine::FORWARD_PLUS:
			SetForwardPlus();
			break;
		default:
			break;
		}
	}

	void RacrozRenderer::Render(Scene * sceneGraph, GUIEditor * editor)
	{
		int32 i = 0;

		for (i = 0; i < m_PassesOrder.size(); ++i)
		{
			if (m_PassesOrder[i] == "PostProcess")
			{
				StartPostProcessing();
				continue;
			}

			auto pass = m_Passes[m_PassesOrder[i]];
			
			pass->PreRenderPass();
			pass->RenderPass();
			pass->PostRenderPass();
		}

		static StrPtr<PBR_Pass> pbr = std::dynamic_pointer_cast<PBR_Pass, Pass>(m_Passes["PBR"]);

		//If there exists an editor, render it.
		if (editor)
			editor->RenderEditor(&pbr->config);

		ImGui::Begin("Renderer");
		{
			if (ImGui::Button("SkyBox"))
			{
				StrPtr<SkyBox> Sky = std::make_shared<SkyBox>();
				Sky->InitSkyBox(LoadFile("SkyBox", "Sky", ResVault::Pointer()), Gfx::GfxCore::Pointer(), ResVault::Pointer());
				RacrozRenderer::Pointer()->ChangeSkyBox(Sky);
			}
		};

		ImGui::Render();
		GUIEditor::Pointer()->PreRender(ImGui::GetDrawData());

		m_gfx->Present();
	}

	void RacrozRenderer::Destroy()
	{

	}

	void RacrozRenderer::SetDeferred()
	{
		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		m_RTs["ColorAO"] = CreateRenderTargetAndTexture_XYScales("ColorAO", m_Textures["ColorAO"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		m_RTs["Position"] = CreateRenderTargetAndTexture_XYScales("Position", m_Textures["Position"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT);
		m_RTs["NormalsMR"] = CreateRenderTargetAndTexture_XYScales("Normals", m_Textures["NormalsMR"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT);
		m_RTs["Emmisive"] = CreateRenderTargetAndTexture_XYScales("EmmisiveTex", m_Textures["Emmisive"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);
		m_RTs["Specular"] = CreateRenderTargetAndTexture_XYScales("Specular", m_Textures["Specular"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);

		m_RTs["ColorCorrection"] = CreateRenderTargetAndTexture_XYScales("ColorCorrection", m_Textures["ColorCorrection"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);
		
		m_RTs["PBR"] = CreateRenderTargetAndTexture_XYScales("PBR", m_Textures["PBR"], 2, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT);

		m_RTs["Luminance"] = CreateRenderTargetAndTexture_XYScales("Luminance", m_Textures["Luminance"], 11, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		m_RTs["Bright"] = CreateRenderTargetAndTexture_XYScales("Bright", m_Textures["Bright"], 2, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		m_RTs["Bloom"] = CreateRenderTargetAndTexture_XYScales("Bloom", m_Textures["Bloom"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		m_RTs["AvgLuminance"] = CreateRenderTargetAndTexture_XYScales("AvgLuminance", m_Textures["AvgLuminance"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
		
		m_RTs["HDRBloom"] = CreateRenderTargetAndTexture_XYScales("HDRBloom", m_Textures["HDRBloom"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);

		/////////////////
		///SKYBOX PASS///
		/////////////////

		auto passSkyBox = CreatePass("SkyBox", PASSES::SKYBOX, m_CurrentRenderingMode);

		passSkyBox->AddRenderTarget(m_RTs["ColorAO"].get(), 0);
		passSkyBox->AddRasterizerState(&m_RSSolidCullNone);

		m_PassesOrder.push_back("SkyBox");

		///////////////////
		///GEOMETRY PASS///
		///////////////////

		///Create the geometry pass
		auto passGeometry = CreatePass("Geometry", PASSES::GEOMETRY_PASS, m_CurrentRenderingMode);

		passGeometry->AddRenderTarget(m_RTs["ColorAO"].get(), 0);
		passGeometry->AddRenderTarget(m_RTs["Position"].get(), 1);
		passGeometry->AddRenderTarget(m_RTs["NormalsMR"].get(), 2);
		passGeometry->AddRenderTarget(m_RTs["Emmisive"].get(), 3);
		passGeometry->AddRenderTarget(m_RTs["Specular"].get(), 4);

		m_PassesOrder.push_back("Geometry");
	
		///////////////////
		///TERRAIN GEOMETRY PASS///
		///////////////////

		///Create the geometry pass
		auto passTerrainGeometry = CreatePass("Terrain", PASSES::TERRAIN_GEOMETRY_PASS, m_CurrentRenderingMode);

		passTerrainGeometry->AddRenderTarget(m_RTs["ColorAO"].get(), 0);
		passTerrainGeometry->AddRenderTarget(m_RTs["Position"].get(), 1);
		passTerrainGeometry->AddRenderTarget(m_RTs["NormalsMR"].get(), 2);
		passTerrainGeometry->AddRenderTarget(m_RTs["Emmisive"].get(), 3);
		passTerrainGeometry->AddRenderTarget(m_RTs["Specular"].get(), 4);

		m_PassesOrder.push_back("Terrain");


		///Start the post processing.
		m_PassesOrder.push_back("PostProcess");

		/////////
		///PBR///
		/////////

		///Create the PBR pass
		auto passPBR = CreatePass("PBR", PASSES::PBR, m_CurrentRenderingMode);

		passPBR->AddRenderTarget(m_RTs["PBR"].get(), 0);

		passPBR->AddTexture2D(m_Textures["ColorAO"].get(), 0);
		passPBR->AddTexture2D(m_Textures["NormalsMR"].get(), 1);
		passPBR->AddTexture2D(m_Textures["Position"].get(), 2);
		//passPBRLight.AddTexture2D(BlurVTex, 5); //Final SSAO Blur
		//passPBRLight.AddTexture2D(ShadowFinalTex, 6);
		passPBR->AddTexture2D(m_Textures["Specular"].get(), 8);

		m_PassesOrder.push_back("PBR");

		///////////////
		///LUMINANCE///
		///////////////

		//Receives the PBR pass and outputs a R16F luminance map.

		///Create the Luminance pass
		auto passLuminance = CreatePass("Luminance", PASSES::LUMINANCE, m_CurrentRenderingMode);

		passLuminance->AddRenderTarget(m_RTs["Luminance"].get(), 0);
		passLuminance->AddTexture2D(m_Textures["PBR"].get(), 0);

		m_PassesOrder.push_back("Luminance");

		////////////
		///BRIGHT///
		////////////

		//Receives the Luminance pass and the Emmisive Map and outputs a RGB16F bright map to blur to get bloom and glow.

		///Create the Bright pass
		auto passBright = CreatePass("Bright", PASSES::BRIGHT, m_CurrentRenderingMode);

		passBright->AddRenderTarget(m_RTs["Bright"].get(), 0);

		passBright->AddTexture2D(m_Textures["Luminance"].get(), 0);
		passBright->AddTexture2D(m_Textures["Emmisive"].get(), 1);

		m_PassesOrder.push_back("Bright");

		///////////
		///BLOOM///
		///////////

		//Receives the Bright pass and blurs it 4 times to generate bloom and glow.

		StrPtr<BloomPass> passBloom = std::static_pointer_cast<BloomPass,Pass>(CreatePass("Bloom", PASSES::BLOOM, m_CurrentRenderingMode));

		passBloom->AddRenderTarget(m_RTs["Bloom"].get(), 0);

		passBloom->AddTexture2D(m_Textures["Bright"].get(), 0);

		passBloom->SetOriginalBloom(m_Textures["Bright"]);
		passBloom->SetLastBloomResult(m_Textures["Bloom"]);

		m_PassesOrder.push_back("Bloom");
		m_PassesOrder.push_back("Bloom");
		m_PassesOrder.push_back("Bloom");
		m_PassesOrder.push_back("Bloom");

		////////////
		///AVGLUM///
		////////////

		//Receives the Luminance pass and calculates the average luminance.

		auto passAvgLuminance = CreatePass("AvgLuminance", PASSES::AVG_LUMINANCE, m_CurrentRenderingMode);

		passAvgLuminance->AddRenderTarget(m_RTs["AvgLuminance"].get(), 0);

		passAvgLuminance->AddTexture2D(m_Textures["Luminance"].get(), 0);

		m_PassesOrder.push_back("AvgLuminance");

		//////////////
		///HDRBLOOM///
		//////////////

		//Receives the AvgLuminance, PBR and Bloom, does HDR and Bloom.

		///Create the Bright pass
		auto passHDRBloom = CreatePass("HDRBloom", PASSES::HDR_BLOOM, m_CurrentRenderingMode);

		passHDRBloom->AddRenderTarget(m_RTs["HDRBloom"].get(), 0);

		passHDRBloom->AddTexture2D(m_Textures["PBR"].get(), 0);
		passHDRBloom->AddTexture2D(m_Textures["Bloom"].get(), 1);
		passHDRBloom->AddTexture2D(m_Textures["AvgLuminance"].get(), 2);

		m_PassesOrder.push_back("HDRBloom");

		//////////////////////
		///COLOR CORRECTION///
		//////////////////////

		///Create the PBR pass
		auto passColorCorrection = CreatePass("ColorCorrection", PASSES::COLOR_CORRECTION, m_CurrentRenderingMode);

		passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"].get(), 0);
		passColorCorrection->AddTexture2D(m_Textures["HDRBloom"].get(), 0);

		m_PassesOrder.push_back("ColorCorrection");
	}

	void RacrozRenderer::SetForward()
	{
		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		m_RTs["ColorCorrection"] = CreateRenderTargetAndTexture_XYScales("ColorCorrection", m_Textures["ColorCorrection"], 1, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM);
		m_RTs["ColorPBR"] = CreateRenderTargetAndTexture_XYScales("ColorPBR", m_Textures["ColorPBR"], 2, 1.0f, 1.0f, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT);

		/////////////////
		///SKYBOX PASS///
		/////////////////

		auto passSkyBox = CreatePass("SkyBox", PASSES::SKYBOX, m_CurrentRenderingMode);

		passSkyBox->AddRenderTarget(m_RTs["ColorPBR"].get(), 0);
		passSkyBox->AddRasterizerState(&m_RSSolidCullNone);

		m_PassesOrder.push_back("SkyBox");

		/////////
		///PBR///
		/////////

		///Create the PBR pass
		auto passPBR = CreatePass("ColorPBR", PASSES::PBR_FORWARD, m_CurrentRenderingMode);

		passPBR->AddRenderTarget(m_RTs["ColorPBR"].get(), 0);

		m_PassesOrder.push_back("ColorPBR");

		///Start the post processing.
		m_PassesOrder.push_back("PostProcess");

		//////////////////////
		///COLOR CORRECTION///
		//////////////////////

		///Create the PBR pass
		auto passColorCorrection = CreatePass("ColorCorrection", PASSES::COLOR_CORRECTION, m_CurrentRenderingMode);

		passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"].get(), 0);
		passColorCorrection->AddTexture2D(m_Textures["ColorPBR"].get(), 0);

		m_PassesOrder.push_back("ColorCorrection");
	}

	void RacrozRenderer::SetForwardPlus()
	{

	}

	void RacrozRenderer::RenderScene(Scene * sceneGraph, eCOMPONENT_ID componentID, MATERIAL_TYPE matType, bool Forward)
	{
		auto ObjectList = sceneGraph->FindActorsWithComponent(componentID);
		auto CurrentCamera = CameraManager::Pointer()->GetActiveCamera().lock();
		auto pos = CurrentCamera->GetPosition();
		auto dir = CurrentCamera->GetOrientation();

		auto size = ObjectList.size();

		MMap<float, WeakPtr<GameObject>> ObjectMap;

		for (int32 i = 0; i < size; ++i)
		{
			auto act = ObjectList[i].lock();

			auto VectorToObj = (act->GetPosition() - pos);
			float Magnitude = VectorToObj.Magnitude();

			float Visible = VectorToObj.GetNormalized() | dir;

			ObjectMap.insert(Pair<float, WeakPtr<GameObject>>(Magnitude, act));
			//if (Visible > 0.0f)
			//{
			//	ObjectMap[Magnitude] = act;
			//}
		}

		if (Forward)
		{
			for (auto it = ObjectMap.begin(); it != ObjectMap.end(); ++it)
			{
				(*it).second.lock()->PreRender(sceneGraph);
				(*it).second.lock()->Render(sceneGraph, componentID, matType);
			}
		}
		else
		{
			for (auto it = ObjectMap.rend(); it != ObjectMap.rbegin(); --it)
			{
				(*it).second.lock()->PreRender(sceneGraph);
				(*it).second.lock()->Render(sceneGraph, componentID, matType);
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
		m_gfx->ClearDepthTargetView();
	}

	StrPtr<Gfx::RenderTarget> RacrozRenderer::CreateRenderTargetAndTexture_XYScales(const char* name, StrPtr<Texture2D>& out_Texture, int mipMaps, float screenWidthScale, float screenHeightScale, Gfx::eFORMAT format)
	{
		return CreateRenderTargetAndTexture_WidthHeight(name, out_Texture, mipMaps, Math::Truncate(m_Width*screenWidthScale), Math::Truncate(m_Height*screenHeightScale), format);
	}

	StrPtr<Gfx::RenderTarget> RacrozRenderer::CreateRenderTargetAndTexture_WidthHeight(const char * name, StrPtr<Texture2D>& out_Texture, int mipMaps, int32 width, int32 height, Gfx::eFORMAT format)
	{
		///Allocate a new RenderTarget Object
		auto renderTarget = std::make_shared<Gfx::RenderTarget>();
		
		///Save the Pointer on the RenderTarget Vector.
		m_RTs[name] = (renderTarget);

		bool SetToFinal = false;

		if (name == "ColorCorrection")
		{
			SetToFinal = true;
		}

		///Create the RenderTarget.
		m_gfx->CreateRenderTarget(*renderTarget, name, SetToFinal, mipMaps, width, height, format);

		///Just in case the texture is already allocated. delete the texture. The pointers that referenced that texture will now reference this new one.
		if (out_Texture)
		{
			delete out_Texture.get();
		}

		out_Texture = std::make_shared<Texture2D>();
		out_Texture->CreateFromRenderTarget(*renderTarget);

		m_res->InsertResource(out_Texture.get());

		return renderTarget;
	}

	StrPtr<Pass> RacrozRenderer::CreatePass(const char * name, PASSES pass, RENDERING_MODE renderMode)
	{
		StrPtr<Pass> returnPass;

		switch (pass)
		{
		case PASSES::SKYBOX:
			returnPass = m_Passes[name] = std::make_shared<SkyBoxPass>();
			break;
		case PASSES::GEOMETRY_PASS:
			returnPass = m_Passes[name] = std::make_shared<GeometryPass>();
			break;
		case PASSES::TERRAIN_GEOMETRY_PASS:
			returnPass = m_Passes[name] = std::make_shared<TerrainGeometryPass>();
			break;
		case PASSES::PBR:
			returnPass = m_Passes[name] = std::make_shared<PBR_Pass>();
			break;
		case PASSES::COLOR_CORRECTION:
			returnPass = m_Passes[name] = std::make_shared<ColorCorrectionPass>();
			break;
		case PASSES::PBR_FORWARD:
			returnPass = m_Passes[name] = std::make_shared<PBR_Forward_Pass>();
			break;
		case PASSES::LUMINANCE:
			returnPass = m_Passes[name] = std::make_shared<LuminancePass>();
			break;
		case PASSES::BRIGHT:
			returnPass = m_Passes[name] = std::make_shared<BrightPass>();
			break;
		case PASSES::BLOOM:
			returnPass = m_Passes[name] = std::make_shared<BloomPass>();
			break;
		case PASSES::AVG_LUMINANCE:
			returnPass = m_Passes[name] = std::make_shared<AverageLuminancePass>();
			break;
		case PASSES::HDR_BLOOM:
			returnPass = m_Passes[name] = std::make_shared<HDRBloomPass>();
			break;
		}

		returnPass->SetRenderingMode(renderMode);
		return returnPass;
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

	}
};
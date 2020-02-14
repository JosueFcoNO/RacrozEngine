#include <RZRendererPCH.h>

namespace rczEngine
{
	void PipelineDeferred::InitRenderPipeline(const String & name, int32 width, int32 height, RacrozRenderer * renderer)
	{
		Gfx::GfxCore::Pointer()->CreateDepthStencyl(depth, width, height);

		m_Renderer = renderer;

		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		CreateRenderTarget("ColorAO", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);
		CreateRenderTarget("Position", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 1);
		CreateRenderTarget("NormalsMR", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 1);
		CreateRenderTarget("Emmisive", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);
		CreateRenderTarget("Velocity", width, height, Gfx::FORMAT_R32G32_FLOAT, 1);
		CreateRenderTarget("Specular", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);

		CreateRenderTarget("ColorCorrection", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);

		CreateRenderTarget("PBR", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 2);

		CreateRenderTarget("MotionBlur", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 2);

		CreateRenderTarget("Luminance", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 9);
		CreateRenderTarget("Bright", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 2);
		CreateRenderTarget("Bloom", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 2);
		CreateRenderTarget("AvgLuminance", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 1);
		CreateRenderTarget("HDRBloom", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 1);
		
		CreateRenderTarget("SSAO", width, height, Gfx::FORMAT_R32_FLOAT, 2);

		/////////////////
		///SKYBOX PASS///
		/////////////////

		auto passSkyBox = m_Renderer->CreatePass(name + "SkyBox", PASSES::SKYBOX, RENDERING_MODE::DEFERRED);

		passSkyBox->AddRenderTarget(m_RTs["ColorAO"], 0);
		passSkyBox->AddRasterizerState(&m_Renderer->m_RSSolidCullNone);
		passSkyBox->AddDepthStencyl(&depth);

		m_PassesOrder.push_back(name + "SkyBox");

		///////////////////
		///GEOMETRY PASS///
		///////////////////

		///Create the geometry pass
		auto passGeometry = m_Renderer->CreatePass(name + "Geometry", PASSES::GEOMETRY_PASS, RENDERING_MODE::DEFERRED);

		passGeometry->AddRenderTarget(m_RTs["ColorAO"], 0);
		passGeometry->AddRenderTarget(m_RTs["Position"], 1);
		passGeometry->AddRenderTarget(m_RTs["NormalsMR"], 2);
		passGeometry->AddRenderTarget(m_RTs["Emmisive"], 3);
		passGeometry->AddRenderTarget(m_RTs["Velocity"], 4);
		passGeometry->AddRenderTarget(m_RTs["Specular"], 5);
		passGeometry->AddDepthStencyl(&depth);

		m_PassesOrder.push_back(name + "Geometry");

		/////Start the post processing.

		m_PassesOrder.push_back("PostProcess");

		////////////////////////
		/////SS Ambient Occlusion PASS
		////////////////////////
		
		///Create the geometry pass
		auto passSSAO = m_Renderer->CreatePass(name + "SSAO", PASSES::SSAO, RENDERING_MODE::DEFERRED);
		
		passSSAO->AddRenderTarget(m_RTs["SSAO"], 0);
		passSSAO->AddDepthStencyl(&depth);
		
		passSSAO->AddTexture2D(m_Textures["Position"], 0);
		passSSAO->AddTexture2D(m_Textures["NormalsMR"], 1);
		m_PassesOrder.push_back(name + "SSAO");

		/////////
		///PBR///
		/////////

		///Create the PBR pass
		auto passPBR = m_Renderer->CreatePass(name + "PBR", PASSES::PBR, RENDERING_MODE::DEFERRED);

		passPBR->AddRenderTarget(m_RTs["PBR"], 0);
		passPBR->AddDepthStencyl(&depth);

		passPBR->AddTexture2D(m_Textures["ColorAO"], 0);
		passPBR->AddTexture2D(m_Textures["NormalsMR"], 1);
		passPBR->AddTexture2D(m_Textures["Position"], 2);
		passPBR->AddTexture2D(m_Textures["Specular"], 3);
		passPBR->AddTexture2D(m_Textures["SSAO"], 5); //Final SSAO Blur
		//passPBRLight.AddTexture2D(ShadowFinalTex, 6);

		m_PassesOrder.push_back(name + "PBR");

		//////////////////////
		///Transparent PASS///
		//////////////////////

		///Create the geometry pass
		auto passTransparent = m_Renderer->CreatePass(name + "Transparent", PASSES::PBR_TRANSPARENT, RENDERING_MODE::DEFERRED);

		passTransparent->AddRenderTarget(m_RTs["PBR"], 0);
		passTransparent->AddRenderTarget(m_RTs["Position"], 1);
		passTransparent->AddRenderTarget(m_RTs["NormalsMR"], 2);
		passTransparent->AddDepthStencyl(&depth);

		m_PassesOrder.push_back(name + "Transparent");

		//////////////////////
		///GDebugger PASS///
		//////////////////////

		///Create the geometry pass
		auto passDebugger = m_Renderer->CreatePass(name + "Debugger", PASSES::GDEBUGGER, RENDERING_MODE::DEFERRED);
		
		passDebugger->AddRenderTarget(m_RTs["PBR"], 0);
		passDebugger->AddDepthStencyl(&depth);
		
		m_PassesOrder.push_back(name + "Debugger");

		///////////////////
		///PLANET PASS/////
		///////////////////

		auto passPlanet = m_Renderer->CreatePass(name + "Planet", PASSES::PLANET_PASS, RENDERING_MODE::DEFERRED);
		
		passPlanet->AddRenderTarget(m_RTs["PBR"], 0);
		passPlanet->AddRenderTarget(m_RTs["Position"], 1);
		passPlanet->AddRenderTarget(m_RTs["NormalsMR"], 2);
		//passPlanet->AddDepthStencyl(&depth);
		
		m_PassesOrder.push_back(name + "Planet");

		///////////////////
		///SCATTER PASS/////
		///////////////////

		auto passAtmos = m_Renderer->CreatePass(name + "Atmos", PASSES::ATMOS_SCATTER_PASS, RENDERING_MODE::DEFERRED);
		
		passAtmos->AddRenderTarget(m_RTs["PBR"], 0);
		passAtmos->AddDepthStencyl(&depth);
		
		m_PassesOrder.push_back(name + "Atmos");
		
		m_PassesOrder.push_back("PostProcess");

		/////////////////
		///MOTION BLUR///
		/////////////////

		///Create the PBR pass
		auto passMotionBlur = m_Renderer->CreatePass(name + "MotionBlur", PASSES::MOTION_BLUR, RENDERING_MODE::DEFERRED);
		
		passMotionBlur->AddRenderTarget(m_RTs["MotionBlur"], 0);
		passMotionBlur->AddDepthStencyl(&depth);
		
		passMotionBlur->AddTexture2D(m_Textures["PBR"], 0);
		passMotionBlur->AddTexture2D(m_Textures["Velocity"], 1);
		
		m_PassesOrder.push_back(name + "MotionBlur");

		///////////////
		///LUMINANCE///
		///////////////

		//Receives the PBR pass and outputs a R16F luminance map.

		//create the Luminance pass
		auto passLuminance = m_Renderer->CreatePass(name + "Luminance", PASSES::LUMINANCE, RENDERING_MODE::DEFERRED);
		
		passLuminance->AddRenderTarget(m_RTs["Luminance"], 0);
		passLuminance->AddTexture2D(m_Textures["MotionBlur"], 0);
		passLuminance->AddDepthStencyl(&depth);
		
		m_PassesOrder.push_back(name + "Luminance");

		////////////
		///BRIGHT///
		////////////

		//Receives the Luminance pass and the Emmisive Map and outputs a RGB16F bright map to blur to get bloom and glow.

		///Create the Bright pass
		auto passBright = m_Renderer->CreatePass(name + "Bright", PASSES::BRIGHT, RENDERING_MODE::DEFERRED);
		
		passBright->AddRenderTarget(m_RTs["Bright"], 0);
		passBright->AddDepthStencyl(&depth);
		
		passBright->AddTexture2D(m_Textures["Luminance"], 0);
		passBright->AddTexture2D(m_Textures["Emmisive"], 1);
		
		m_PassesOrder.push_back(name + "Bright");

		///////////
		///BLOOM///
		///////////

		//Receives the Bright pass and blurs it 4 times to generate bloom and glow.

		StrPtr<BloomPass> passBloom = std::static_pointer_cast<BloomPass, Pass>(m_Renderer->CreatePass(name + "Bloom", PASSES::BLOOM, RENDERING_MODE::DEFERRED));
		
		passBloom->AddRenderTarget(m_RTs["Bloom"], 0);
		
		passBloom->AddTexture2D(m_Textures["Bright"], 0);
		passBloom->AddDepthStencyl(&depth);
		
		passBloom->SetOriginalBloom(m_Textures["Bright"]);
		passBloom->SetLastBloomResult(m_Textures["Bloom"]);
		
		m_PassesOrder.push_back(name + "Bloom");
		m_PassesOrder.push_back(name + "Bloom");
		m_PassesOrder.push_back(name + "Bloom");
		m_PassesOrder.push_back(name + "Bloom");

		////////////
		///AVGLUM///
		////////////

		//Receives the Luminance pass and calculates the average luminance.

		auto passAvgLuminance = m_Renderer->CreatePass(name + "AvgLuminance", PASSES::AVG_LUMINANCE, RENDERING_MODE::DEFERRED);
		
		passAvgLuminance->AddRenderTarget(m_RTs["AvgLuminance"], 0);
		passAvgLuminance->AddDepthStencyl(&depth);
		
		passAvgLuminance->AddTexture2D(m_Textures["Luminance"], 0);
		
		m_PassesOrder.push_back(name + "AvgLuminance");

		//////////////
		///HDRBLOOM///
		//////////////

		//Receives the AvgLuminance, PBR and Bloom, does HDR and Bloom.

		///Create the Bright pass
		auto passHDRBloom = m_Renderer->CreatePass(name + "HDRBloom", PASSES::HDR_BLOOM, RENDERING_MODE::DEFERRED);
		
		passHDRBloom->AddRenderTarget(m_RTs["HDRBloom"], 0);
		passHDRBloom->AddDepthStencyl(&depth);
		
		passHDRBloom->AddTexture2D(m_Textures["MotionBlur"], 0);
		passHDRBloom->AddTexture2D(m_Textures["Bloom"], 1);
		passHDRBloom->AddTexture2D(m_Textures["AvgLuminance"], 2);
		
		m_PassesOrder.push_back(name + "HDRBloom");

		//////////////////////
		///COLOR CORRECTION///
		//////////////////////

		///Create the PBR pass
		auto passColorCorrection = m_Renderer->CreatePass(name + "ColorCorrection", PASSES::COLOR_CORRECTION, RENDERING_MODE::DEFERRED);

		passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"], 0);
		passColorCorrection->AddTexture2D(m_Textures["HDRBloom"], 0);
		passColorCorrection->AddDepthStencyl(&depth);

		m_PassesOrder.push_back(name + "ColorCorrection");
	}
	
	void PipelineDeferred::DoRender()
	{
		int32 i = 0;

		for (i = 0; i < m_PassesOrder.size(); ++i)
		{
			if (m_PassesOrder[i] == "PostProcess")
			{
				m_Renderer->StartPostProcessing();
				continue;
			}

			auto pass = m_Renderer->m_Passes[m_PassesOrder[i]];

			pass->PreRenderPass();
			pass->RenderPass();
			pass->PostRenderPass();
		}
	}
}
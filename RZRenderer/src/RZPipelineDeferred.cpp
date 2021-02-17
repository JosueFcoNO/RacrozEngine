#include <RZRendererPCH.h>

namespace rczEngine
{
	void PipelineDeferred::InitRenderPipeline(const String & name, int32 width, int32 height, RacrozRenderer * renderer)
	{
		depth = Gfx::GfxCore::Pointer()->GetDefaultDepthStencyl();

		m_Renderer = renderer;

		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		CreateRenderTarget("ColorAO", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);
		CreateRenderTarget("Position", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 1);
		CreateRenderTarget("NormalsMR", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 1);
		CreateRenderTarget("Emmisive", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);
		CreateRenderTarget("Velocity", width, height, Gfx::FORMAT_R32G32_FLOAT, 1);
		CreateRenderTarget("Specular", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 1);

		RacrozRenderer::Pointer()->m_Normals = m_Textures["NormalsMR"];
		RacrozRenderer::Pointer()->m_Albedo = m_Textures["ColorAO"];

		CreateRenderTarget("ColorCorrection", width, height, Gfx::FORMAT_R8G8B8A8_UNORM, 1);

		CreateRenderTarget("PBR", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 2);

		const auto shadowMapRes = RacrozRenderer::Pointer()->m_ShadowMapRes;

		CreateRenderTarget("ShadowMap", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 1);
		CreateRenderTarget("LightDepth", shadowMapRes, shadowMapRes, Gfx::FORMAT_R32_FLOAT, 1);

		CreateRenderTarget("MotionBlur", width, height, Gfx::FORMAT_R32G32B32A32_FLOAT, 2);

		CreateRenderTarget("Luminance", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 9);
		CreateRenderTarget("Bright", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 2);
		CreateRenderTarget("Bloom", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 2);
		CreateRenderTarget("AvgLuminance", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 1);
		CreateRenderTarget("HDRBloom", width, height, Gfx::FORMAT_R16G16B16A16_FLOAT, 1);
		
		/////////
		///Light Depth///
		/////////

		///Create the PBR pass
		auto passLightDepth = CreatePass(name + "LightDepth", ePasses::LightDepth, eRenderingPipelines::Deferred);

		passLightDepth->AddRenderTarget(m_RTs["LightDepth"], 0);

		ResVault::Pointer()->InsertResource(m_Textures["LightDepth"]);

		m_PassesOrder.push_back(name + "LightDepth");

		/////////////////
		///Skybox PASS///
		/////////////////

		auto passSkyBox = CreatePass(name + "SkyBox", ePasses::Skybox, eRenderingPipelines::Deferred);

		passSkyBox->AddRenderTarget(m_RTs["ColorAO"], 0);
		passSkyBox->AddRasterizerState(&m_Renderer->m_RSSolidCullNone);
		passSkyBox->AddDepthStencyl(depth);

		m_PassesOrder.push_back(name + "SkyBox");

		///////////////////
		///GEOMETRY PASS///
		///////////////////

		///Create the geometry pass
		auto passGeometry = CreatePass(name + "Geometry", ePasses::GeoBuffer, eRenderingPipelines::Deferred);

		passGeometry->AddRenderTarget(m_RTs["ColorAO"], 0);
		passGeometry->AddRenderTarget(m_RTs["Position"], 1);
		passGeometry->AddRenderTarget(m_RTs["NormalsMR"], 2);
		passGeometry->AddRenderTarget(m_RTs["Emmisive"], 3);
		passGeometry->AddRenderTarget(m_RTs["Velocity"], 4);
		passGeometry->AddRenderTarget(m_RTs["Specular"], 5);
		passGeometry->AddDepthStencyl(depth);

		m_PassesOrder.push_back(name + "Geometry");

		/////////
		///Shadow Map///
		/////////

		///Create the PBR pass
		auto passShadow = CreatePass(name + "ShadowMap", ePasses::ShadowMap, eRenderingPipelines::Deferred);

		passShadow->AddRenderTarget(m_RTs["ShadowMap"], 0);
		passShadow->AddDepthStencyl(depth);

		passShadow->AddTexture2D(m_Textures["LightDepth"], 0);
		passShadow->AddTexture2D(m_Textures["Position"], 1);
		passShadow->AddTexture2D(m_Textures["NormalsMR"], 2);

		m_PassesOrder.push_back(name + "ShadowMap");

		/////////
		///PBR///
		/////////

		///Create the PBR pass
		auto passPBR = CreatePass(name + "PBR", ePasses::PBR, eRenderingPipelines::Deferred);

		passPBR->AddRenderTarget(m_RTs["PBR"], 0);
		passPBR->AddDepthStencyl(depth);

		passPBR->AddTexture2D(m_Textures["ColorAO"], 0);
		passPBR->AddTexture2D(m_Textures["NormalsMR"], 1);
		passPBR->AddTexture2D(m_Textures["Position"], 2);
		passPBR->AddTexture2D(m_Textures["Specular"], 3);
		passPBR->AddTexture2D(m_Textures["ShadowMap"], 6);

		m_PassesOrder.push_back(name + "PBR");

		//////////////////////
		///Transparent PASS///
		//////////////////////

		///Create the geometry pass
		auto passTransparent = CreatePass(name + "Transparent", ePasses::PbrTransparent, eRenderingPipelines::Deferred);

		passTransparent->AddRenderTarget(m_RTs["PBR"], 0);
		passTransparent->AddRenderTarget(m_RTs["Position"], 1);
		passTransparent->AddRenderTarget(m_RTs["NormalsMR"], 2);
		passTransparent->AddDepthStencyl(depth);

		m_PassesOrder.push_back(name + "Transparent");

		//////////////////////
		///GDebugger PASS///
		//////////////////////

		///Create the geometry pass
		auto passDebugger = CreatePass(name + "Debugger", ePasses::GraphicDebugger, eRenderingPipelines::Deferred);
		
		passDebugger->AddRenderTarget(m_RTs["PBR"], 0);
		passDebugger->AddDepthStencyl(depth);
		
		m_PassesOrder.push_back(name + "Debugger");

		///////////////////
		///PLANET PASS/////
		///////////////////

		//auto passPlanet = CreatePass(name + "Planet", ePasses::PlanetSurface, eRenderingPipelines::Deferred);
		//
		//passPlanet->AddRenderTarget(m_RTs["PBR"], 0);
		//passPlanet->AddRenderTarget(m_RTs["Position"], 1);
		//passPlanet->AddRenderTarget(m_RTs["NormalsMR"], 2);
		////passPlanet->AddDepthStencyl(depth);
		//
		//m_PassesOrder.push_back(name + "Planet");

		///////////////////
		///SCATTER PASS/////
		///////////////////

		//auto passAtmos = CreatePass(name + "Atmos", ePasses::PlanetAtmosphere, eRenderingPipelines::Deferred);
		//
		//passAtmos->AddRenderTarget(m_RTs["PBR"], 0);
		//passAtmos->AddDepthStencyl(depth);
		//
		//m_PassesOrder.push_back(name + "Atmos");
		
		/////////////////
		///MOTION BLUR///
		/////////////////

		///Create the PBR pass
		//auto passMotionBlur = CreatePass(name + "MotionBlur", ePasses::MotionBlur, eRenderingPipelines::Deferred);
		//
		//passMotionBlur->AddRenderTarget(m_RTs["MotionBlur"], 0);
		//passMotionBlur->AddDepthStencyl(depth);
		//
		//passMotionBlur->AddTexture2D(m_Textures["PBR"], 0);
		//passMotionBlur->AddTexture2D(m_Textures["Velocity"], 1);
		//
		//m_PassesOrder.push_back(name + "MotionBlur");

		///////////////
		///Luminance///
		///////////////

		//Receives the PBR pass and outputs a R16F luminance map.

		//create the Luminance pass
		//auto passLuminance = CreatePass(name + "Luminance", ePasses::Luminance, eRenderingPipelines::Deferred);
		//
		//passLuminance->AddRenderTarget(m_RTs["Luminance"], 0);
		//passLuminance->AddTexture2D(m_Textures["MotionBlur"], 0);
		//passLuminance->AddDepthStencyl(depth);
		//
		//m_PassesOrder.push_back(name + "Luminance");

		////////////
		///Bright///
		////////////

		//Receives the Luminance pass and the Emmisive Map and outputs a RGB16F bright map to blur to get bloom and glow.

		///Create the Bright pass
		//auto passBright = CreatePass(name + "Bright", ePasses::Bright, eRenderingPipelines::Deferred);
		//
		//passBright->AddRenderTarget(m_RTs["Bright"], 0);
		//passBright->AddDepthStencyl(depth);
		//
		//passBright->AddTexture2D(m_Textures["Luminance"], 0);
		//passBright->AddTexture2D(m_Textures["Emmisive"], 1);
		//
		//m_PassesOrder.push_back(name + "Bright");

		///////////
		///Bloom///
		///////////

		//Receives the Bright pass and blurs it 4 times to generate bloom and glow.

		//StrPtr<BloomPass> passBloom = std::static_pointer_cast<BloomPass, Pass>(CreatePass(name + "Bloom", ePasses::Bloom, eRenderingPipelines::Deferred));
		//
		//passBloom->AddRenderTarget(m_RTs["Bloom"], 0);
		//
		//passBloom->AddTexture2D(m_Textures["Bright"], 0);
		//passBloom->AddDepthStencyl(depth);
		//
		//passBloom->SetOriginalBloom(m_Textures["Bright"]);
		//passBloom->SetLastBloomResult(m_Textures["Bloom"]);
		//
		//m_PassesOrder.push_back(name + "Bloom");
		//m_PassesOrder.push_back(name + "Bloom");
		//m_PassesOrder.push_back(name + "Bloom");
		//m_PassesOrder.push_back(name + "Bloom");

		////////////
		///AVGLUM///
		////////////

		//Receives the Luminance pass and calculates the average luminance.

		//auto passAvgLuminance = CreatePass(name + "AvgLuminance", ePasses::AverageLuminance, eRenderingPipelines::Deferred);
		//
		//passAvgLuminance->AddRenderTarget(m_RTs["AvgLuminance"], 0);
		//passAvgLuminance->AddDepthStencyl(depth);
		//
		//passAvgLuminance->AddTexture2D(m_Textures["Luminance"], 0);
		//
		//m_PassesOrder.push_back(name + "AvgLuminance");

		//////////////
		///HDRBLOOM///
		//////////////

		//Receives the AvgLuminance, PBR and Bloom, does HDR and Bloom.

		///Create the Bright pass
		//auto passHDRBloom = CreatePass(name + "HDRBloom", ePasses::BloomApply, eRenderingPipelines::Deferred);
		//
		//passHDRBloom->AddRenderTarget(m_RTs["HDRBloom"], 0);
		//passHDRBloom->AddDepthStencyl(depth);
		//
		//passHDRBloom->AddTexture2D(m_Textures["MotionBlur"], 0);
		//passHDRBloom->AddTexture2D(m_Textures["Bloom"], 1);
		//passHDRBloom->AddTexture2D(m_Textures["AvgLuminance"], 2);
		//
		//m_PassesOrder.push_back(name + "HDRBloom");

		//////////////////////
		///COLOR CORRECTION///
		//////////////////////

		///Create the PBR pass
		auto passColorCorrection = CreatePass(name + "ColorCorrection", ePasses::GammaCorrection, eRenderingPipelines::Deferred);

		passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"], 0);
		passColorCorrection->AddTexture2D(m_Textures["PBR"], 0);
		passColorCorrection->AddDepthStencyl(depth);

		m_PassesOrder.push_back(name + "ColorCorrection");
	}
	
	void PipelineDeferred::DoRender()
	{
		int32 i = 0;

		for (i = 0; i < m_PassesOrder.size(); ++i)
		{
			auto pass = m_Passes[m_PassesOrder[i]];

			pass->PreRenderPass();
			pass->RenderPass();
			pass->PostRenderPass();
		}
	}
}
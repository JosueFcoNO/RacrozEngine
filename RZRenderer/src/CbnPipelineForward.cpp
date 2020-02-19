#include <RZRendererPCH.h>

namespace rczEngine
{
	void PipelineForward::InitRenderPipeline(const String & name, int32 width, int32 height, RacrozRenderer * renderer)
	{
		Gfx::GfxCore::Pointer()->CreateDepthStencyl(depth, width, height);

		m_Renderer = renderer;

		///Create Render targets and textures for Position,  Normals, Color, Metallic and Roughness, Tangents and Binormals.
		///This also inserts the render targets and the textures into the RenderTarget and Texture2D Maps.
		CreateRenderTarget("ColorCorrection", width, height, Gfx::eFORMAT::FORMAT_R8G8B8A8_UNORM, 1);
		CreateRenderTarget("ColorPBR", width, height, Gfx::eFORMAT::FORMAT_R32G32B32A32_FLOAT, 1);
		
		/////////////////
		///Skybox PASS///
		/////////////////
		
		auto passSkyBox = CreatePass("SkyBox", ePasses::Skybox, eRenderingPipelines::Forward);
		
		passSkyBox->AddRenderTarget(m_RTs["ColorPBR"], 0);
		passSkyBox->AddRasterizerState(&m_Renderer->m_RSSolidCullNone);
		
		m_PassesOrder.push_back("SkyBox");
		
		/////////
		///PBR///
		/////////
		
		///Create the PBR pass
		auto passPBR = CreatePass("PBR", ePasses::PbrForward, eRenderingPipelines::Forward);
		
		passPBR->AddRenderTarget(m_RTs["ColorPBR"], 0);
		
		m_PassesOrder.push_back("PBR");
		
		///Start the post processing.
		m_PassesOrder.push_back("PostProcess");
		
		//////////////////////
		///COLOR CORRECTION///
		//////////////////////
		
		///Create the PBR pass
		auto passColorCorrection = CreatePass("ColorCorrection", ePasses::GammaCorrection, eRenderingPipelines::Forward);
		
		passColorCorrection->AddRenderTarget(m_RTs["ColorCorrection"], 0);
		passColorCorrection->AddTexture2D(m_Textures["ColorPBR"], 0);
		
		m_PassesOrder.push_back("ColorCorrection");
	}
}
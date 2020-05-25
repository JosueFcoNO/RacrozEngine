#include <RZRendererPCH.h>

namespace rczEngine
{
	void RenderPipeline::DoRender()
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

	void RenderPipeline::CreateRenderTarget(const String & name, int32 width, int32 height, Gfx::eFORMAT format, int32 mipmaps)
	{
		m_RTs[name.c_str()] = m_Renderer->CreateRenderTexture(name, m_Textures[name], mipmaps, width, height, format);
	}

	StrPtr<Pass> RenderPipeline::CreatePass(const String & name, ePasses pass, eRenderingPipelines renderMode)
	{
		StrPtr<Pass> returnPass;

		switch (pass)
		{
		case ePasses::Skybox:
			returnPass = m_Passes[name] = std::make_shared<SkyBoxPass>();
			break;
		case ePasses::GeoBuffer:
			returnPass = m_Passes[name] = std::make_shared<GeometryPass>();
			break;
		case ePasses::TerrainGeoBuffer:
			returnPass = m_Passes[name] = std::make_shared<TerrainGeometryPass>();
			break;
		case ePasses::PlanetSurface:
			returnPass = m_Passes[name] = std::make_shared<PlanetPass>();
			break;
		case ePasses::PlanetAtmosphere:
			returnPass = m_Passes[name] = std::make_shared<AtmosScatterPass>();
			break;
		case ePasses::PBR:
			returnPass = m_Passes[name] = std::make_shared<PBR_Pass>();
			break;
		case ePasses::GammaCorrection:
			returnPass = m_Passes[name] = std::make_shared<ColorCorrectionPass>();
			break;
		case ePasses::PbrForward:
			returnPass = m_Passes[name] = std::make_shared<PBR_Forward_Pass>();
			break;
		case ePasses::PbrTransparent:
			returnPass = m_Passes[name] = std::make_shared<PBR_Transparent_Pass>();
			break;
		case ePasses::Luminance:
			returnPass = m_Passes[name] = std::make_shared<LuminancePass>();
			break;
		case ePasses::Bright:
			returnPass = m_Passes[name] = std::make_shared<BrightPass>();
			break;
		case ePasses::Bloom:
			returnPass = m_Passes[name] = std::make_shared<BloomPass>();
			break;
		case ePasses::AverageLuminance:
			returnPass = m_Passes[name] = std::make_shared<AverageLuminancePass>();
			break;
		case ePasses::BloomApply:
			returnPass = m_Passes[name] = std::make_shared<HDRBloomPass>();
			break;
		case ePasses::MotionBlur:
			returnPass = m_Passes[name] = std::make_shared<MotionBlurPass>();
			break;
		case ePasses::Perlin3D:
			returnPass = m_Passes[name] = std::make_shared<PerlinPlanetPass>();
			break;
		case ePasses::SSAO:
			returnPass = m_Passes[name] = std::make_shared<SSAOPass>();
			break;
		case ePasses::GraphicDebugger:
			returnPass = m_Passes[name] = std::make_shared<GraphicDebuggerPass>();
			break;
		}

		returnPass->m_Name = name;
		returnPass->CreatePipeline(renderMode);

		return returnPass;
	}
}
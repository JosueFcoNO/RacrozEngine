#include <RZRendererPCH.h>

namespace rczEngine
{
	void LightDepthPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();

		m_PShaderPath = L"Shaders/GPassLightDepth.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());
	}

	void LightDepthPass::PreRenderPass()
	{
		///Set the rasterizer state.
		m_gfx->SetRSState(RacrozRenderer::Pointer()->m_RSSolidCullNone);
	}

	void LightDepthPass::RenderPass()
	{
		auto activeScene = m_ActiveScene.get();
		auto renderer = RacrozRenderer::Pointer();

		m_gfx->SetViewPort(1024, 1024);

		m_PShader.SetThisPixelShader(m_gfx);
		renderer->DrawObjectsByProperties(true, CMP_MODEL_RENDERER, eMaterialType::AnyMaterial, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);
	}

	void LightDepthPass::PostRenderPass()
	{
	}
};
#include <RZRendererPCH.h>

namespace rczEngine
{
	void LightDepthPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();

		m_PShaderPath = L"Shaders/GPassLightDepth.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		const auto shadowMapRes = RacrozRenderer::Pointer()->m_ShadowMapRes;

		m_DepthStencyl = std::make_shared<Gfx::DepthStencyl>();
		Gfx::GfxCore::Pointer()->CreateDepthStencyl(*m_DepthStencyl, shadowMapRes, shadowMapRes);
	}

	void LightDepthPass::RenderLightPass(Light * light)
	{
		
	}

	void LightDepthPass::PreRenderPass()
	{
		auto light = LightManager::Pointer()->GetLight(0);

		if (light)
		{
			auto gfx = Gfx::GfxCore::Pointer();

			gfx->SetNumberOfRenderTargets(1);
			gfx->AddRenderTarget(*m_RenderTargets[0], 0);
			gfx->SetRenderTargets(true, &(*m_DepthStencyl));
			m_gfx->SetBlendStateDefault();

			gfx->ClearDepthTargetView(&(*m_DepthStencyl));
			gfx->ClearRenderTargetView(0, 0, 0, 0, 1);

			const auto res = RacrozRenderer::Pointer()->m_ShadowMapRes;

			///Set the rasterizer state.
			//m_gfx->SetRSState(RacrozRenderer::Pointer()->m_RSSolidCullFront);
			m_gfx->SetViewPort(res, res);
			m_PShader.SetThisPixelShader(m_gfx);

			auto activeScene = m_ActiveScene.get();
			auto renderer = RacrozRenderer::Pointer();

			Matrix4 view;
			Matrix4 proj;
			light->GetLightViewProjMatrix(view, proj);
			RacrozRenderer::Pointer()->UpdateCameraBuffersMatrices(view, proj);
			RacrozRenderer::Pointer()->SetCameraBuffersInPipeline(5, 5);
		}
	}

	void LightDepthPass::RenderPass()
	{
		auto light = LightManager::Pointer()->GetLight(0);

		if (light)
		{
			auto renderer = RacrozRenderer::Pointer();
			renderer->DrawObjectsByProperties(true, CMP_MODEL_RENDERER, eMaterialType::PBR_MetRough, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);

		}
	}

	void LightDepthPass::PostRenderPass()
	{
		auto light = LightManager::Pointer()->GetLight(0);

		if (light)
		{
			m_gfx->SetViewPortDefault();
			m_gfx->UnbindRenderTargets();
			m_gfx->SetRSStateDefault();
		}
	}
};
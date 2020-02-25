#include <RZRendererPCH.h>

namespace rczEngine
{
	void GeometryPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_ActiveScene = SceneManager::Pointer()->GetActiveScene();

		///Geometry Pixel Shader
		m_PShaderPath = L"Shaders/GPassMetRough.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());

		m_gfx->CompileAndCreatePixelShader(m_SmoothPS, L"Shaders/GPassSpecSmooth.hlsl");
		m_gfx->CompileAndCreatePixelShader(m_SmoothSpecAlphaPS, L"Shaders/GPassSpecSmoothAlpha.hlsl");
	}

	void GeometryPass::PreRenderPass()
	{
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearDepthTargetView();
		m_gfx->ClearRenderTargetView(1, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(2, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(3, 0.0f, 0.0f, 0.0f, 0.0f);
		m_gfx->ClearRenderTargetView(4, 0.0f, 0.0f, 0.0f, 0.0f);

		///Set the rasterizer state.
		m_gfx->SetRSStateDefault();
	}

	void GeometryPass::RenderPass()
	{
		auto activeScene = m_ActiveScene.get();
		auto renderer = RacrozRenderer::Pointer();

		///Normal Geometry

		m_PShader.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_MetRough, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);

		m_SmoothPS.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_SpecSmooth, eShadingType::PBR, eBlendType::Opaque, false, false, false, false);

		///Tesselated Geometry

		m_PShader.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_MetRough, eShadingType::PBR, eBlendType::Opaque, true, false, false, false);
		
		m_SmoothPS.SetThisPixelShader(m_gfx);
		renderer->RenderObjs(true, CMP_MODEL_RENDERER, eMaterialType::PBR_SpecSmooth, eShadingType::PBR, eBlendType::Opaque, true, false, false, false);

		//m_SkinnedVshader.SetThisVertexShaderAndInputLayout(m_gfx);
		//RacrozRenderer::PrepareRender(SceneManager::Pointer()->GetActiveScene().get(), CMP_SKINNED_MODEL_RENDERER, PBR_MetRough, true);
	}

	void GeometryPass::PostRenderPass()
	{
		m_gfx->UnbindRenderTargets();
		for (int32 i = 0; i < 10; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();

		RacrozRenderer::Pointer()->m_VXGI.RenderChannels();
	}
};
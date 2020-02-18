#include <RZRendererPCH.h>

namespace rczEngine
{
	void MotionBlurPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_PShaderPath = L"Shaders/MotionBlur.hlsl";
		m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());
		m_FPSBuffer.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, Gfx::GfxCore::Pointer());
		FPS.Set(0, 0, 0, 0);
	}

	void MotionBlurPass::PreRenderPass()
	{
		m_PShader.SetThisPixelShader(m_gfx);
		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		FPS.m_x = ImGUIEditor::Pointer()->FPS;
		m_FPSBuffer.UpdateConstantBuffer(&FPS, m_gfx);
		m_FPSBuffer.SetBufferInPS(7, m_gfx);

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void MotionBlurPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void MotionBlurPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);
		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();
		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
}
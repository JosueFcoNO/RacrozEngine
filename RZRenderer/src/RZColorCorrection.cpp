#include <RZRendererPCH.h>

namespace rczEngine
{
	void ColorCorrectionPass::SetRenderingMode(RENDERING_MODE mode)
	{
		m_gfx->CompileAndCreatePixelShader(m_Pshader, L"Shaders/ColorCorrection.hlsl");

		GammaBuffer.Destroy();
		GammaBuffer.CreateConstantBuffer(sizeof(GammaSetting), Gfx::USAGE_DEFAULT, m_gfx);
		GammaBuffer.UpdateConstantBuffer(&GammaData, m_gfx);

		GUIEditor::Pointer()->ColorMode = GammaData.Correction;
	}

	void ColorCorrectionPass::PreRenderPass()
	{
		m_Pshader.SetThisPixelShader(m_gfx);

		GammaData.Exposure = GUIEditor::Pointer()->Exposure;
		GammaData.Correction = GUIEditor::Pointer()->ColorMode;
		GammaBuffer.UpdateConstantBuffer(&GammaData, m_gfx);
		GammaBuffer.SetBufferInPS(8, m_gfx);

		SetRenderTargetsInPipeline();
		SetTexturesInPipeline();
		SetRasterizerState();

		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
	}

	void ColorCorrectionPass::RenderPass()
	{
		RacrozRenderer::RenderScreenAlignedQuad();
	}

	void ColorCorrectionPass::PostRenderPass()
	{
		for (int32 i = 0; i < MAX_TEXTURES_PASS; ++i)
			m_gfx->UnbindPSShaderResource(i);

		if (m_RasterizerState)
			m_gfx->SetRSStateDefault();

		if (m_BlendState)
			m_gfx->SetBlendStateDefault();
	}
};
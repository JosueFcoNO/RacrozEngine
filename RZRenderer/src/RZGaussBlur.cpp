#include <RZRendererPCH.h>

namespace rczEngine
{
	void GaussPass::CreatePipeline(eRenderingPipelines mode)
	{
		m_gfx->CompileAndCreatePixelShader(m_HorShader, L"Shaders/Blur/BlurH.hlsl");
		m_gfx->CompileAndCreatePixelShader(m_VerShader, L"Shaders/Blur/BlurV.hlsl");

		m_BlurInfoCB.Destroy();
		m_BlurInfoCB.CreateConstantBuffer(sizeof(BlurData), Gfx::USAGE_DEFAULT, m_gfx);
		m_BlurInfoCB.UpdateConstantBuffer(&m_BlurInfo, m_gfx);

		m_Blur = RacrozRenderer::Pointer()->
			CreateRenderTexture("BlurMid", m_BlurTex, 9, m_gfx->GetWidth(), m_gfx->GetHeight(), Gfx::eFORMAT::FORMAT_R16G16B16A16_FLOAT);
	}

	void GaussPass::BlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height)
	{
		//Update the CBuffer
		m_BlurInfo.TextureSizeX = width;
		m_BlurInfo.TextureSizeY = width;
		m_BlurInfoCB.UpdateConstantBuffer(&m_BlurInfo, m_gfx);
		m_BlurInfoCB.SetBufferInPS(0, m_gfx);

		DoBlur(BLUR_H, m_Blur, inTexture);
		m_gfx->GenerateMipMaps(inTexture->GetTextureCore());

		DoBlur(BLUR_V, outRenderTarget, m_BlurTex);
		m_gfx->GenerateMipMaps(m_BlurTex->GetTextureCore());
	}

	void GaussPass::DoBlur(BLUR_PASS pass, StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture)
	{
		switch (pass)
		{
		default:
		case rczEngine::GaussPass::BLUR_H:
			m_HorShader.SetThisPixelShader(m_gfx);
			break;
		case rczEngine::GaussPass::BLUR_V:
			m_VerShader.SetThisPixelShader(m_gfx);
			break;
		}

		//Set the render target in the pipeline
		m_gfx->SetNumberOfRenderTargets(1);
		m_gfx->AddRenderTarget(*outRenderTarget.get(), 0);
		m_gfx->SetRenderTargets(false);

		//Set the input texture on the pipeline
		inTexture->SetThisTextureInPS(0, 1, m_gfx);

		//Clear depth stencil and Render target.
		m_gfx->ClearRenderTargetView(0, 0, 0, 0, 0);
		m_gfx->ClearDepthTargetView();
		
		//Render the screen quad.
		RacrozRenderer::Pointer()->DrawScreenQuad();

		//unbind the texture.
		m_gfx->UnbindPSShaderResource(0);

	}

}
#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_EXP GaussPass : public Pass
	{
	public:
		struct BlurData
		{
			int TextureSizeX;
			int TextureSizeY;
			int Padding[2];
		};

		enum BLUR_PASS
		{
			BLUR_H,
			BLUR_V
		};

		virtual void SetRenderingMode(eRenderingPipelines mode);
		
		void BlurPass(StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture, int width, int height);

		///Do nothing on Blur Passes		
		virtual void PreRenderPass() {};
		virtual void RenderPass() {};
		virtual void PostRenderPass() {};

	private:
		void DoBlur(BLUR_PASS pass, StrPtr<Gfx::RenderTarget> outRenderTarget, StrPtr<Texture2D> inTexture);

		Gfx::ConstantBuffer m_BlurInfoCB;
		BlurData m_BlurInfo;

		StrPtr<Gfx::RenderTarget> m_Blur;
		StrPtr<Texture2D> m_BlurTex;

		Gfx::PixelShader m_HorShader;
		Gfx::PixelShader m_VerShader;

	};
}
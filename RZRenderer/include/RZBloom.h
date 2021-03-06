#pragma once

namespace rczEngine
{
	class RZ_EXP BloomPass : public Pass
	{
	public:
		virtual void CreatePipeline(eRenderingPipelines mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

		void SetOriginalBloom(StrPtr<Texture2D> originalBright) { m_OriginalBright = originalBright; };
		void SetLastBloomResult(StrPtr<Texture2D> lastBloomTexture) { m_LastBloomResult = lastBloomTexture; };

	private:
		int m_BloomPasses;

		StrPtr<Texture2D> m_OriginalBright;
		StrPtr<Texture2D> m_LastBloomResult;
		StrPtr<Texture2D> m_BlurredBloom;
		StrPtr<Gfx::RenderTarget> m_BlurredBloomRT;
	};
}
#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_UTILITY_EXPORT MotionBlurPass : public Pass
	{
	public:
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Do nothing on Blur Passes		
		virtual void PreRenderPass();
		virtual void RenderPass();
		virtual void PostRenderPass();

	private:
		Gfx::PixelShader m_BlurShader;
	};
}
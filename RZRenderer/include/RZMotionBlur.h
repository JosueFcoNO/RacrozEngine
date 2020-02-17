#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_EXP MotionBlurPass : public Pass
	{
	public:
		virtual void SetRenderingMode(eRenderingPipelines mode);

		///Do nothing on Blur Passes		
		virtual void PreRenderPass();
		virtual void RenderPass();
		virtual void PostRenderPass();
		Vector4 FPS;

	private:
		Gfx::ConstantBuffer m_FPSBuffer;
	};
}
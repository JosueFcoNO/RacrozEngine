#pragma once

namespace rczEngine
{
	struct GammaSetting
	{
		int Correction = 0;
		float Exposure = 1.0f;
		int padding[2];
	};

	///A Base Class to derive into concrete Passes.
	class RZ_UTILITY_EXPORT ColorCorrectionPass : public Pass
	{
	public:
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

	private:
		Gfx::PixelShader m_Pshader;

		GammaSetting GammaData;
		Gfx::ConstantBuffer GammaBuffer;

	};
}
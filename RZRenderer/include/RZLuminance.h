#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT LuminancePass : public Pass
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
		Gfx::PixelShader m_LuminanceShader;
	};
}
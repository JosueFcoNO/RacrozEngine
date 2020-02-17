#pragma once

namespace rczEngine
{
	class RZ_EXP LuminancePass : public Pass
	{
	public:
		virtual void SetRenderingMode(eRenderingPipelines mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

	private:
	};
}
#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_EXP GraphicDebuggerPass : public Pass
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
		Gfx::VertexShader m_VShader;
	};
}
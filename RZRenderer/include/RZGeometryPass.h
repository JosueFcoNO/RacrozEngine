#pragma once

namespace rczEngine
{
	class RZ_EXP GeometryPass : public Pass
	{
	public:
		///Inits the pass with a name and a rendering mode.
		virtual void CreatePipeline(eRenderingPipelines mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

	private:
		Gfx::PixelShader m_SmoothPS;
		Gfx::PixelShader m_SmoothSpecAlphaPS;

		StrPtr<Scene> m_ActiveScene;

	};

}
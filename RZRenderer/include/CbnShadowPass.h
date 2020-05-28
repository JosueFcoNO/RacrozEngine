#pragma once

namespace rczEngine
{
	class RZ_EXP ShadowPass : public Pass
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
		Gfx::PixelShader m_DefaultPS;
		Gfx::VertexShader m_ShadowVS;

		StrPtr<Scene> m_ActiveScene;
	};

}
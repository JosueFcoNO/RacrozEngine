#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT PlanetPass : public Pass
	{
	public:
		///Inits the pass with a name and a rendering mode.
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();

	private:
		Gfx::VertexShader m_PlanetVshader;
		Gfx::DomainShader m_PlanetDshader;
		Gfx::HullShader   m_PlanetHshader;
		Gfx::PixelShader  m_PlanetPshader;

		StrPtr<Scene> m_ActiveScene;

	};

}
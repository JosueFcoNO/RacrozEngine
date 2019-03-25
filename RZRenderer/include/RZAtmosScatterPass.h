#pragma once

namespace rczEngine
{
	class RZ_EXP AtmosScatterPass : public Pass
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
		Gfx::VertexShader m_GroundFromSpaceVS;
		Gfx::PixelShader  m_GroundFromSpacePS;

		Gfx::VertexShader m_GroundFromAtmosphereVS;
		Gfx::PixelShader  m_GroundFromAtmospherePS;

		Gfx::VertexShader m_SkyFromSpaceVS;
		Gfx::PixelShader  m_SkyFromSpacePS;

		Gfx::VertexShader m_SkyFromAtmosphereVS;
		Gfx::PixelShader  m_SkyFromAtmospherePS;

		StrPtr<Scene> m_ActiveScene;
		Gfx::BlendState Transparent;

		Gfx::RasterizerState m_CullBack;

	};

}
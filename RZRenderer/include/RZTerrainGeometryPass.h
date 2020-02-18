#pragma once

namespace rczEngine
{
	class RZ_EXP TerrainGeometryPass : public Pass
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
		Gfx::VertexShader m_Vshader;
		Gfx::DomainShader m_Dshader;
		Gfx::HullShader   m_Hshader;

		Gfx::RasterizerState wire;
	};

}
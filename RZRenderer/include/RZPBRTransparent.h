#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_EXP PBR_Transparent_Pass : public Pass
	{
	public:
		virtual void SetRenderingMode(eRenderingPipelines mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();
		RendererConfig config;

	private:
		StrPtr<Texture2D> m_LUT;

		Gfx::VertexShader m_GeometryPBRShader;
		Gfx::VertexShader m_SkinnedGeometryPBRShader;

		Gfx::ConstantBuffer UserDisney;
		
		Gfx::RasterizerState CullBack;
		Gfx::RasterizerState CullFront;

		Gfx::BlendState Transparent;
	};
}
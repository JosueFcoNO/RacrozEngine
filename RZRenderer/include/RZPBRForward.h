#pragma once

namespace rczEngine
{
	///A Base Class to derive into concrete Passes.
	class RZ_UTILITY_EXPORT PBR_Forward_Pass : public Pass
	{
	public:
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Sets the buffers, shaders, textures and render targets before rendering geomtry.
		virtual void PreRenderPass();

		///Renders the geometry from the pass.
		virtual void RenderPass();

		///Unbinds the render targets.
		virtual void PostRenderPass();
		UserConfig config;

	private:
		StrPtr<Texture2D> m_LUT;

		Gfx::PixelShader m_PBRShader;

		Gfx::VertexShader m_GeometryPBRShader;
		Gfx::VertexShader m_SkinnedGeometryPBRShader;

		Gfx::ConstantBuffer UserDisney;
	};
}
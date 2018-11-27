#pragma once

namespace rczEngine
{
	///A concrete SkyBox Pass implementation, using a normal CubeMap.
	class RZ_EXP PerlinPlanetPass : public Pass
	{
	public:
		///Sets the rendering mode for this Pass. Changes the shaders and buffers as needed.
		virtual void SetRenderingMode(RENDERING_MODE mode);
		virtual void PreRenderPass();
		virtual void RenderPass();
		virtual void PostRenderPass();

	private:
		///A cube model for sky box rendering.
		WeakPtr<Model> m_Cube;

		Gfx::VertexShader m_VShader;

		Gfx::RasterizerState rzState;
		ResourceHandle m_Planet;

		Matrix4 m_SkyMatrix;
		Gfx::ConstantBuffer m_SkyBoxBuffer;

	};
}
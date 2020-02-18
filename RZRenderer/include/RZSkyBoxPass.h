#pragma once

namespace rczEngine
{
	///A concrete SkyBox Pass implementation, using a normal CubeMap.
	class RZ_EXP SkyBoxPass : public Pass
	{
	public:
		///Sets the rendering mode for this Pass. Changes the shaders and buffers as needed.
		virtual void SetRenderingMode(eRenderingPipelines mode);
		virtual void PreRenderPass();
		virtual void RenderPass();
		virtual void PostRenderPass();

		void RenderPassGUI() final
		{
			Pass::RenderPassGUI();
			
			if (ImGui::Button("Recompile Vertex Shader"))
			{
				m_VShader.Destroy();
				m_gfx->CompileAndCreateVertexShader(m_VShader, L"Shaders/SkyBoxShader.hlsl");
				m_VShader.ReflectLayout(0, m_gfx);
			}


		}

	private:
		WeakPtr<CubeMap> m_CubeMap;

		///A cube model for sky box rendering.
		WeakPtr<Model> m_Cube;

		Gfx::VertexShader m_VShader;

		StringW VertexShaderPath;

		Matrix4 m_SkyMatrix;
		Gfx::ConstantBuffer m_SkyBoxBuffer;
	};
}
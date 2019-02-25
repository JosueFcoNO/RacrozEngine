#pragma once

namespace rczEngine
{
	struct SSAOsettings
	{
		float Sampler_Radius = 0.05f;
		float Bias = 0.1f;
		float Intensity = 1.0f;
		float Scale = 0.5f;
	};

	///A Base Class to derive into concrete Passes.
	class RZ_EXP SSAOPass : public Pass
	{
	public:
		virtual void SetRenderingMode(RENDERING_MODE mode);

		///Do nothing on Blur Passes		
		virtual void PreRenderPass();
		virtual void RenderPass();
		virtual void PostRenderPass();

#ifndef EDITOR
		void RenderPassGUI() override
		{
			ImGui::Text("%s", m_Name.c_str());



			if (ImGui::Button("Recompile Pixel Shader"))
			{
				m_PShader.Destroy();
				m_gfx->CompileAndCreatePixelShader(m_PShader, m_PShaderPath.c_str());
			}

			for (int i = 0; i < 8; ++i)
			{
				if (m_RenderTargets[i])
				{
					ImVec2 size = ImGui::GetWindowSize();
					size.y *= m_RenderTargets[i]->GetTextureCore()->m_Width / m_RenderTargets[i]->GetTextureCore()->m_Height;
					ImGui::Text("Renders Target %d: %s", i, m_RenderTargets[i]->m_Name.c_str());
					ImGui::Image(m_RenderTargets[i]->GetTextureCore()->m_ShaderResource, size);
				}
			}

			ImGui::SliderFloat("SamplerRadius", &settings.Sampler_Radius, 0.001f, 1.0f, "%.3f", 1);
			ImGui::SliderFloat("Bias", &settings.Bias, 0.001f, 1.0f, "%.3f", 1);
			ImGui::SliderFloat("Intensity", &settings.Intensity, 0.001f, 1.0f, "%.3f", 1);
			ImGui::SliderFloat("Scale", &settings.Scale, 0.001f, 1.0f, "%.3f", 1);

		}
#endif

	private:
		SSAOsettings settings;
		Gfx::ConstantBuffer settingsCB;
	};
}
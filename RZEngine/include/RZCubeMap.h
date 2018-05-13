#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CubeMap : public Texture2D
	{
	public:
		CubeMap() { m_Type = ResourceType::RES_CUBEMAP; };
		virtual ~CubeMap() {};

		void LoadCubeMapFromDDS(const char* filePath, const char* resName);
		void LoadCubeMapFrom6Images(char* front, char* back, char* left, char* right, char* top, char* down, char* resName);

#ifndef RZ_EDITOR
		virtual void RenderResourceGUI()
		{
			ImGui::Text("Texture Resource");
			ImVec2 size = ImGui::GetWindowSize();
			size.x -= 30;
			size.y -= 30;

			ImGui::Image(m_TextureCore.m_ShaderResource, size);
		}
#endif

		virtual void Serialize();
		virtual void DeSerialize();
	
	};
}
#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT CubeMap : public Texture2D
	{
	public:
		virtual ~CubeMap() {};

		void LoadCubeMapFromDDS(const char* filePath, const char* resName, bool addToResManager = true);
		void LoadCubeMapFrom6Images(char* front, char* back, char* left, char* right, char* top, char* down, char* resName, bool addToResManager = true);

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
	
	};
}
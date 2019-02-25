#pragma once

namespace rczEngine
{
	class RZ_EXP CubeMap : public Texture2D
	{
	public:
		CubeMap() { m_Type = ResourceType::RES_CUBEMAP; };
		virtual ~CubeMap() {};

		void LoadCubeMapFromDDS(const String& filePath, const String& resName);
		void LoadCubeMapFrom6Images(const String& front, const String& back, const String& left, const String& right, const String& top, const String& down, const String& resName);

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
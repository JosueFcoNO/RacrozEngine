#include <RZEditorPCH.h>

namespace rczEngine
{
	void ResourceListWindow::Render()
	{
		ImGui::Text("Resources");

		static int comboint = 0;
		static bool AddComponentMenu = false;
		static int componentIds[5] = { -1, RES_3DMODEL, RES_SKINNEDMODEL, RES_MATERIAL, RES_TEXTURE };

		ImGui::Combo("Filter", &comboint, "None \0Model \0Skinned Model \0Material \0Texture2D");

		UoMap<ResourceHandle, StrPtr<Resource>>* map = &ResVault::Pointer()->m_ResourceMap;
		MMap<String, StrPtr<Resource>> map2;

		if (ImGui::TreeNode("Resource List"))
		{
			int i = 0;
			for (auto it = map->begin(); it != map->end(); ++it)
			{
				if (it->second->m_Type == (ResourceType)componentIds[comboint] || componentIds[comboint] == -1)
				{
					auto pair = std::make_pair(String(it->second->GetName()), it->second);
					map2.insert(pair);
				}
			}


			for (auto& m : map2)
			{
				++i;
				ImGui::PushID(i);
				if (ImGui::Selectable(m.first.c_str()))
				{
					ImGui::TreeAdvanceToLabelPos();
					EditorCore::Pointer()->CreateActiveResourceWindow(m.second);
				}
				ImGui::PopID();
			}

			ImGui::TreePop();
		}

		//if (ImGui::Button("New Resource"))
		//{
		//	ImGUIEditor::LoadFile("Load Resource", "NewRes", ResVault::Pointer());
		//}
	}
}
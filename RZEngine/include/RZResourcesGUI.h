#pragma once

namespace rczEngine
{
	class RZ_EXP GUIResources
	{
	public:
		void RenderResources()
		{
			ImGui::Begin("Resources Management");
			{
				ImGui::Text("Resources Loaded");

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
							m_ActiveResource = m.second.get();
						}
						ImGui::PopID();
					}

					ImGui::TreePop();
				}

				if (ImGui::Button("New Resource"))
				{
					//GUIEditor::LoadFile("Load Resource", "NewRes", ResVault::Pointer());
				}

				
			}

			ImGui::Begin("Selected Resource");
			{
				if (m_ActiveResource)
				m_ActiveResource->RenderResourceGUI();
			}
		}
	
		Resource* m_ActiveResource = nullptr;
		ResourceType m_ActiveType;
	};
}
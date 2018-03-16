#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT GUIResources
	{
	public:
		void RenderResources()
		{
			ImGui::Begin("Resources Management");
			{
				ImGui::Text("Resources Loaded");

				UoMap<ResourceHandle, StrPtr<Resource>>* map = &ResVault::Pointer()->m_ResourceMap; 
				if (ImGui::TreeNode("Resource List"))
				{
					int i = 0;
					for (auto it = map->begin(); it != map->end(); ++it, ++i)
					{
						ImGui::PushID(i);
						if (ImGui::Selectable(it->second->m_Name.c_str()))
						{
							ImGui::TreeAdvanceToLabelPos();
							m_ActiveResource = it->second.get();
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

	};
}
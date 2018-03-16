#include <RZEnginePCH.h>

namespace rczEngine
{
	void GUISceneGraph::InitSceneGraphGUI()
	{
		m_SceneGameObjects.clear();
	}

	void GUISceneGraph::RenderSceneGraphGUI()
	{
		auto Scene = SceneManager::Pointer()->GetActiveScene().get();

		ImGui::Begin("Scene Management");
		{
			ImGui::Text("Current Scene");

			RenderChildren(Scene->m_RootNode);

			if (ImGui::Button("New GameObject"))
			{
				Scene->CreateActor("Nuevo", NULL);
			}

			if (ImGui::Button("Save Scene"))
			{
				
			}

			ImGui::SameLine();

			if (ImGui::Button("Delete GameObject"))
			{
				if (m_ActiveGameObject)
				{
					auto DestroyedID = m_ActiveGameObject->GetID();

					m_ActiveGameObject->GetParent().lock()->RemoveChild(DestroyedID);
					Scene->m_SceneActorMap.erase(DestroyedID);

					m_ActiveGameObject.reset();
				}
			}
		}
	}

	void GUISceneGraph::RenderChildren(StrGameObjectPtr obj)
	{
		/// Get the children list from the object
		auto childrens = obj->GetChildren();

		ImGui::PushID(obj->GetID());
		///If there are no children, make it a selectable and return.
		if (!childrens.size())
		{
			if (ImGui::Selectable(obj->GetName().c_str()))
			{
				ImGui::TreeAdvanceToLabelPos();
				m_ActiveGameObject = obj;
			}

			ImGui::PopID();
			return;
		}

		///If there are children, call RenderChildren on all of them.
		if (ImGui::TreeNode(obj->GetName().c_str()))
		{
			if (ImGui::IsItemClicked())
			{
				m_ActiveGameObject = obj;
			}

			for (int i = 0; i < childrens.size(); ++i)
			{
				RenderChildren(childrens[i].lock());
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}
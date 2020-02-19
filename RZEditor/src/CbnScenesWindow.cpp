#include "RZEditorPCH.h"

namespace rczEngine
{
	void ScenesWindow::Render()
	{
		auto sceneManager = SceneManager::Pointer();
		auto sceneToDraw = SceneManager::Pointer()->GetActiveScene().get();

		if (!sceneManager)
		{
			ImGui::Text("No Scene Manager Created");
			return;
		};

		ImGui::Text("Current Scene: ");
		ImGui::SameLine();
		ImGui::Text(m_SceneFileName.c_str());

		ImGui::Text("Number of actors: %d", m_SceneGameObjects.size());

		RenderChildActors(sceneToDraw->m_RootNode);

		if (ImGui::Button("New GameObject"))
		{
			sceneToDraw->CreateActor("Nuevo", NULL);
		}

		if (ImGui::Button("Delete GameObject"))
		{
			if (m_ActiveGameObject)
			{
				auto DestroyedID = m_ActiveGameObject->GetID();

				m_ActiveGameObject->Destroy(true);

				m_ActiveGameObject.reset();
			}
		}
	}

	void ScenesWindow::RenderChildActors(StrGameObjectPtr obj)
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
				EditorCore::Pointer()->SetActiveGameObject(obj);
			}

			ImGui::PopID();
			return;
		}

		///If there are children, call RenderChildren on all of them.
		if (ImGui::TreeNode(obj->GetName().c_str()))
		{
			if (ImGui::IsItemClicked())
			{
				EditorCore::Pointer()->SetActiveGameObject(obj);
			}

			for (int i = 0; i < childrens.size(); ++i)
			{
				RenderChildActors(childrens[i].lock());
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}
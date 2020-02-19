#include "RZEditorPCH.h"

namespace rczEngine
{
	void GameObjectWindow::Render()
	{
		auto gameObject = EditorCore::Pointer()->GetActiveGameObject();

		if (gameObject.expired())
		{
			ImGui::Text("No actor selected.");
			return;
		}

		auto m_ActiveObject = gameObject.lock();

		auto windowSize = ImVec2(ImGui::GetWindowSize());

		if (ImGui::DragFloat3("Position", (float*)&m_ActiveObject->m_Position))
		{
			m_ActiveObject->m_DirtyLocalMatrix = true;
		}

		if (ImGui::DragFloat3("Orientation", (float*)&m_ActiveObject->m_Orientation))
		{
			m_ActiveObject->m_DirtyLocalMatrix = true;
		}

		if (ImGui::DragFloat3("Scale", (float*)&m_ActiveObject->m_Scale))
		{
			m_ActiveObject->m_DirtyLocalMatrix = true;
		}

		static int comboint = 0;
		static bool AddComponentMenu = false;
		static int componentIds[5] = { 3, 4, 5, 6, 9 };

		if (ImGui::Button("AddComponent"))
		{
			AddComponentMenu = true;
		}

		if (AddComponentMenu)
		{
			ImGui::Begin("PopUp");
			{
				ImGui::Combo("Components", &comboint, "Model Renderer \0Skinned Model Renderer\0Light \0Move \0CameraWalk");
				if (ImGui::Button("Add"))
				{
					SceneManager::Pointer()->GetActiveScene()->CreateComponent((eComponentID)componentIds[comboint], m_ActiveObject->GetID());
					AddComponentMenu = false;
					return;
				}
			}
		}

		m_ActiveObject->RenderComponents();
	}
}
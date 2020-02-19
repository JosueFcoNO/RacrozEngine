#include <RZEnginePCH.h>

namespace rczEngine
{
	void GUIGameObject::InitWindow()
	{
		///TODO
	}

	void GUIGameObject::RenderWindow()
	{
		ImGui::Begin("GameObject");
		{
			if (ImGui::InputText("Name", m_Name, 50, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_ActiveObject->SetName(m_Name);
			}

			if (m_ActiveObject)
			{

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
	}

	void GUIGameObject::SetNewGameObject(StrGameObjectPtr obj)
	{
		strcpy_s(m_Name, obj->GetName().c_str());
		m_ActiveObject = obj;
	}


}
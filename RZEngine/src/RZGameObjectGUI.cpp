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
			if (ImGui::InputText("Name", m_Name, 20, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
			{
				m_ActiveObject->SetName(m_Name);
			}

			if (m_ActiveObject)
			{
				Vector3 vec3 = m_ActiveObject->GetPosition();

				if (ImGui::InputFloat3("Position", (float*)&vec3))
				{
					m_ActiveObject->SetPosition(vec3);
				}

				vec3 = m_ActiveObject->GetOrientation();

				if (ImGui::InputFloat3("Orientation", (float*)&vec3))
				{
					m_ActiveObject->SetOrientation(vec3);
				}

				vec3 = m_ActiveObject->GetScale();

				if (ImGui::InputFloat3("Scale", (float*)&vec3))
				{
					m_ActiveObject->SetScale(vec3);
				}

				static int comboint = 0;
				static bool AddComponentMenu = false;
				static int componentIds[3] = { 3, 4, 6 };

				if (ImGui::Button("AddComponent"))
				{
					AddComponentMenu = true;
				}

				if (AddComponentMenu)
				{
					ImGui::Begin("PopUp");
					{
						ImGui::Combo("Components", &comboint, "Model Renderer \0Skinned Model Renderer \0Move");
						if (ImGui::Button("Add"))
						{
							m_ActiveObject->AddComponent((eCOMPONENT_ID)componentIds[comboint]).lock()->Init();
							AddComponentMenu = false;
						}
					}
				}

				m_ActiveObject->RenderComponents();
			}
		}
	}

	void GUIGameObject::SetNewGameObject(StrGameObjectPtr obj)
	{
		strcpy(m_Name, obj->GetName().c_str());
		m_ActiveObject = obj;
	}


}
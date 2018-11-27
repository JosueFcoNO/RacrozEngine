#include <RZEnginePCH.h>

namespace rczEngine
{
	void GUISceneGraph::InitSceneGraphGUI()
	{
		m_SceneGameObjects.clear();

		m_SceneFileName = "Default Scene";
	}

	String GetScenePath(const char * instruction)
	{
		ANSICHAR filename[MAX_PATH];

		OPENFILENAME ofn;
		ZeroMemory(filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = "Any File\0*.rcz\0";
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = instruction;
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA(&ofn))
		{
			return filename;
		}
		else
		{
			return "";
		}
	}

	void GUISceneGraph::RenderSceneGraphGUI()
	{
		auto Scene = SceneManager::Pointer()->GetActiveScene().get();

		ImGui::Begin("Scene Management");
		{
			ImGui::Text("Current Scene");

			ImGui::Text(m_SceneFileName.c_str());

			RenderChildren(Scene->m_RootNode);

			if (ImGui::Button("New GameObject"))
			{
				Scene->CreateActor("Nuevo", NULL);
			}

			if (ImGui::Button("Save Scene"))
			{
				//Parse the string to see if it has the .rcz extension.
				auto parsedString = Parser::ParseToStrings<CHAR>(m_SceneFileName, String("."), 0);

				//If it does not. Add it.
				if (parsedString[parsedString.size() - 1] != "rcz")
				{
					m_SceneFileName.append(".rcz");
				}
				//Save the scene.
				SceneManager::Pointer()->SaveScene(m_SceneFileName.c_str());
			}

			if (ImGui::Button("Load Scene"))
			{
				auto tempString = GetScenePath("Choose Scene");

				if (tempString != "")
				{
					m_SceneFileName = tempString;
					SceneManager::Pointer()->LoadScene(m_SceneFileName.c_str());
				}
			}

			ImGui::SameLine();

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
#include <RZEnginePCH.h>

namespace rczEngine
{
	////////////////////
	////SCENE GRAPH/////
	////////////////////
	
	void Editor_SceneGraph::InitSceneGraphEditor(Scene * sceneGraph, Editor * editor)
	{
		m_ShowPos.m_x = 1620;
		m_ShowPos.m_y = 0;

		m_HidePos.m_x = 1620+300;
		m_HidePos.m_y = 0;

		m_Scene = sceneGraph;
		m_Editor = editor;

		UI_Canvas* canvas = editor->GetCanvas();
		m_SceneGraphBox = canvas->CreateUI_Element("SceneGraphBox", editor->GetUIResource(ERES_SCENEGRAPH), 1620, 0, 3, 300, 1080);
		canvas->m_Root.AddChildren(m_SceneGraphBox);

		TextSceneGraph = canvas->CreateUI_Text(L"Scene Graph", "TextSceneGraph", 60, 5, 24, 30);
		m_SceneGraphBox.lock()->AddChildren(TextSceneGraph);

		Editor_MaxButtonAction* buttonMax = new Editor_MaxButtonAction;
		Editor_MinButtonAction* buttonMin = new Editor_MinButtonAction;

		buttonMax->m_Element = this;
		buttonMin->m_Element = this;

		m_ButtonMin = canvas->CreateUI_Button("ButtonMaxSceneGraph", editor->GetUIResource(ERES_MIN_IDLE), editor->GetUIResource(ERES_MIN_HOVER), editor->GetUIResource(ERES_MIN_PRESSED), buttonMin, -33, 0, 2, 33, 34);
		m_SceneGraphBox.lock()->AddChildren(m_ButtonMin);

		m_ButtonMax = canvas->CreateUI_Button("ButtonMinSceneGraph", editor->GetUIResource(ERES_MAX_IDLE), editor->GetUIResource(ERES_MAX_HOVER), editor->GetUIResource(ERES_MAX_PRESSED), buttonMax, -33, 2000, 2, 33, 34);
		m_SceneGraphBox.lock()->AddChildren(m_ButtonMax);
	}

	void Editor_SceneGraph::UpdateScene()
	{
		int32 i = 0;
		for (auto it = m_Scene->m_SceneActorMap.begin(); it != m_Scene->m_SceneActorMap.end(); ++it, ++i)
		{
			auto ptr = new Editor_GameObject;
			//ptr->InitEditorGameObject(m_Editor, it->second);
			m_GameObjects.push_back(ptr);

			m_SceneGraphBox.lock()->AddChildren(ptr->m_Button);

			ptr->SetPos(8, 47 + 27 * i);
		}
	}
};
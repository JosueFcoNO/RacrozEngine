#include <RZEnginePCH.h>

namespace rczEngine
{
	void Editor_GameObjectProps::InitGameObjectProps(Scene * sceneGraph, Editor * editor)
	{
		m_ShowPos.m_x = 0;
		m_ShowPos.m_y = 1080 - 592;

		m_HidePos.m_x = 0;
		m_HidePos.m_y = 1080 - 140;

		m_Scene = sceneGraph;
		m_Editor = editor;

		UI_Canvas* canvas = editor->GetCanvas();
		m_GameObjectPropBox = canvas->CreateUI_Element("PlanetConfig", editor->GetUIResource(ERES_GAMEOBJECT_PROPS), 0, 1080 - 592, 4, 423, 490);
		canvas->m_Root.AddChildren(m_GameObjectPropBox);

		m_TextGameObjectProps = canvas->CreateUI_Text(L"Planet", "GameObjectTextBox", 20, 2, 24, 24);
		m_GameObjectPropBox.lock()->AddChildren(m_TextGameObjectProps);

		Editor_MaxButtonAction* buttonMax = new Editor_MaxButtonAction;
		Editor_MinButtonAction* buttonMin = new Editor_MinButtonAction;

		buttonMax->m_Element = this;
		buttonMin->m_Element = this;

		int32 Min = editor->GetUIResource(ERES_MIN_IDLE);
		int32 Max = editor->GetUIResource(ERES_MAX_IDLE);

		m_ButtonMin = canvas->CreateUI_Button("ButtonMaxGameObject", Min, Min+1, Min+2, buttonMin, 235, -5, 2, 33, 34);
		m_TextGameObjectProps.lock()->AddChildren(m_ButtonMin);

		m_ButtonMax = canvas->CreateUI_Button("ButtonMinGameObject", Max, Max+1, Max+2, buttonMax, 235, 2000, 2, 33, 34);
		m_TextGameObjectProps.lock()->AddChildren(m_ButtonMax);


	}

	void Editor_GameObjectProps::ChangedObject()
	{
		UI_Text* ptr = (UI_Text*)m_TextGameObjectSelected.lock().get();
		ptr->SetText(TextTool::AnsiToUni(&m_Editor->m_ActiveGameObject.lock()->GetName()[0]));
		
	}

	void Editor_GameObjectProps::UpdatePoint(Vector2 polar)
	{
		m_Point.lock()->SetLocalPosition(polar.m_x / 80.0f * 256 - 5, polar.m_y / 40.f * 128 - 5);

	}

	void Editor_GameObjectProps::InitPlanet(SpaceManager * space)
	{
		auto canvas = m_Editor->GetCanvas();

		Octaves = new Editor_VarChanger<int>();
		Octaves->InitVarChanger(&space->Octaves, "Octave", 100, 70, 1, 15, 1, m_GameObjectPropBox, m_Editor);

		Scale = new Editor_VarChanger<int>();
		Scale->InitVarChanger(&space->Scale, "Scale", 100, 210, 1, 15, 1, m_GameObjectPropBox, m_Editor);

		VScale = new Editor_VarChanger<int>();
		VScale->InitVarChanger(&space->VerticalScale, "VertScale", 100, 280, 10, 100, 5, m_GameObjectPropBox, m_Editor);

		Persistance = new Editor_VarChanger<float>();
		Persistance->InitVarChanger(&space->Persistance, "Persistance", 100, 140, 0.1f, 1.0f, 0.1f, m_GameObjectPropBox, m_Editor);

		auto MipMap = new Editor_VarChanger<int>();
		MipMap->InitVarChanger(&space->MipMap, "MipMap", 100, 350, 0, 6, 1, m_GameObjectPropBox, m_Editor);

		GenButton = new GenerateHeightMap();
		GenButton->InitGenHeightMap(space);

		ResourceHandle ButtonText = m_Editor->GetUIResource(ERES_BTN1_IDLE);

		auto m_ButtonGenerator = canvas->CreateUI_Button("ButtonGenerate", ButtonText, ButtonText + 1, ButtonText + 2, GenButton, 100, 400, 2, 143, 34);
		m_TextGameObjectProps.lock()->AddChildren(m_ButtonGenerator);

		auto m_ButtonGeneratorText = canvas->CreateUI_Text(L"Generate", "GenText", 10, 10, 1, 16);
		m_ButtonGenerator.lock()->AddChildren(m_ButtonGeneratorText);

	}




}
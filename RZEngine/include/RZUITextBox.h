#pragma once

namespace rczEngine
{

	class RZ_UTILITY_EXPORT UI_TextBox : public UI_Element, public IMouseHandler, public IKeyHandler
	{
	public:
		UI_TextBox(UI_Id elementId = INVALID_ID, char* elementName = "UIText")
		{
			m_ElementId = elementId;
			m_ElementName = elementName;
		};

		void InitTextBox(String* strToLink, int8 maxChars, UI_Canvas* canvas, EventManager* evnt, ResourceHandle idle, ResourceHandle active) 
		{
			m_LinkedStr = strToLink;
			m_maxChars = maxChars;

			m_Text = canvas->CreateUI_Text(TextTool::AnsiToUni(strToLink->c_str()), "BoxText", 2, 2, 20, 20);
			AddChildren(m_Text);

			m_Idle = idle;
			m_Active = active;

			evnt->SubscribeToEvent((IKeyHandler*)this, EVENT_KEY_DOWN, KEY_RETURN);
			evnt->SubscribeToEvent((IMouseHandler*)this, EVENT_MOUSE_DOWN, MOUSE_LEFT);
			m_evnt = evnt;
		};

		void InitTextBoxFloat(float* varToLink, int8 maxChars, UI_Canvas* canvas, EventManager* evnt) {};
		void InitTextBoxInt(int* intToLink, int8 maxChars, UI_Canvas* canvas, EventManager* evnt) {};


		virtual void Update(float deltaTime, UI_Canvas * canvas)
		{
			if (m_Parent)
				m_AccumulatedPosition = m_Parent->GetAccumulatedPosition() + m_LocalPosition;
			else
				m_AccumulatedPosition = m_LocalPosition;

			UpdateVertices();
			UpdateChildren(deltaTime, canvas);

			if (m_Input)
			{
				m_Text.lock()->SetText(TextTool::AnsiToUni(m_evnt->GetKeyBuffer()));
			}
		}

		///////////////////////////////////////
		////////////KEY HANDLING///////////////
		///////////////////////////////////////

		virtual void OnKeyDown(int32 k)
		{
			if (k == KEY_RETURN)
			{
				m_Input = false;
			}
		};

		virtual void OnKeyPressed(int32 k)
		{

		};

		virtual void OnKeyUp(int32 k)
		{

		};


		///////////////////////////////////////
		//////////MOUSE HANDLING///////////////
		///////////////////////////////////////
		virtual void OnClick(int32 mb, int32 mx, int32 my)
		{
			///If the left mouse button was clicked, check for collision with mouse cursor.
			if (mb == MOUSE_LEFT)
			{
				m_Input = CheckMouseCollision(mx, my);

				if (m_Input)
				{
					m_evnt->SetKeyBuffer(m_Text.lock()->GetText());
					m_Texture = m_Active;
				}
				else
				{
					m_Texture = m_Idle;
				}
			}
		};

		virtual void OnHold(int32 mb, int32 mx, int32 my) {};
		virtual void OnReleased(int32 mb, int32 mx, int32 my) {};
		virtual void OnMoved(int32 mb, int32 mx, int32 my) {};

	private:
		WeakPtr<UI_Text> m_Text;

		ResourceHandle m_Idle;
		ResourceHandle m_Active;

		String* m_LinkedStr = NULL;
		int8 m_maxChars = NULL;

		bool m_Input = false;
		EventManager* m_evnt = NULL;
	};
}
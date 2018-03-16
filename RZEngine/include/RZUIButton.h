#pragma once
#include <RZEnginePCH.h>

namespace rczEngine
{
	///The ui_button states
	enum UI_BUTTON_STATE
	{
		BUTTON_IDLE,
		BUTTON_HOVER,
		BUTTON_CLICKED
	};

	///An interface to implement button different button actions.
	class RZ_UTILITY_EXPORT UI_Button_Action
	{
	public:
		///Do an action, gets called by the UI_Button on clicked
		virtual void DoAction() = 0;
	};

	///A UI_Button that implements the click functionality.
	class RZ_UTILITY_EXPORT UI_Button : public UI_Element, public IMouseHandler
	{
	public:
		UI_Button(UI_Id elementId = INVALID_ID, char* elementName = "UIButton")
		{
			m_ElementId = elementId;
			m_ElementName = elementName;
		};

		///Inits the button, it receives a UI_Button_Action derivate that implements the button's action and subscribes to the event manager
		void InitButton(UI_Button_Action* onClickAction, ResourceHandle onIdle, ResourceHandle onHover, ResourceHandle onClick, EventManager* evnt)
		{
			m_Action = onClickAction;
			m_TextureClick = onClick;
			m_TextureHover = onHover;
			m_TextureIdle = onIdle;

			evnt->SubscribeToEvent(this, EVENT_MOUSE_DOWN, MOUSE_LEFT);
			evnt->SubscribeToEvent(this, EVENT_MOUSE_UP, MOUSE_LEFT);
			evnt->SubscribeToEvent(this, EVENT_MOUSE_MOVED, -1);
		};

		///////////////////////////////////////
		//////////MOUSE HANDLING///////////////
		///////////////////////////////////////
		virtual void OnClick(int32 mb, int32 mx, int32 my)
		{
			///If the left mouse button was clicked, check for collision with mouse cursor.
			if (mb == MOUSE_LEFT)
			{
				if (m_ButtonState == BUTTON_HOVER)
				{
					m_Texture = m_TextureClick;
					m_ButtonState = BUTTON_CLICKED;
				}
			}
		};
		virtual void OnHold(int32 mb, int32 mx, int32 my) {};
		virtual void OnReleased(int32 mb, int32 mx, int32 my) 
		{
			if (m_ButtonState == BUTTON_CLICKED)
			{
				///If the mouse is inside the button's rectangle
				if (CheckMouseCollision(mx, my))
				{
					m_Texture = m_TextureHover;
					m_ButtonState = BUTTON_HOVER;

					///If there's a button action set, call it.
					if (m_Action)
					{
						m_Action->DoAction();
					}
				}
			}
		};
		virtual void OnMoved(int32 mb, int32 mx, int32 my)
		{
			///If the mouse is inside the button's rectangle
			if (CheckMouseCollision(mx, my))
			{
				if (m_ButtonState == BUTTON_IDLE)
				{
					m_Texture = m_TextureHover;
					m_ButtonState = BUTTON_HOVER;
				}
			}
			else
			{
				if (m_ButtonState == BUTTON_CLICKED || m_ButtonState == BUTTON_HOVER)
				{
					m_Texture = m_TextureIdle;
					m_ButtonState = BUTTON_IDLE;
				}
			}
		};

		ResourceHandle m_TextureIdle = INVALID_RESOURCE;
		ResourceHandle m_TextureHover = INVALID_RESOURCE;
		ResourceHandle m_TextureClick = INVALID_RESOURCE;

	private:

		UI_BUTTON_STATE m_ButtonState = BUTTON_IDLE;

		UI_Button_Action* m_Action = NULL;
	};

}
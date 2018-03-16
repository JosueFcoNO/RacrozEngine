#pragma once

namespace rczEngine
{
	template <class t>
	class RZ_UTILITY_EXPORT UI_Slider : public UI_Element, public IMouseHandler
	{
	public:
		UI_Slider(UI_Id elementId = INVALID_ID, char* elementName = "UISlider")
		{
			m_ElementId = elementId;
			m_ElementName = elementName;
		};

		///Inits Slider Specific Variables
		void InitSlider(t* variableToChange, t maxValue, t minValue, ResourceHandle pointTex, ResourceHandle slideText, ResourceHandle minTex, ResourceHandle maxTex, UI_Canvas* canvas, EventManager* evnt)
		{
			///Subscribe to the Event Manager
			evnt->SubscribeToEvent(this, EVENT_MOUSE_DOWN, MOUSE_LEFT);
			evnt->SubscribeToEvent(this, EVENT_MOUSE_PRESSED, MOUSE_LEFT);
			evnt->SubscribeToEvent(this, EVENT_MOUSE_UP, MOUSE_LEFT);
			evnt->SubscribeToEvent(this, EVENT_MOUSE_MOVED, MOUSE_LEFT);

			///Set the variable that this Slider is going to control.
			m_VariableMax = maxValue;
			m_VariableMin = minValue;
			m_VariableToChange = variableToChange;

			///Create the needed UI Elements in the canvas.

			///Create the Point Sliding in the Slider
			m_Point = canvas->CreateUI_Button("PointSlider", pointTex, pointTex+1, pointTex+2, NULL, 0,0, 1, 18,18);
			AddChildren(m_Point);

			m_MaxText = canvas->CreateUI_Text(TextTool::WIntTo(m_VariableMax), "MaxText", m_PixelSize.m_x, 0, 35, 12);
			AddChildren(m_MaxText);

			m_MinText = canvas->CreateUI_Text(TextTool::WIntTo(m_VariableMin), "MinText", -5, 0, 35, 12);
			AddChildren(m_MinText);

			m_PointText = canvas->CreateUI_Text(TextTool::WIntTo(*m_VariableToChange), "PointText", 0, -20, 35, 16);
			m_Point.lock()->AddChildren(m_PointText);
		};

		///Mouse Handling Interface
		virtual void OnClick(int32 mb, int32 mx, int32 my)
		{
			if (CheckMouseCollision(mx, my))
				m_Active = true;
		};

		virtual void OnHold(int32 mb, int32 mx, int32 my)
		{
			if (CheckMouseCollision(mx, m_AccumulatedPosition.m_y+1))
				if (m_Active)
			{
				float posMouseLocal = mx - m_AccumulatedPosition.m_x;
				posMouseLocal /= m_PixelSize.m_x;


				(*m_VariableToChange) = (m_VariableMax - m_VariableMin) * posMouseLocal;
				m_PointText.lock()->SetText(TextTool::WIntTo(*m_VariableToChange));

				m_Point.lock()->SetLocalPosition(posMouseLocal*m_PixelSize.m_x-10, -5);

				m_Active = true;
			}
		};

		virtual void OnReleased(int32 mb, int32 mx, int32 my) 
		{
				m_Active = false;
		};
		virtual void OnMoved(int32 mb, int32 mx, int32 my) {};

	private:
		t* m_VariableToChange = NULL;
		t m_VariableMin;
		t m_VariableMax;

		WeakPtr<UI_Element> m_Point;
		WeakPtr<UI_Text> m_PointText;
		WeakPtr<UI_Text> m_MaxText;
		WeakPtr<UI_Text> m_MinText;

		bool m_Active = false;
	};
}
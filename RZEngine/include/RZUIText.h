#pragma once

namespace rczEngine
{
	class UI_Text : public UI_Element
	{
	public:
		UI_Text(UI_Id elementId = INVALID_ID, char* elementName = "UIText")
		{
			m_ElementId = elementId;
			m_ElementName = elementName;
		};
		
		void InitText(UNICHAR* textString, float x, float y, float wPixels, float hPixels, Gfx::FontRender* uifont) 
		{
			Init(x, y, 0, wPixels, hPixels, INVALID_RESOURCE);
			SetText(textString);
			m_UIFont = uifont;
		};

		void Init(float x, float y, float depth, float wPixels, float hPixels, ResourceHandle texture)
		{
			m_Texture = texture;

			m_LocalPosition.m_x = x;
			m_LocalPosition.m_y = y;

			if (m_Parent)
				m_AccumulatedPosition = m_LocalPosition + m_Parent->GetAccumulatedPosition();
			else
				m_AccumulatedPosition = m_LocalPosition;

			m_PixelSize.m_x = wPixels;
			m_PixelSize.m_y = hPixels;
		};

		virtual void PreRender(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas) {};
		virtual void Render(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas) 
		{
			m_UIFont->DrawString(graphicsApi, &m_Text[0], m_PixelSize.m_y, m_AccumulatedPosition);

			RenderChildren(graphicsApi, canvas);
		};

		virtual void RenderChildren(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas)
		{
			for (int32 i = 0; i < m_Children.size(); ++i)
			{
				m_Children[i]->Render(graphicsApi, canvas);
			}
		};

		virtual void PostRender(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas) {};

		virtual void Update(float deltaTime, UI_Canvas* canvas)
		{
			if (m_Parent)
			{
				m_AccumulatedPosition.m_x = m_LocalPosition.m_x + m_Parent->GetAccumulatedPosition().m_x;
				m_AccumulatedPosition.m_y = m_LocalPosition.m_y + m_Parent->GetAccumulatedPosition().m_y;
			}
			else
				m_AccumulatedPosition = m_LocalPosition;

			UpdateChildren(deltaTime, canvas);
		};

		virtual void UpdateChildren(float deltaTime, UI_Canvas* canvas)
		{
			for (int32 i = 0; i < m_Children.size(); ++i)
			{
				m_Children[i]->Update(deltaTime, canvas);
			}
		};

		void SetText(UNICHAR* textString)
		{
			m_Text = textString;
		};

		char* GetText()
		{
			return TextTool::UniToAnsi(m_Text.c_str());
		};

	private:
		Gfx::FontRender* m_UIFont;
		BasicString<UNICHAR> m_Text;
	};
}




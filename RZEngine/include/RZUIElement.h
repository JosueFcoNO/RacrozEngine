#pragma once
#include <RZEnginePCH.h>

namespace rczEngine
{
	class UI_Canvas;
	class UI_Element;

	typedef Vector<StrPtr<UI_Element>> UIElementVector;
	typedef int32 UI_Id;

	struct UI_Vertex
	{
		UI_Vertex() : m_Uvs(INIT_ZERO), m_Position(INIT_ZERO)
		{};
		Vector3 m_Position;
		Vector2 m_Uvs;
	};

	class RZ_UTILITY_EXPORT UI_Element
	{
	public:
		UI_Element(UI_Id elementId = INVALID_ID, char* elementName = "UIElement") : m_AccumulatedPosition(INIT_ZERO), m_PixelSize(INIT_ZERO)
		{ 
			m_ElementId = elementId; 
			m_ElementName = elementName; 
		};
		
		virtual void Init(float x, float y, float depth, float wPixels, float hPixels, ResourceHandle texture);

		virtual void PreRender(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas);
		virtual void Render(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas);
		virtual void RenderChildren(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas);
		virtual void PostRender(Gfx::GfxCore* graphicsApi, UI_Canvas* canvas);

		virtual void Update(float deltaTime, UI_Canvas* canvas);
		virtual void UpdateChildren(float deltaTime, UI_Canvas* canvas);

		void AddChildren(WeakPtr<UI_Element> children);

		void SetAccumulatedPosition(float x, float y);
		void SetLocalPosition(float x, float y);

		void UpdateVertices();
		void Translate(Vector2 position);

		Vector2 GetAccumulatedPosition();
		Vector2 GetPixelSize();

		UI_Id GetId() { return m_ElementId; };

		StrPtr<UI_Element> m_Parent;
		UIElementVector m_Children;
		
		UI_Vertex m_Vertices[4];
		int32 m_IndexOffset = 0;

		ResourceHandle m_Texture = INVALID_RESOURCE;

	protected:
		bool CheckMouseCollision(int32 mx, int32 my)
		{
			///If the mouse is inside the button's rectangle
			if (m_AccumulatedPosition.m_x < mx && m_AccumulatedPosition.m_x + m_PixelSize.m_x > mx)
				if (m_AccumulatedPosition.m_y < my && m_AccumulatedPosition.m_y + m_PixelSize.m_y > my)
				{
					return true;
				}

			return false;
		};

		Vector2 m_AccumulatedPosition;
		Vector2 m_LocalPosition;

		Vector2 m_PixelSize;

		UI_Id m_ElementId;
		String m_ElementName;
	};
	
}
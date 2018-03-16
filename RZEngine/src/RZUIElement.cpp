#include <RZEnginePCH.h>

namespace rczEngine
{
	void UI_Element::Init(float x, float y, float depth, float wPixels, float hPixels, ResourceHandle texture)
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

		m_Vertices[0].m_Position.m_z = depth;
		m_Vertices[1].m_Position.m_z = depth;
		m_Vertices[2].m_Position.m_z = depth;
		m_Vertices[3].m_Position.m_z = depth;

		m_Vertices[0].m_Uvs.m_x = 0.0f;
		m_Vertices[0].m_Uvs.m_y = 1.0f;

		m_Vertices[1].m_Uvs.m_x = 0.0f;
		m_Vertices[1].m_Uvs.m_y = 0.0f;

		m_Vertices[2].m_Uvs.m_x = 1.0f;
		m_Vertices[2].m_Uvs.m_y = 1.0f;

		m_Vertices[3].m_Uvs.m_x = 1.0f;
		m_Vertices[3].m_Uvs.m_y = 0.0f;

		UpdateVertices();
	}

	void UI_Element::PreRender(Gfx::GfxCore * gfx, UI_Canvas* canvas)
	{
		if (m_Texture != INVALID_RESOURCE)
		canvas->m_res->GetResource<Texture2D>(m_Texture).lock()->SetThisTextureInPS(0, 1, gfx);
	}

	void UI_Element::Render(Gfx::GfxCore * gfx, UI_Canvas* canvas)
	{
		PreRender(gfx, canvas);

		gfx->DrawIndexed(6, m_IndexOffset, 0);
		RenderChildren(gfx, canvas);

		PostRender(gfx, canvas);
	}

	void UI_Element::RenderChildren(Gfx::GfxCore * graphicsApi, UI_Canvas* canvas)
	{
		for (int i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->Render(graphicsApi, canvas);
		}
	}

	void UI_Element::PostRender(Gfx::GfxCore * graphicsApi, UI_Canvas* canvas)
	{

	}

	void UI_Element::Update(float deltaTime, UI_Canvas * canvas)
	{
		if (m_Parent)
			m_AccumulatedPosition = m_Parent->GetAccumulatedPosition() + m_LocalPosition;
		else
			m_AccumulatedPosition = m_LocalPosition;

		UpdateVertices();
		UpdateChildren(deltaTime, canvas);
	}

	void UI_Element::UpdateChildren(float deltaTime, UI_Canvas * canvas)
	{
		for (int32 i = 0; i < m_Children.size(); ++i)
		{
			m_Children[i]->Update(deltaTime, canvas);
		}
	}

	void UI_Element::AddChildren(WeakPtr<UI_Element> children)
	{
		m_Children.push_back(children.lock());

		StrPtr<UI_Element> ptr(this);
		children.lock()->m_Parent = ptr;
	}

	void UI_Element::SetAccumulatedPosition(float x, float y)
	{
		m_AccumulatedPosition.Set(x, y);
		
		m_LocalPosition = m_Parent->GetAccumulatedPosition() - m_AccumulatedPosition;
	}

	void UI_Element::SetLocalPosition(float x, float y)
	{
		m_LocalPosition.Set(x, y);
	}

	void UI_Element::UpdateVertices()
	{
		m_Vertices[0].m_Position.m_x = m_AccumulatedPosition.m_x;
		m_Vertices[0].m_Position.m_y = m_AccumulatedPosition.m_y;

		m_Vertices[1].m_Position.m_x = m_AccumulatedPosition.m_x;
		m_Vertices[1].m_Position.m_y = m_AccumulatedPosition.m_y + (m_PixelSize.m_y);

		m_Vertices[2].m_Position.m_x = m_AccumulatedPosition.m_x + (m_PixelSize.m_x);
		m_Vertices[2].m_Position.m_y = m_AccumulatedPosition.m_y;

		m_Vertices[3].m_Position.m_x = m_AccumulatedPosition.m_x + (m_PixelSize.m_x);
		m_Vertices[3].m_Position.m_y = m_AccumulatedPosition.m_y + (m_PixelSize.m_y);
	}

	void UI_Element::Translate(Vector2 position)
	{
		m_AccumulatedPosition += position;
	}

	Vector2 UI_Element::GetAccumulatedPosition()
	{
		return m_AccumulatedPosition;
	}

	Vector2 UI_Element::GetPixelSize()
	{
		return m_PixelSize;
	}

}
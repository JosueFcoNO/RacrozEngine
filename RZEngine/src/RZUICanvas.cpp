#include <RZEnginePCH.h>

namespace rczEngine
{
	void UI_Canvas::Init(int32 canvasWidth, int32 canvasHeight, Gfx::GfxCore* gfx, ResourceManager* res, EventManager* evnt)
	{
		m_CanvasWidth = canvasWidth;
		m_CanvasHeight = canvasHeight;

		m_res = res;
		m_gfx = gfx;
		m_evnt = evnt;

		Vector4 ScreenSize;
		ScreenSize.m_x = canvasWidth;
		ScreenSize.m_y = canvasHeight;

		m_ScreenSizeBuffer.CreateConstantBuffer(sizeof(Vector4), Gfx::USAGE_DEFAULT, m_gfx);
		m_ScreenSizeBuffer.UpdateConstantBuffer(&ScreenSize, m_gfx);

		m_Root.Init(0,0, 0, canvasWidth, canvasHeight,INVALID_RESOURCE);

		m_UIFont.InitFont(L"Arial", gfx);

		m_bState.InitBlendState(true);
		m_bState.CreateBlendState(gfx);

		
	}

	void UI_Canvas::AddCanvasElement(WeakPtr<UI_Element> element)
	{
		Pair<UI_Id, StrPtr<UI_Element>> par(element.lock()->GetId(), element);
		m_CanvasElements.insert(par);
	}

	void UI_Canvas::DeleteCanvasElement(UI_Id elementToDelete)
	{
		m_CanvasElements.erase(elementToDelete);
	}



	WeakPtr<UI_Element> UI_Canvas::CreateUI_Element(char* name, ResourceHandle texture, int32 x, int32 y, int32 depth, int32 w, int32 h)
	{
		StrPtr<UI_Element> ptr;
		ptr.reset(new UI_Element(m_UIElementsIndex++, name));
		ptr->Init(x, y, depth, w, h, texture);
		AddCanvasElement(ptr);


		return ptr;
	}

	WeakPtr<UI_Text> UI_Canvas::CreateUI_Text(wchar_t* text, char* name, int32 x, int32 y, int32 w, int32 h)
	{
		StrPtr<UI_Text> ptr;
		ptr.reset(new UI_Text(m_UIElementsIndex++, name));
		ptr->InitText(text, x, y, w, h, &m_UIFont);
		AddCanvasElement(ptr);

		return ptr;
	}

	WeakPtr<UI_Button> UI_Canvas::CreateUI_Button(char * name, ResourceHandle textureOnIdle, ResourceHandle textureOnHover, ResourceHandle textureOnClicked, UI_Button_Action * action, int32 x, int32 y, int32 depth, int32 w, int32 h)
	{
		StrPtr<UI_Button> ptr;
		ptr.reset(new UI_Button(m_UIElementsIndex++, name));
		ptr->Init(x, y, depth, w, h, textureOnIdle);
		ptr->InitButton(action, textureOnIdle, textureOnHover, textureOnClicked, m_evnt);
		AddCanvasElement(ptr);


		return ptr;
	}

	WeakPtr<UI_TextBox> UI_Canvas::CreateUI_TextBox(String * strToLink, char * name, ResourceHandle texture, ResourceHandle textureOnInput, int32 x, int32 y, int32 depth, int32 w, int32 h)
	{
		StrPtr<UI_TextBox> ptr;
		ptr.reset(new UI_TextBox(m_UIElementsIndex++, name));
		ptr->Init(x, y, depth, w, h, texture);
		ptr->InitTextBox(strToLink, 15, this, this->m_evnt, texture, textureOnInput);
		AddCanvasElement(ptr);


		return ptr;
	}

	void UI_Canvas::PreRenderCanvas()
	{
		m_UIVertices.ClearVertexList();
		m_UIIndices.ClearIndexList();

		m_CurrentVertex = 0;
		m_CurrentIndex = 0;

		for (auto it = m_CanvasElements.begin(); it != m_CanvasElements.end(); ++it)
		{
			StrPtr<UI_Element> ptr = it->second;

			for (int32 i = 0; i < 4; ++i)
			{
				m_UIVertices.AddVertex(ptr->m_Vertices[i]);
			}

			m_UIIndices.AddIndex(m_CurrentVertex);
			m_UIIndices.AddIndex(m_CurrentVertex + 1);
			m_UIIndices.AddIndex(m_CurrentVertex + 2);

			m_UIIndices.AddIndex(m_CurrentVertex + 1);
			m_UIIndices.AddIndex(m_CurrentVertex + 3);
			m_UIIndices.AddIndex(m_CurrentVertex + 2);

			ptr->m_IndexOffset = m_CurrentIndex;

			m_CurrentVertex += 4;
			m_CurrentIndex += 6;
		}

		CreateCanvasBuffers();

		m_UIIndices.SetThisIndexBuffer(m_gfx);
		m_UIVertices.SetThisVertexBuffer(m_gfx);

		m_ScreenSizeBuffer.SetBufferInVS(0, m_gfx);

		m_bState.SetThisBlendState(m_gfx);
	}

	void UI_Canvas::RenderCanvas()
	{
		m_Root.RenderChildren(m_gfx, this);
	}

	void UI_Canvas::PostRenderCanvas()
	{
		m_gfx->SetBlendStateDefault();
	}

	void UI_Canvas::UpdateCanvas(float deltaTime)
	{
		m_Root.Update(deltaTime, this);
	}

	void UI_Canvas::CreateCanvasBuffers()
	{
		m_UIIndices.CreateIndexBuffer(Gfx::USAGE_DEFAULT, m_gfx);
		m_UIVertices.CreateVertexBuffer(Gfx::USAGE_DEFAULT, false, m_gfx);
	}

	void UI_Canvas::SetScreenWidth(int32 newWidth)
	{
		m_CanvasWidth = newWidth;
	}

	void UI_Canvas::SetScreenHeight(int32 newHeight)
	{
		m_CanvasHeight = newHeight;
	}

	int32 UI_Canvas::GetScreenWidth()
	{
		return m_CanvasWidth;
	}

	int32 UI_Canvas::GetScreenHeight()
	{
		return m_CanvasHeight;
	}

};
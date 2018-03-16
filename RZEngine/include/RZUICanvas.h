#pragma once

namespace rczEngine
{
	typedef UoMap<UI_Id, StrPtr<UI_Element>> UI_ElementList;

	class UI_Button_Action;
	class UI_Text;
	class UI_TextBox;
	class UI_Button;

	template <class t>
	class UI_Slider;

	class RZ_UTILITY_EXPORT UI_Canvas
	{
	public:
		~UI_Canvas() {};

		void Init(int32 canvasWidth, int32 canvasHeight, Gfx::GfxCore* gfx, ResourceManager* res, EventManager* evnt);
		void AddCanvasElement(WeakPtr<UI_Element> element);
		void DeleteCanvasElement(UI_Id elementToDelete);

		WeakPtr<UI_Element> CreateUI_Element(char* name, ResourceHandle texture, int32 x, int32 y, int32 depth, int32 w, int32 h);
		WeakPtr<UI_Text>    CreateUI_Text(wchar_t* text, char* name, int32 x, int32 y, int32 w, int32 h);
		WeakPtr<UI_Button>  CreateUI_Button(char* name, ResourceHandle textureOnIdle, ResourceHandle textureOnHover, ResourceHandle textureOnClicked, UI_Button_Action* action, int32 x, int32 y, int32 depth, int32 w, int32 h);
		WeakPtr<UI_TextBox> CreateUI_TextBox(String* strToLink, char* name, ResourceHandle texture, ResourceHandle textureOnInput, int32 x, int32 y, int32 depth, int32 w, int32 h);
		template <class t>
		WeakPtr<UI_Slider<t>> CreateUI_SliderInt(char* name, ResourceHandle texture, int32 x, int32 y, int32 depth, int32 w, int32 h) 
		{
			StrPtr<UI_Slider<t>> ptr;
			ptr.reset(new UI_Slider<t>(m_UIElementsIndex++, name));
			ptr->Init(x, y, depth, w, h, texture);
			AddCanvasElement(ptr);

			return ptr;
		}

		void PreRenderCanvas();
		void RenderCanvas();
		void PostRenderCanvas();

		void UpdateCanvas(float deltaTime);

		void CreateCanvasBuffers();

		void SetScreenWidth(int32 newWidth);
		void SetScreenHeight(int32 newHeight);
		int32 GetScreenWidth();
		int32 GetScreenHeight();

		UI_Element m_Root;
		Gfx::FontRender m_UIFont;

		ResourceManager* m_res = NULL;
		Gfx::GfxCore* m_gfx = NULL;
		EventManager* m_evnt = NULL;

	private:
		int32 m_UIElementsIndex = 0;

		Gfx::BlendState m_bState;

		UI_ElementList m_CanvasElements;

		Gfx::VertexBuffer<UI_Vertex> m_UIVertices;
		Gfx::IndexBuffer m_UIIndices;

		Gfx::ConstantBuffer m_ScreenSizeBuffer;

		int16 m_CurrentVertex = 0;
		int16 m_CurrentIndex = 0;
		
		int32 m_CanvasWidth;
		int32 m_CanvasHeight;
	};
};
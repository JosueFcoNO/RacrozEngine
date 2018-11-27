#pragma once

namespace rczEngine
{
	struct RendererConfig
	{
		float LightIntensity;
		float AmbientLightIntensity;
		float ReflectionIntensity;
		float pad0;

		bool Wireframe;
		bool pad1;
		bool pad2;
		bool pad3;

		int R;
		int G;
		int B;
	};

	struct ScreenChannel
	{
		int ChannelR;
		int ChannelG;
		int ChannelB;
		int ChannelA;
	};

	struct VERTEX_CONSTANT_BUFFER
	{
		float        mvp[4][4];
	};

	static ID3D11Buffer * g_pVertexConstantBuffer;

	static ID3D11Buffer* g_pVB;
	static ID3D11Buffer* g_pIB;

	static ID3D11ShaderResourceView* g_pFontTextureView;

	static ID3D11BlendState * g_pBlendState;
	static ID3D11DepthStencilState* g_pDepthStencilState;
	static ID3D11RasterizerState* g_pRasterizerState;
	static ID3D11SamplerState* g_pFontSampler;

	static Gfx::VertexShader* g_VertexShader;
	static Gfx::PixelShader* g_PixelShader;

#pragma region Static Functions

	static bool IsAnyMouseButtonDown();

	IMGUI_API LRESULT ImguiWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	

#pragma endregion

	class RZ_EXP GUIEditor
	{
	private:
		static GUIEditor*& _Instance();

	public:
		static void Start();
		static GUIEditor* Pointer();
		static void ShutDown();

		void InitEditor(PlatformData* platformData);
		void PreRender(ImDrawData* draw_data);
		void DestroyEditor();
		void RenderEditor(RendererConfig* user);
		
		void AddWindow(const String& name, GUIBaseWindow* window)
		{
			m_Windows[name] = window;
		}

		int ColorMode = 0;
		float Exposure = 1.0f;
		float FPS = 0.0f;

	private:
		void CreateFontsTexture();
		void InvalidateDeviceObjects();
		bool CreateDeviceObjects(rczEngine::Gfx::GfxCore* gfx);
		void NewFrame(Gfx::GfxCore* gfx);

		HWND m_Hwnd;

		int64 m_TicksPerSecond;
		int64 m_Time;

		Gfx::GfxCore* m_gfx;
		Gfx::ConstantBuffer m_ConstantBuffer;
		Gfx::BlendState m_NormalBlendState;
		Gfx::BlendState m_TextBlendState;

		GUISceneGraph m_SceneGraphGUI;
		GUIGameObject m_GameObjectGUI;
		GUIResources m_ResourcesGUI;
		GUIConsole m_ConsoleGUI;

		Map<String, GUIBaseWindow*> m_Windows;

		StrGameObjectPtr m_LastActiveObject;

	};
};
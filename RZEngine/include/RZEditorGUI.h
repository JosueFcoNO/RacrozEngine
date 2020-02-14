#pragma once

#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

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
	
#pragma region Static Functions


#pragma endregion

	class RZ_EXP ImGUIEditor
	{
	private:
		static ImGUIEditor*& _Instance();

	public:
		static void Start();
		static ImGUIEditor* Pointer();
		static void ShutDown();

		void InitEditor();
		void PreRender(ImDrawData* draw_data);
		void DestroyEditor();
		void RenderEditor(RendererConfig* user);

		void InitImgui();
		
		void AddWindow(const String& name, GUIBaseWindow* window)
		{
			m_Windows[name] = window;
		}

		int ColorMode = 0;
		float Exposure = 1.0f;
		float FPS = 0.0f;
		bool Wireframe = false;
		int SkinMode = 0;
		bool TransposeJoint = false;
		bool ByOffset = false;
		bool UseParents = false;
	private:
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
#pragma once

namespace rczEngine
{
	struct MouseData
	{
		float x;
		float y;
		long dx;
		long dy;
		byte buttons[4];
	};

	class RZ_EXP Input
	{
	private:
		static Input*& _Instance();

	public:
		static void Start();
		static Input* Pointer();
		static void ShutDown();

		~Input();
		bool InitInput();
		void EnumDevices();

		void RegainDevices();
		void ReleaseDevices();

		void Destroy();

		void GetKeyboard(byte* b);
		bool CheckKeyboardState(byte scan_code);
		MouseData GetMouseData();
		void UpdateMouseData();
		FORCEINLINE Vector2 GetMousePos() { return m_MousePos; };

		HINSTANCE hinstance = 0;
		HWND hwnd = 0;

	private:
		IDirectInput8A* InputPtr = NULL;
		LPDIRECTINPUTDEVICE8  m_KeyBoardDevice;
		LPDIRECTINPUTDEVICE8  m_MouseDevice;

		MouseData m_MouseData;
		Vector2 m_MousePos;
		POINT m_point;
	};
}
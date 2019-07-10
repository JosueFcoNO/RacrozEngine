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

	enum class InputEvent
	{
		None,
		Hold,
		Pressed,
		Released
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
		void Destroy();

		void RegainDevices();
		void ReleaseDevices();

		FORCEINLINE void UpdateInput() noexcept { UpdateKeyData(); UpdateMouseData(); };

		FORCEINLINE static bool GetKeyDown(byte key) noexcept { return (Input::Pointer()->m_KeyEvent[key] == InputEvent::Pressed); };
		FORCEINLINE static bool GetKeyUp(byte key) noexcept { return (Input::Pointer()->m_KeyEvent[key] == InputEvent::Released); };
		FORCEINLINE static bool GetKeyHold(byte key) noexcept { return (Input::Pointer()->m_KeyEvent[key] == InputEvent::Hold); };

		FORCEINLINE static Vector2 GetMousePos() noexcept { return Input::Pointer()->m_MousePos; };
		FORCEINLINE static MouseData GetMouseData() noexcept { return Input::Pointer()->m_MouseData; };
		
		HINSTANCE hinstance = nullptr;
		HWND hwnd = nullptr;

		byte lastPress = 0;

	private:
		void UpdateKeyData();
		void UpdateMouseData();

		void EnumDevices();

		IDirectInput8A* InputPtr = NULL;
		LPDIRECTINPUTDEVICE8  m_KeyBoardDevice = nullptr;
		LPDIRECTINPUTDEVICE8  m_MouseDevice = nullptr;

		std::array<byte, 256> m_LastKeyState;
		std::array<InputEvent, 256> m_KeyEvent;

		MouseData m_MouseData;
		Vector2 m_MousePos;
		POINT m_point;
	};
}
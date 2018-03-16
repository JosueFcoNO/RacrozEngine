#include <RZInputPCH.h>

namespace rczEngine
{
	Input*& Input::_Instance()
	{
		static Input* instance = nullptr;
		return instance;
	}

	void Input::Start()
	{
		(_Instance()) = new Input;
	}

	Input* Input::Pointer()
	{
		return _Instance();
	}

	void Input::ShutDown()
	{
		delete _Instance();
	}

	Input::~Input()
	{
		Destroy();
	}

	bool Input::InitInput()
	{
		HRESULT hr = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&InputPtr, NULL);

		hr = InputPtr->CreateDevice(GUID_SysKeyboard, &m_KeyBoardDevice, NULL);
		hr = InputPtr->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);

		m_KeyBoardDevice->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		m_MouseDevice->SetCooperativeLevel(hwnd,
			DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

		m_MouseDevice->SetDataFormat(&c_dfDIMouse);
		hr = m_KeyBoardDevice->SetDataFormat(&c_dfDIKeyboard);
		
		hr = m_KeyBoardDevice->Acquire();
		hr = m_MouseDevice->Acquire();

		DIPROPDWORD  dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = 10;
		hr = m_KeyBoardDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
		hr = m_MouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

		GetCursorPos(&m_point);
		ScreenToClient(hwnd, &m_point);

		m_MousePos.m_x = m_point.x;
		m_MousePos.m_y = m_point.y;

		if (S_OK(hr))
		{
			return true;
		}

		return false;
	}

	void Input::EnumDevices()
	{

	}
	
	void Input::RegainDevices()
	{
		m_KeyBoardDevice->Acquire();
		m_MouseDevice->Acquire();

		GetCursorPos(&m_point);
		ScreenToClient(hwnd, &m_point);

		m_MousePos.m_x = m_point.x;
		m_MousePos.m_y = m_point.y;
			
	}

	void Input::ReleaseDevices()
	{
		m_KeyBoardDevice->Unacquire();
		m_MouseDevice->Unacquire();
	}

	void Input::Destroy()
	{
		ReleaseDevices();
	}

	void Input::GetKeyboard(byte * b)
	{
		if (m_KeyBoardDevice->GetDeviceState(256, (LPVOID)b) == DI_OK)
		{
			return;
		};

		ZeroMemory(b, 256);
	}

	bool Input::CheckKeyboardState(byte scan_code)
	{
		byte Keyboard[256];
		if (m_KeyBoardDevice->GetDeviceState(256, Keyboard) == DI_OK)
		{
			if (Keyboard[scan_code] & 0x80)
			{
				return true;
			}
		}

		return false;
	}

	MouseData Input::GetMouseData()
	{
		return m_MouseData;
	}

	void Input::UpdateMouseData()
	{
		DIMOUSESTATE mousedata;

		m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mousedata);

		m_MouseData.dx = mousedata.lX;
		m_MouseData.dy = mousedata.lY;

		GetCursorPos(&m_point);
		ScreenToClient(hwnd, &m_point);

		m_MousePos.m_x = m_point.x;
		m_MousePos.m_y = m_point.y;

		m_MouseData.x = m_MousePos.m_x;
		m_MouseData.y = m_MousePos.m_y;

		m_MouseData.buttons[0] = mousedata.rgbButtons[0];
		m_MouseData.buttons[1] = mousedata.rgbButtons[1];
		m_MouseData.buttons[2] = mousedata.rgbButtons[2];
		m_MouseData.buttons[3] = mousedata.rgbButtons[3];
	}

	
}
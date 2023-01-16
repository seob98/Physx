#include "InputDevice.h"


InputDevice::InputDevice()
{
}

InputDevice::~InputDevice()
{
}

void InputDevice::WindowProcedure(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		SetKeyDown((unsigned char)Key::LeftMouse);
		break;
	case WM_RBUTTONDOWN:
		SetKeyDown((unsigned char)Key::RightMouse);
		break;
	case WM_MBUTTONDOWN:
		SetKeyDown((unsigned char)Key::WhileMouse);
		break;
	case WM_LBUTTONUP:
		SetKeyUp((unsigned char)Key::LeftMouse);
		break;
	case WM_RBUTTONUP:
		SetKeyUp((unsigned char)Key::RightMouse);
		break;
	case WM_MBUTTONUP:
		SetKeyUp((unsigned char)Key::WhileMouse);
		break;
	case WM_KEYDOWN:
		SetKeyDown((unsigned char)wParam);
		break;
	case WM_KEYUP:
		SetKeyUp((unsigned char)wParam);
		break;
	}
}

bool InputDevice::GetKey(const Key& key)
{
	return GetAsyncKeyState((int)key);
}

bool InputDevice::GetKeyDown(const Key& key)
{
	return m_downState[(unsigned char)key];
}

bool InputDevice::GetKeyUp(const Key& key)
{
	return m_upState[(unsigned char)key];
}

void InputDevice::SetKeyDown(unsigned char key)
{
	if (m_keyState[key])
		return;

	m_hasDown = true;

	m_downState[key] = true;

	m_keyState[key] = true;
}

void InputDevice::SetKeyUp(unsigned char key)
{
	if (!m_keyState[key])
		return;

	m_hasUp = true;

	m_upState[key] = true;

	m_keyState[key] = false;
}

void InputDevice::SetUsed()
{
	if (m_hasDown)
		ZeroMemory(m_downState, sizeof(m_downState));

	if (m_hasUp)
		ZeroMemory(m_upState, sizeof(m_upState));

	m_hasDown = false;

	m_hasUp = false;
}

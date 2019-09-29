#include "stdafx.h"
#include "KeyManager.h"

KeyManager::KeyManager()
{
	ZeroMemory(&m_KeyArr, sizeof(KEY_MAX) * STATE_END);
}

KeyManager::~KeyManager()
{
}

const bool KeyManager::GetKeyState(const KEYSTATE& KeyState, const int & VirtualKey)
{
	if (VirtualKey >= KEY_MAX ||
		KeyState >= STATE_END)
	{
		return false;
	}

	return m_KeyArr[KeyState][VirtualKey];
}

int KeyManager::UpdateKey()
{
	// Keyboard
	ComputeKeyState(VK_LEFT);
	ComputeKeyState(VK_RIGHT);
	ComputeKeyState(VK_UP);
	ComputeKeyState(VK_DOWN);
	
	// Mouse
	ComputeKeyState(VK_LBUTTON);
	
	return 0;
}

void KeyManager::ComputeKeyState(const int & VirtualKey)
{
	bool& KeyStateDown = m_KeyArr[STATE_DOWN][VirtualKey];
	bool& KeyStatePush = m_KeyArr[STATE_PUSH][VirtualKey];
	bool& KeyStateUp = m_KeyArr[STATE_UP][VirtualKey];

	SHORT KeyState = GetAsyncKeyState(VirtualKey);

	if (0 != GetAsyncKeyState(VirtualKey))
	{
		// 이전에 누른적이 없는 상태에서 눌린경우
		if (false == KeyStateDown &&
			false == KeyStatePush)
		{
			KeyStateDown = true;
			KeyStatePush = false;
			KeyStateUp = false;
		}
		// 이전에 누른적이 있는 상태에서 눌린경우
		else
		{
			KeyStateDown = false;
			KeyStatePush = true;
			KeyStateUp = false;
		}
	}
	else
	{
		// 이전에 누른적이 없는 상태에서 뗀경우
		if (true == KeyStateUp)
		{
			KeyStateDown = false;
			KeyStatePush = false;
			KeyStateUp = false;
		}
		// 이전에 누른적이 있는 상태에서 뗀경우
		else
		{
			KeyStateDown = false;
			KeyStatePush = false;
			KeyStateUp = true;
		}
	}
}

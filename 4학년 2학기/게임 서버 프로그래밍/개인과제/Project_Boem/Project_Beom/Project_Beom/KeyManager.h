#pragma once
class KeyManager : public Singleton<KeyManager>
{
public:
	KeyManager();
	virtual ~KeyManager();

public:
	const bool GetKeyState(const KEYSTATE& KeyState, const int& VirtualKey);

public:
	void SetRunning(const bool& check) { m_IsRunning = check; }

public:
	int UpdateKey();

private:
	void ComputeKeyState(const int& VirtualKey);

private:
	bool m_KeyArr[STATE_END][KEY_MAX];
	bool m_IsRunning = true;
};


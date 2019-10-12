#pragma once

class WinCallbackManager : public Singleton<WinCallbackManager>
{
public:
	WinCallbackManager();
	virtual ~WinCallbackManager();

public:
	const WPARAM& GetWParam() { return m_wParam; }
	const LPARAM& GetLParam() { return m_lParam; }
	const UINT& _GetMessage() { return m_Message; }

public:
	void SetWParam(const WPARAM& wParam) { m_wParam = wParam; }
	void SetLParam(const LPARAM& lParam) { m_lParam = lParam; }
	void SetMessage(const UINT& message) { m_Message = message; }

private:
	UINT m_Message = WM_NULL;
	WPARAM m_wParam = NULL;
	LPARAM m_lParam = NULL;
};
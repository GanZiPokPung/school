#pragma once

class GameObject;
class Player;
class Maingame
{
public:
	Maingame();
	~Maingame();

public:
	bool Initialize();
	int  Update(const float& TimeDelta);
	void Render();
	void Release();

private:
	HDC		m_hDC;

	GameObject *m_Player = nullptr;

	SceneManager *m_SceneMgr = nullptr;
};


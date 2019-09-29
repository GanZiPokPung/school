#include "stdafx.h"
#include "Maingame.h"
#include "GameObject.h"


Maingame::Maingame()
{
}


Maingame::~Maingame()
{
	Release();
}

bool Maingame::Initialize()
{
	// BackBuffer
	GET_MANAGER<BmpManager>()->LoadBmp(L"backbuffer", L"../Resources/backbuffer.bmp");
	GET_MANAGER<BmpManager>()->LoadBmp(L"back", L"../Resources/back.bmp");

	m_SceneMgr = GET_MANAGER<SceneManager>();
	
	if (false == m_SceneMgr->ChangeSceneState(SCENE_TESTMENU))
	{
		return false;
	}

	m_hDC = GetDC(g_hWnd);

	return true;
}

int Maingame::Update(const float & TimeDelta)
{
	if (m_SceneMgr)
	{
		if(-1 == m_SceneMgr->Update(TimeDelta))
			return -1;
	}

	return 0;
}

void Maingame::Render()
{
	// 더블 버퍼링
	// 후면 버퍼
	HDC BackBuffer = GET_MANAGER<BmpManager>()->FindBmp(L"backbuffer")->GetMemDC();

	// 잔상을 없애줄 비트맵
	HDC hMemDC = GET_MANAGER<BmpManager>()->FindBmp(L"back")->GetMemDC();

	BitBlt(BackBuffer, 0, 0, WINSIZE_X, WINSIZE_Y,
		hMemDC, 0, 0, SRCCOPY);

	bool isDoubleBuffer = false;
	// 후면 버퍼에 Render
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(BackBuffer);
	}

	// 후면 버퍼에 모든 오브젝트들의 이미지를 미리 그려넣은 다음
	// 출력 버퍼(전면버퍼)로 고속 복사한다.
	BitBlt(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y,
		BackBuffer, 0, 0, SRCCOPY);
}

void Maingame::Release()
{
	m_SceneMgr->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}

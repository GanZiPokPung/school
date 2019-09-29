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
	// ���� ���۸�
	// �ĸ� ����
	HDC BackBuffer = GET_MANAGER<BmpManager>()->FindBmp(L"backbuffer")->GetMemDC();

	// �ܻ��� ������ ��Ʈ��
	HDC hMemDC = GET_MANAGER<BmpManager>()->FindBmp(L"back")->GetMemDC();

	BitBlt(BackBuffer, 0, 0, WINSIZE_X, WINSIZE_Y,
		hMemDC, 0, 0, SRCCOPY);

	bool isDoubleBuffer = false;
	// �ĸ� ���ۿ� Render
	if (m_SceneMgr)
	{
		m_SceneMgr->Render(BackBuffer);
	}

	// �ĸ� ���ۿ� ��� ������Ʈ���� �̹����� �̸� �׷����� ����
	// ��� ����(�������)�� ��� �����Ѵ�.
	BitBlt(m_hDC, 0, 0, WINSIZE_X, WINSIZE_Y,
		BackBuffer, 0, 0, SRCCOPY);
}

void Maingame::Release()
{
	m_SceneMgr->DestroyInstance();

	ReleaseDC(g_hWnd, m_hDC);
}

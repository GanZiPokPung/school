#include "stdafx.h"
#include "TestMenuScene.h"

TestMenuScene::TestMenuScene()
{
}

TestMenuScene::~TestMenuScene()
{
	Release();
}

bool TestMenuScene::Initialize()
{
	ShowWindow(g_NewhWnd, SW_SHOW);
	UpdateWindow(g_NewhWnd);

	return true;
}

int TestMenuScene::Update(const float& TimeDelta)
{


	return 0;
}

void TestMenuScene::Render(HDC hDC)
{
	Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
}

void TestMenuScene::Release()
{
	ShowWindow(g_NewhWnd, SW_HIDE);
}

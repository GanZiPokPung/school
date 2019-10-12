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
	//WinCallbackManager* CallbackManager = GET_MANAGER<WinCallbackManager>();
	//
	//switch (CallbackManager->_GetMessage())
	//{
	//case WM_COMMAND:
	//{
	//	switch (CallbackManager->GetWParam())
	//	{
	//	case 2:
	//	{
	//		// IP 입력 창에 버튼을 누른 경우
	//		/*char szEdit[128] = { 0, };
	//		GetWindowTextA(hEdit, szEdit, 128);

	//		if (true == GET_MANAGER<NetworkManager>()->ConnectToServer(szEdit))
	//		{
	//			GET_MANAGER<SceneManager>()->ChangeSceneState(SCENE_TEST);
	//		}
	//		break;*/
	//	}
	//	}
	//}
	//}

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

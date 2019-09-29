#include "stdafx.h"
#include "TestScene.h"
#include "Player.h"
#include "Board.h"
#include "Mouse.h"

TestScene::TestScene()
{
}


TestScene::~TestScene()
{
	Release();
}

bool TestScene::Initialize()
{
	GET_MANAGER<BmpManager>()->LoadBmpBySceneState(SCENE_TEST);
	m_Player = AbstractFactory<Player>::CreateObj();
	m_ChessBoard = AbstractFactory<Board>::CreateObj();
	m_Mouse = AbstractFactory<Mouse>::CreateObj();

	
	GET_MANAGER<ChessManager>()->Initialize(m_ChessBoard->GetInfo().Pos_X, 
											m_ChessBoard->GetInfo().Pos_Y, 
											m_ChessBoard->GetInfo().Size_Width,
											m_ChessBoard->GetInfo().Size_Height);
	
	if (false == InitChessBoardIndex())
		return false;

	return true;
}

int TestScene::Update(const float & TimeDelta)
{
	if (-1 == UpdateChessInfo(TimeDelta))
		return -1;

	m_ChessBoard->Update(TimeDelta);
	m_Player->Update(TimeDelta);
	m_Mouse->Update(TimeDelta);

	for (const auto& dummy : m_DummyPlayer)
	{
		dummy->Update(TimeDelta);
	}

	return 0;
}

void TestScene::Render(HDC hDC)
{
	Rectangle(hDC, 0, 0, WINSIZE_X, WINSIZE_Y);
	m_ChessBoard->Render(hDC);
	m_Player->Render(hDC);
	m_Mouse->Render(hDC);

	for (const auto& dummy : m_DummyPlayer)
	{
		dummy->Render(hDC);
	}
}

void TestScene::Release()
{
	GET_MANAGER<ChessManager>()->DestroyInstance();

	if (m_ChessBoard)
	{
		delete m_ChessBoard;
		m_ChessBoard = nullptr;
	}

	if (m_Player)
	{
		delete m_Player;
		m_Player = nullptr;
	}

	if (m_Mouse)
	{
		delete m_Mouse;
		m_Mouse = nullptr;
	}

	for (auto& dummy : m_DummyPlayer)
	{
		if (dummy)
		{
			delete dummy;
			dummy = nullptr;
		}
	}
	m_DummyPlayer.clear();
}

bool TestScene::InitChessBoardIndex()
{
	NetworkManager* NetManager = GET_MANAGER<NetworkManager>();

	int InitIndexX = 1;
	int InitIndexY = 1;
	int PosX = 0;
	int PosY = 0;

	// 인덱스 위치를 모두 서버로 보내 이미 배치되어 있는 말은 배치를 해놓는다.
	for (int i = 1; i <= BOARDSIZE; ++i)
	{
		for (int j = 1; j <= BOARDSIZE; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(i, j))
			{
				MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
					L"Error", MB_OK);
				return false;
			}

			if ('X' == NetManager->GetInfoBuffer()[TYPE_CANMOVE])
			{
				// 이미 자리가 있으면 더미 말을 놓는다.
				GameObject* Object = AbstractFactory<Player>::CreateObj();
				GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY, i, j);
				Object->SetPosition(PosX, PosY);
				m_DummyPlayer.emplace_back(Object);
			}
		}
	}

	
	// 초기 말 위치를 서버로 보내 여분의 자리가 있는지 확인하여 배치한다.
	bool exitOuterLoop = false;
	for (int i = 1; i <= BOARDSIZE; ++i)
	{
		for (int j = 1; j <= BOARDSIZE; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(i, j))
			{
				MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
					L"Error", MB_OK);
				return false;
			}

			if ('O' == NetManager->GetInfoBuffer()[TYPE_CANMOVE])
			{
				// 여분의 자리를 저장하고 반복문 탈출.
				InitIndexX = (int)NetManager->GetInfoBuffer()[TYPE_INDEX_X];
				InitIndexY = (int)NetManager->GetInfoBuffer()[TYPE_INDEX_Y];
				exitOuterLoop = true;
				break;
			}
		}

		if (true == exitOuterLoop)
			break;
	}

	// exitOuterLoop가 false라는 뜻은 체스말이 현재 서버내의 모든 체스 인덱스에 존재했다는 것이다.
	if (false == exitOuterLoop)
	{
		MessageBox(g_hWnd, L"현재 서버에 말을 놓을 빈공간이 없습니다.\n",
			L"Error", MB_OK);
		return false;
	}

	// 인덱스에 맞는 보드 위치를 가져온다.
	GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY, InitIndexX, InitIndexY);
	m_Player->SetPosition(PosX, PosY);
	
	// 말을 놓았으므로 서버에 인덱스에 놓은것을 전송한다.
	if (-1 == NetManager->SendByAdjustMove(InitIndexX, InitIndexY))
	{
		MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
			L"Error", MB_OK);
		return false;
	}

	return true;
}

int TestScene::UpdateChessInfo(const float& TimeDelta)
{
	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	ChessManager* chessManager = GET_MANAGER<ChessManager>();
	NetworkManager* netManager = GET_MANAGER<NetworkManager>();

	if (false == m_IsProcess)
	{
		// 현재 플레이어의 체스 인덱스 위치를 가져온다.
		GAMEOBJINFO PlayerInfo = m_Player->GetInfo();
		int IndexPlayerX = 0;
		int IndexPlayerY = 0;
		chessManager->GetIndexByPos(&IndexPlayerX, &IndexPlayerY, PlayerInfo.Pos_X, PlayerInfo.Pos_Y);

		int isReceived = 0;

		// 상하좌우 키 입력을 받는다.
		if (true == keyManager->GetKeyState(STATE_DOWN, VK_UP))
		{
			isReceived = netManager->SendAndReceiveByInput(IndexPlayerX, IndexPlayerY, VK_UP);
		}
		else if (true == keyManager->GetKeyState(STATE_DOWN, VK_DOWN))
		{
			isReceived = netManager->SendAndReceiveByInput(IndexPlayerX, IndexPlayerY, VK_DOWN);
		}
		else if (true == keyManager->GetKeyState(STATE_DOWN, VK_LEFT))
		{
			isReceived = netManager->SendAndReceiveByInput(IndexPlayerX, IndexPlayerY, VK_LEFT);
		}
		else if (true == keyManager->GetKeyState(STATE_DOWN, VK_RIGHT))
		{
			isReceived = netManager->SendAndReceiveByInput(IndexPlayerX, IndexPlayerY, VK_RIGHT);
		}

		// 통신이 제대로 되었고 
		if (1 == isReceived)
		{
			// 서버에서 갈 수 있다고 보내왔다면
			if ('O' == netManager->GetInfoBuffer()[TYPE_CANMOVE])
			{
				m_IsProcess = true;
				int IndexX = (int)netManager->GetInfoBuffer()[TYPE_INDEX_X];
				int IndexY = (int)netManager->GetInfoBuffer()[TYPE_INDEX_Y];
				chessManager->GetPosByIndex(&m_DestX, &m_DestY, IndexX, IndexY);
				m_SrcX = PlayerInfo.Pos_X;
				m_SrcY = PlayerInfo.Pos_Y;
			}
		}
		else if(-1 == isReceived)
		{
			// 통신에 문제가 생겼다면
			MessageBox(g_hWnd, L"서버가 끊겼습니다. 다시 접속해주세요.", L"Error", MB_OK);
			return -1;
		}
	}
	else if (true == m_IsProcess)
	{
		// 보간을 이용하여 목적지 위치까지 이동
		if (m_Process > 1.f)
		{
			m_Process = 0.f;
			m_IsProcess = false;
			m_Player->SetPosition(m_DestX, m_DestY);
		}
		else
		{
			// 부드럽게 이동하도록 가속도를 주었음.. 
			if (0.5f > m_Process)
			{
				m_Acc = Lerp <float, float>(1.f, 10.f, m_Process);
			}
			else
			{
				m_Acc = Lerp<float, float>(10.f, 1.f, m_Process);
			}

			m_Process += TimeDelta * m_Acc;
			GAMEOBJINFO PlayerInfo = m_Player->GetInfo();
			m_Player->SetPosition(Lerp<int, int>(m_SrcX, m_DestX, m_Process),
				Lerp<int, int>(m_SrcY, m_DestY, m_Process));
		}
	}

	return 0;
}

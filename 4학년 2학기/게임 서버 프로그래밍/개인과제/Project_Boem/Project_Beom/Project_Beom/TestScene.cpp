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

	// �ε��� ��ġ�� ��� ������ ���� �̹� ��ġ�Ǿ� �ִ� ���� ��ġ�� �س��´�.
	for (int i = 1; i <= BOARDSIZE; ++i)
	{
		for (int j = 1; j <= BOARDSIZE; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(i, j))
			{
				MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
					L"Error", MB_OK);
				return false;
			}

			if ('X' == NetManager->GetInfoBuffer()[TYPE_CANMOVE])
			{
				// �̹� �ڸ��� ������ ���� ���� ���´�.
				GameObject* Object = AbstractFactory<Player>::CreateObj();
				GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY, i, j);
				Object->SetPosition(PosX, PosY);
				m_DummyPlayer.emplace_back(Object);
			}
		}
	}

	
	// �ʱ� �� ��ġ�� ������ ���� ������ �ڸ��� �ִ��� Ȯ���Ͽ� ��ġ�Ѵ�.
	bool exitOuterLoop = false;
	for (int i = 1; i <= BOARDSIZE; ++i)
	{
		for (int j = 1; j <= BOARDSIZE; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(i, j))
			{
				MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
					L"Error", MB_OK);
				return false;
			}

			if ('O' == NetManager->GetInfoBuffer()[TYPE_CANMOVE])
			{
				// ������ �ڸ��� �����ϰ� �ݺ��� Ż��.
				InitIndexX = (int)NetManager->GetInfoBuffer()[TYPE_INDEX_X];
				InitIndexY = (int)NetManager->GetInfoBuffer()[TYPE_INDEX_Y];
				exitOuterLoop = true;
				break;
			}
		}

		if (true == exitOuterLoop)
			break;
	}

	// exitOuterLoop�� false��� ���� ü������ ���� �������� ��� ü�� �ε����� �����ߴٴ� ���̴�.
	if (false == exitOuterLoop)
	{
		MessageBox(g_hWnd, L"���� ������ ���� ���� ������� �����ϴ�.\n",
			L"Error", MB_OK);
		return false;
	}

	// �ε����� �´� ���� ��ġ�� �����´�.
	GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY, InitIndexX, InitIndexY);
	m_Player->SetPosition(PosX, PosY);
	
	// ���� �������Ƿ� ������ �ε����� �������� �����Ѵ�.
	if (-1 == NetManager->SendByAdjustMove(InitIndexX, InitIndexY))
	{
		MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
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
		// ���� �÷��̾��� ü�� �ε��� ��ġ�� �����´�.
		GAMEOBJINFO PlayerInfo = m_Player->GetInfo();
		int IndexPlayerX = 0;
		int IndexPlayerY = 0;
		chessManager->GetIndexByPos(&IndexPlayerX, &IndexPlayerY, PlayerInfo.Pos_X, PlayerInfo.Pos_Y);

		int isReceived = 0;

		// �����¿� Ű �Է��� �޴´�.
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

		// ����� ����� �Ǿ��� 
		if (1 == isReceived)
		{
			// �������� �� �� �ִٰ� �����Դٸ�
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
			// ��ſ� ������ ����ٸ�
			MessageBox(g_hWnd, L"������ ������ϴ�. �ٽ� �������ּ���.", L"Error", MB_OK);
			return -1;
		}
	}
	else if (true == m_IsProcess)
	{
		// ������ �̿��Ͽ� ������ ��ġ���� �̵�
		if (m_Process > 1.f)
		{
			m_Process = 0.f;
			m_IsProcess = false;
			m_Player->SetPosition(m_DestX, m_DestY);
		}
		else
		{
			// �ε巴�� �̵��ϵ��� ���ӵ��� �־���.. 
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

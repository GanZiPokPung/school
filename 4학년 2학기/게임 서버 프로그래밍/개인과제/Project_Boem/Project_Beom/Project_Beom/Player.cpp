#include "stdafx.h"
#include "Player.h"


Player::Player()
	: GameObject()
{
}

Player::~Player()
{
	Release();
}

void Player::MoveToIndex(const int& indexX, const int& indexY)
{
	NetworkManager* netManager = GET_MANAGER<NetworkManager>();
	ChessManager* chessManager = GET_MANAGER<ChessManager>();
	GAMEOBJINFO PlayerInfo = GetInfo();
	int oldIndexX, oldIndexY;

	m_IsProcess = true;

	// ������ �ű� ó���� ���
	chessManager->GetIndexByPos(&oldIndexX, &oldIndexY, PlayerInfo.Pos_X, PlayerInfo.Pos_Y);
	//netManager->SendBySetIndex(m_ID, indexX, indexY, oldIndexX, oldIndexY);

	chessManager->GetPosByIndex(&m_DestX, &m_DestY, indexX, indexY);
	m_SrcX = PlayerInfo.Pos_X;
	m_SrcY = PlayerInfo.Pos_Y;
}

bool Player::InitOnce()
{
	NetworkManager* NetManager = GET_MANAGER<NetworkManager>();

	int InitIndexX = 1;
	int InitIndexY = 1;
	int PosX = 0;
	int PosY = 0;

	// �ʱ� �� ��ġ�� ������ ���� ������ �ڸ��� �ִ��� Ȯ���Ͽ� ��ġ�Ѵ�.
	bool exitOuterLoop = false;
	ClassPacket<MOVEINFO> recv_info;
	for (int i = 1; i <= BOARDSIZE_Y; ++i)
	{
		for (int j = 1; j <= BOARDSIZE_X; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(&recv_info, i, j))
			{
				MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
					L"Error", MB_OK);
				return false;
			}

			if ('O' == recv_info.GetInfo().canMove)
			{
				// ������ �ڸ��� �����ϰ� �ݺ��� Ż��.
				InitIndexX = recv_info.GetMustInfo().indexX;
				InitIndexY = recv_info.GetMustInfo().indexY;
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
	SetPosition(PosX, PosY);

	// ������ ���� �÷��̾��� ���̵� �޾ƿ´�.
	if (-1 == NetManager->SendAndReceiveByID(&recv_info))
	{
		MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
			L"Error", MB_OK);
		return false;
	}
	SetIDInfo(recv_info.GetMustInfo().id);


	// ���� �������Ƿ� ������ �ε����� �������� �����Ѵ�.
	if (-1 == NetManager->SendByAdjustMove(recv_info.GetMustInfo().id, InitIndexX, InitIndexY))
	{
		MessageBox(g_hWnd, L"������ ���� ������ �޴��߿� ����������ϴ�.\n �ٽ� �����Ͽ� �ּ���.",
			L"Error", MB_OK);
		return false;
	}


	return true;
}

int Player::Update_Input(const float& TimeDelta)
{
	if (true == m_IsProcess || false == m_IsPlayeable)
		return 1;

	m_IsReceived = 0;

	KeyManager* keyManager = GET_MANAGER<KeyManager>();
	ChessManager* chessManager = GET_MANAGER<ChessManager>();
	NetworkManager* netManager = GET_MANAGER<NetworkManager>();

	// ���� �÷��̾��� ü�� �ε��� ��ġ�� �����´�.
	GAMEOBJINFO PlayerInfo = GetInfo();
	int IndexPlayerX = 0;
	int IndexPlayerY = 0;
	chessManager->GetIndexByPos(&IndexPlayerX, &IndexPlayerY, PlayerInfo.Pos_X, PlayerInfo.Pos_Y);

	// �����¿� Ű �Է��� �޴´�.
	if (true == keyManager->GetKeyState(STATE_DOWN, VK_UP))
	{
		m_IsReceived = netManager->SendAndReceiveByInput(&m_recv_info_input, m_ID, IndexPlayerX, IndexPlayerY, VK_UP);
	}
	else if (true == keyManager->GetKeyState(STATE_DOWN, VK_DOWN))
	{
		m_IsReceived = netManager->SendAndReceiveByInput(&m_recv_info_input, m_ID, IndexPlayerX, IndexPlayerY, VK_DOWN);
	}
	else if (true == keyManager->GetKeyState(STATE_DOWN, VK_LEFT))
	{
		m_IsReceived = netManager->SendAndReceiveByInput(&m_recv_info_input, m_ID, IndexPlayerX, IndexPlayerY, VK_LEFT);
	}
	else if (true == keyManager->GetKeyState(STATE_DOWN, VK_RIGHT))
	{
		m_IsReceived = netManager->SendAndReceiveByInput(&m_recv_info_input, m_ID, IndexPlayerX, IndexPlayerY, VK_RIGHT);
	}

	return 0;
}

int Player::Update_Position(const float& TimeDelta)
{
	ChessManager* chessManager = GET_MANAGER<ChessManager>();
	NetworkManager* netManager = GET_MANAGER<NetworkManager>();

	if (false == m_IsProcess)
	{
		GAMEOBJINFO PlayerInfo = GetInfo();

		// ����� ����� �Ǿ��� 
		if (1 == m_IsReceived)
		{
			// �������� �� �� �ִٰ� �����Դٸ�
			if ('O' == m_recv_info_input.GetInfo().canMove)
			{
				m_IsProcess = true;
				int IndexX = m_recv_info_input.GetMustInfo().indexX;
				int IndexY = m_recv_info_input.GetMustInfo().indexY;
				chessManager->GetPosByIndex(&m_DestX, &m_DestY, IndexX, IndexY);
				m_SrcX = PlayerInfo.Pos_X;
				m_SrcY = PlayerInfo.Pos_Y;
			}
		}
		else if (-1 == m_IsReceived)
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
			SetPosition(m_DestX, m_DestY);
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
			GAMEOBJINFO PlayerInfo = GetInfo();
			SetPosition(Lerp<int, int>(m_SrcX, m_DestX, m_Process),
				Lerp<int, int>(m_SrcY, m_DestY, m_Process));
		}
	}

	return 0;
}

bool Player::Initialize()
{
	m_Info = GAMEOBJINFO{ 512, 512, 100, 100 };

	return true;
}

int Player::Update(const float & TimeDelta)
{
	if (-1 == Update_Input(TimeDelta))
	{
		return -1;
	}

	if (-1 == Update_Position(TimeDelta))
	{
		return -1;
	}

	if (-1 == GameObject::Update(TimeDelta))
	{
		return -1;
	}

	return 0;
}

void Player::Render(HDC hdc)
{
	HDC hMemDC = GET_MANAGER<BmpManager>()->FindBmp(L"horse")->GetMemDC();

	TransparentBlt(hdc, m_Rect.left, m_Rect.top, m_Info.Size_Width, m_Info.Size_Height
		, hMemDC, 0, 0, m_Info.Size_Width, m_Info.Size_Height, RGB(255, 255, 255));

	//Rectangle(hdc, m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.bottom);
}

void Player::Release()
{
}

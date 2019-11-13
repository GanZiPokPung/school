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

	// 서버에 옮긴 처리를 등록
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

	// 초기 말 위치를 서버로 보내 여분의 자리가 있는지 확인하여 배치한다.
	bool exitOuterLoop = false;
	ClassPacket<MOVEINFO> recv_info;
	for (int i = 1; i <= BOARDSIZE_Y; ++i)
	{
		for (int j = 1; j <= BOARDSIZE_X; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(&recv_info, i, j))
			{
				MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
					L"Error", MB_OK);
				return false;
			}

			if ('O' == recv_info.GetInfo().canMove)
			{
				// 여분의 자리를 저장하고 반복문 탈출.
				InitIndexX = recv_info.GetMustInfo().indexX;
				InitIndexY = recv_info.GetMustInfo().indexY;
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
	SetPosition(PosX, PosY);

	// 서버로 부터 플레이어의 아이디를 받아온다.
	if (-1 == NetManager->SendAndReceiveByID(&recv_info))
	{
		MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
			L"Error", MB_OK);
		return false;
	}
	SetIDInfo(recv_info.GetMustInfo().id);


	// 말을 놓았으므로 서버에 인덱스에 놓은것을 전송한다.
	if (-1 == NetManager->SendByAdjustMove(recv_info.GetMustInfo().id, InitIndexX, InitIndexY))
	{
		MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
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

	// 현재 플레이어의 체스 인덱스 위치를 가져온다.
	GAMEOBJINFO PlayerInfo = GetInfo();
	int IndexPlayerX = 0;
	int IndexPlayerY = 0;
	chessManager->GetIndexByPos(&IndexPlayerX, &IndexPlayerY, PlayerInfo.Pos_X, PlayerInfo.Pos_Y);

	// 상하좌우 키 입력을 받는다.
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

		// 통신이 제대로 되었고 
		if (1 == m_IsReceived)
		{
			// 서버에서 갈 수 있다고 보내왔다면
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
			SetPosition(m_DestX, m_DestY);
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
			GAMEOBJINFO PlayerInfo = GetInfo();
			SetPosition(Lerp<int, int>(m_SrcX, m_DestX, m_Process),
				Lerp<int, int>(m_SrcY, m_DestY, m_Process));
		}
	}

	return 0;
}

bool Player::Initialize()
{
	m_Info = GAMEOBJINFO{ 0, 0, 70, 70 };

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

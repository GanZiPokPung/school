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
	m_Mouse = AbstractFactory<Mouse>::CreateObj();
	m_ChessBoard = AbstractFactory<Board>::CreateObj(WINSIZE_X / 2, WINSIZE_Y / 2 - WINSIZE_Y / 30);
	
	GET_MANAGER<ChessManager>()->Initialize(m_ChessBoard->GetInfo().Pos_X,
		m_ChessBoard->GetInfo().Pos_Y,
		m_ChessBoard->GetInfo().Size_Width,
		m_ChessBoard->GetInfo().Size_Height);

	if (false == InitChessBoardIndex())
		return false;
	

	m_Player = AbstractFactory<Player>::CreateObj();
	if(false == dynamic_cast<Player*>(m_Player)->InitOnce()) 
		return false;

	// Dummy와는 다르게 Player는 Key를 입력받을 수 있어야 한다.
	dynamic_cast<Player*>(m_Player)->SetPlayeable(true);

	return true;
}

int TestScene::Update(const float & TimeDelta)
{
	if (-1 == UpdateChessInfo(TimeDelta))
		return -1;

	int Result = 0;

	Result = m_ChessBoard->Update(TimeDelta);
	if (-1 == Result)
		return -1;
	
	Result = m_Player->Update(TimeDelta);
	if (-1 == Result)
	
		return -1;
	Result = m_Mouse->Update(TimeDelta);
	if (-1 == Result)
		return -1;

	for (const auto& dummy : m_DummyPlayer)
	{
		Result = dummy->Update(TimeDelta);
		
		if (-1 == Result)
			return -1;
	}

	return Result;
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
	ClassPacket<MOVEINFO> recv_info;
	for (int i = 1; i <= BOARDSIZE; ++i)
	{
		for (int j = 1; j <= BOARDSIZE; ++j)
		{
			if (-1 == NetManager->SendAndReceiveByCanMove(&recv_info, i, j))
			{
				MessageBox(g_hWnd, L"서버로 부터 정보를 받는중에 문제생겼습니다.\n 다시 접속하여 주세요.",
					L"Error", MB_OK);
				return false;
			}

			if ('X' == recv_info.GetInfo().canMove)
			{
				// 이미 자리가 있으면 더미 말을 놓는다.
				GameObject* Object = AbstractFactory<Player>::CreateObj();
				GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY, i, j);
				Object->SetPosition(PosX, PosY);
				dynamic_cast<Player*>(Object)->SetIDInfo(recv_info.GetInfo().others_id);
				m_DummyPlayer.emplace_back(Object);
			}
		}
	}

	return true;
}

int TestScene::UpdateChessInfo(const float& TimeDelta)
{
	ClassPacket<OTHERINFO> recv_info;
	NetworkManager* NetManager = GET_MANAGER<NetworkManager>();
	int Result = NetManager->AlwaysSendAndReceiveByServerInfo(&recv_info);
	if (CHANGE_INIT == Result)
	{
		int PosX, PosY;
		// 새로 들어온 말인 경우 
		GameObject* Object = AbstractFactory<Player>::CreateObj();
		dynamic_cast<Player*>(Object)->SetIDInfo(recv_info.GetInfo().others_id);
		GET_MANAGER<ChessManager>()->GetPosByIndex(&PosX, &PosY,
			recv_info.GetInfo().others_indexX, recv_info.GetInfo().others_indexY);
		Object->SetPosition(PosX, PosY);
		m_DummyPlayer.emplace_back(Object);
	}
	else if(CHANGE_MOVE == Result)
	{
		for (auto& dummy : m_DummyPlayer)
		{
			if (recv_info.GetInfo().others_id == dynamic_cast<Player*>(dummy)->GetIDInfo())
			{
				dynamic_cast<Player*>(dummy)->MoveToIndex(
					recv_info.GetInfo().others_indexX, recv_info.GetInfo().others_indexY);
				break;
			}
		}
	}
	else if (CHANGE_EXIT == Result)
	{
		vector<GameObject*>::iterator iter_begin = m_DummyPlayer.begin();
		vector<GameObject*>::iterator iter_end = m_DummyPlayer.end();

		vector<GameObject*>::reverse_iterator iter_last = m_DummyPlayer.rbegin();

		for (auto& iter = iter_begin; iter != iter_end; ++iter)
		{
			if (recv_info.GetInfo().others_id == dynamic_cast<Player*>(*iter)->GetIDInfo())
			{
				SAFE_DELETE<GameObject>(*iter);
				iter_swap(iter, iter_last);
				m_DummyPlayer.pop_back();
				break;
			}
		}
	}

	return 0;
}

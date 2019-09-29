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

int Player::Update_Input(const float& TimeDelta)
{
	/*m_Dir = 0;
	KeyManager *keyManager = GET_MANAGER<KeyManager>();

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_LEFT))
	{
		m_Dir |= 0x00000001;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_RIGHT))
	{
		m_Dir |= 0x00000002;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_UP))
	{
		m_Dir |= 0x00000004;
	}

	if (true == keyManager->GetKeyState(STATE_PUSH, VK_DOWN))
	{
		m_Dir |= 0x00000008;
	}*/

	return 0;
}

int Player::Update_Position(const float& TimeDelta, const DIRECTION & Direction)
{
	//// L
	//if (0x00000001 == (m_Dir & 0x00000001))
	//{
	//	m_Info.Pos_X -= int(500.f * TimeDelta);
	//}

	//// R
	//if (0x00000002 == (m_Dir & 0x00000002))
	//{
	//	m_Info.Pos_X += int(500.f * TimeDelta);
	//}

	//// U
	//if (0x00000004 == (m_Dir & 0x00000004))
	//{
	//	m_Info.Pos_Y -= int(500.f * TimeDelta);
	//}

	//// D
	//if (0x00000008 == (m_Dir & 0x00000008))
	//{
	//	m_Info.Pos_Y += int(500.f * TimeDelta);
	//}

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

	if (-1 == Update_Position(TimeDelta, m_Direction))
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

#pragma once
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

public:
	const GAMEOBJINFO& GetInfo() { return m_Info; }
	const RECT& GetRect()		 { return m_Rect; }
	const bool& GetState()		 { return m_isDead; }
	const float& GetAngle()		 { return m_Angle; }

public:
	void SetPosition(int posX, int posY);
	void SetSize(int sizeWidth, int sizeHeight);
	void SetAngle(float angle);

public:
	virtual bool Initialize() PURE;
	virtual int  Update(const float& TimeDelta);
	virtual void Render(HDC hdc) PURE;
	virtual void Release() PURE;

protected:
	GAMEOBJINFO		m_Info;
	RECT			m_Rect;

	bool			m_isDead = false;
	float			m_Angle = 0.f;
};


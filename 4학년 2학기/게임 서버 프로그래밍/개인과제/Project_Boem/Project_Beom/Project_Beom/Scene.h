#pragma once
class Scene
{
public:
	Scene();
	virtual ~Scene();

public:
	virtual bool Initialize() PURE;
	virtual int Update(const float& TimeDelta) PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
};


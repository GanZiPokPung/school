#pragma once

#include "Bitmap.h"

class BmpManager : public Singleton<BmpManager>
{
public:
	BmpManager();
	virtual ~BmpManager();

public:
	Bitmap* FindBmp(const TCHAR* pKey);

public:
	void LoadBmpBySceneState(SCENESTATE SceneState);
	void LoadBmp(const TCHAR* pKey, const TCHAR* pFilePath);
	void ResetContainer();

public:
	unordered_map<const TCHAR*, Bitmap*>& GetBmpContainer() { return m_mapBmp; }

private:
	unordered_map<const TCHAR*, Bitmap*>				m_mapBmp;
	typedef unordered_map<const TCHAR*, Bitmap*>		MAPBITMAP;
};


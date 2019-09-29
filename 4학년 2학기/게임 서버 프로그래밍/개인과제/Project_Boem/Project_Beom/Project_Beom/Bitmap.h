#pragma once
class Bitmap
{
public:
	Bitmap();
	virtual ~Bitmap();

public:
	HDC GetMemDC() { return m_hMemDC; }

public:
	Bitmap* LoadBmp(const TCHAR* pFilePath);

private:
	HDC		m_hDC;
	HDC		m_hMemDC;

	HBITMAP m_Bitmap;
	HBITMAP m_OldBmp;
};


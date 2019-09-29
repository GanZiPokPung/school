#include "stdafx.h"
#include "Frame.h"


Frame::Frame()
{
}


Frame::~Frame()
{
}

bool Frame::Initialize(const float & CallCount)
{
	m_TimePerSec = 1.f / CallCount;

	return true;
}

bool Frame::Permit_Call(const float & TimeDelta)
{
	m_TimeAcc += TimeDelta;

	if (m_TimePerSec <= m_TimeAcc)
	{
		m_FrameTimeDelta = m_TimeAcc;
		m_TimeAcc = 0.f;
		return true;
	}

	return false;
}

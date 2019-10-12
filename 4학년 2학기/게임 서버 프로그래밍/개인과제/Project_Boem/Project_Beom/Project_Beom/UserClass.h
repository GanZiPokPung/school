#pragma once

// 보낼 클래스 패킷 형식

template<typename T>
class ClassPacket
{
public:
	ClassPacket()
	{
		ZeroMemory(&m_info, sizeof(m_info));
		ZeroMemory(&m_mustInfo, sizeof(MUSTINFO));
	}
	virtual ~ClassPacket()
	{

	}
public:
	MUSTINFO& GetMustInfo() { return m_mustInfo; }
	T& GetInfo() { return m_info; }

public:
	void MakePacketToData(char* buf) { memcpy(this, buf, sizeof(ClassPacket)); }
	void MakeDataToPacket(char* buf) { memcpy(buf, this, sizeof(ClassPacket)); }

private:
	MUSTINFO m_mustInfo;
	// T 형식은 보낼 데이터의 타입마다 다르다.
	T m_info;
};
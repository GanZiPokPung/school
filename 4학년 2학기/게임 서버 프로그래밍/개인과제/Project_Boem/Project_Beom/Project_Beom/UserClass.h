#pragma once

// ���� Ŭ���� ��Ŷ ����

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
	// T ������ ���� �������� Ÿ�Ը��� �ٸ���.
	T m_info;
};
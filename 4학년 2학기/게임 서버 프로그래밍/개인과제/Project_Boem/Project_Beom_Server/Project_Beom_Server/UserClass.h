#pragma once

//template<typename T>
//class ClassPacket
//{
//public:
//	ClassPacket()
//	{
//		ZeroMemory(&m_info, sizeof(m_info));
//		ZeroMemory(&m_mustInfo, sizeof(MUSTINFO));
//	}
//	ClassPacket(MUSTINFO must)
//	{
//		ZeroMemory(&m_info, sizeof(m_info));
//		ZeroMemory(&m_mustInfo, sizeof(MUSTINFO));
//
//		m_mustInfo.netType = must.netType;
//		m_mustInfo.id = must.id;
//		m_mustInfo.indexX = must.indexX;
//		m_mustInfo.indexY = must.indexY;
//
//		Alloc_Info();
//	}
//	virtual ~ClassPacket()
//	{
//		if (m_info)
//		{
//			delete m_info;
//			m_info = nullptr;
//		}
//	}
//public:
//	MUSTINFO& GetMustInfo() { return m_mustInfo; }
//	void* GetInfo() { return m_info; }
//private:
//	void Alloc_Info() 
//	{
//		switch (m_mustInfo.netType)
//		{
//		case NETTYPE_CANMOVE:
//		case NETTYPE_ADJMOVE: m_info = new MOVEINFO; break;
//		case NETTYPE_SETINDEX: m_info = new INDEXINFO; break;
//		case NETTYPE_INPUT: m_info = new INPUTINFO; break;
//		case NETTYPE_OTHER: m_info = new OTHERINFO; break;
//		}
//	}
//private:
//	void*	 m_info = nullptr;
//	MUSTINFO m_mustInfo;
//};

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
	T m_info;
};
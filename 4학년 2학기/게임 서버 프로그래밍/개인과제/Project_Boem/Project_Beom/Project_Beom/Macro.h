#pragma once

// 싱글톤
template<class ClassName> 
class Singleton
{
protected:
	Singleton() {}
	virtual ~Singleton() {}
public:
	static ClassName * GetInstance()
	{
		if (nullptr == m_pInstance)
		{
			m_pInstance = new ClassName;
		}
		return m_pInstance;
	}
	void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	static ClassName* m_pInstance;
};

template<class ClassName> 
ClassName* Singleton<ClassName>::m_pInstance = nullptr;

template<class ClassName>
ClassName* GET_MANAGER()
{
	return Singleton<ClassName>::GetInstance();
}

// 보간
template<typename T1, typename T2>
const T1 Lerp(const T2& start, const T2& end, float process)
{
	return T1((1.f - process) * start + process * end);
}

// 안전한 동적할당 해제
template<typename T>
void SAFE_DELETE(T* p)
{
	if (p)
	{
		delete p;
		p = nullptr;
	}
}

// 안전한 동적할당 해제 (배열)
template<typename T>
void SAFE_DELETE_ARRAY(T* p)
{
	if (p)
	{
		delete[] p;
		p = nullptr;
	}
}
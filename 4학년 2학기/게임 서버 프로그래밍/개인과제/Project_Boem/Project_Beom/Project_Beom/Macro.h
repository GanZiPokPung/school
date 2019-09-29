#pragma once

// ΩÃ±€≈Ê
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

// ∫∏∞£
template<typename T1, typename T2>
const T1 Lerp(const T2& start, const T2& end, float process)
{
	return T1((1.f - process) * start + process * end);
}
#pragma once

#define PURE = 0

/* Singleton Pattern */
//#define DECLARE_SINGLETON(ClassName)	\
//public:									\
//	static ClassName* GetInstance()		\
//	{									\
//		if(NULL == m_pInstance)			\
//			m_pInstance = new ClassName;\
//		return m_pInstance;				\
//	}									\
//	void DestroyInstance()				\
//	{									\
//		if(m_pInstance)					\
//		{								\
//			delete m_pInstance;			\
//			m_pInstance = NULL;			\
//		}								\
//	}									\
//private:								\
//	static ClassName* m_pInstance;
//
//#define IMPLEMENT_SINGLETON(ClassName)	\
//ClassName* ClassName::m_pInstance = NULL;
//
//#define GET_SINGLETON(ClassName) ClassName::GetInstance()

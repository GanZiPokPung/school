#include "stdafx.h"
#include "SceneManager.h"
#include "TestScene.h"
#include "TestMenuScene.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Release();
}

bool SceneManager::ChangeSceneState(SCENESTATE SceneState)
{
	if (nullptr != m_Scene)
	{
		Release();
	}

	switch (SceneState)
	{
	case SCENE_TESTMENU:
		m_Scene = new TestMenuScene;
		break;
	case SCENE_TEST:
		m_Scene = new TestScene;
		break;
	}

	if (false == m_Scene->Initialize())
	{
		Release();
		return false;
	}

	m_CurrentScene = SceneState;

	return true;
}

int SceneManager::Update(const float & TimeDelta)
{
	if (-1 == m_Scene->Update(TimeDelta))
		return -1;

	return 0;
}

void SceneManager::Render(HDC hDC)
{
	m_Scene->Render(hDC);
}

void SceneManager::Release()
{
	if (m_Scene)
	{
		delete m_Scene;
		m_Scene = nullptr;
	}
}

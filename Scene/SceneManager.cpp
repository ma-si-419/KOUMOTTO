#include "SceneManager.h"
#include "SceneTitle.h"

SceneManager::SceneManager() :
	m_isEnd(false)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
}

void SceneManager::Update(MyEngine::Input input)
{
	m_pScene->Update(input);
}

void SceneManager::Draw()
{
	m_pScene->Draw();
}
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> next)
{
	if (m_pScene)
	{
		m_pScene->End();
	}
	m_pScene = next;
	m_pScene->Init();
}

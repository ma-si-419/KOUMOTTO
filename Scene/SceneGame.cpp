#include "SceneGame.h"
#include "SceneTitle.h"
#include "Physics.h"
#include "Player.h"

SceneGame::SceneGame(SceneManager& sceneManager) : SceneBase(sceneManager)
{
	m_pPhysics =  new Physics();
	m_pPlayer = std::make_shared<Player>();
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_pPlayer->Init(m_pPhysics);
}

void SceneGame::Update(MyEngine::Input input)
{
	m_pPlayer->Update(input);
	m_pPhysics->Update();
	
	if (input.IsTrigger("CANCEL"))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager));
		return;
	}
}

void SceneGame::Draw()
{
	m_pPlayer->Draw();
	DrawString(0, 0, "SceneGame", GetColor(255, 255, 255));
}

void SceneGame::End()
{
}
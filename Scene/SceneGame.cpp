#include "Game.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "Physics.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"

SceneGame::SceneGame(SceneManager& sceneManager) : SceneBase(sceneManager)
{
	m_pPhysics =  new Physics();
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pEnemy = std::make_shared<Enemy>();
}

SceneGame::~SceneGame()
{
	delete m_pPhysics;
	m_pPhysics = nullptr;
}

void SceneGame::Init()
{
	m_pPlayer->Init(m_pPhysics);
	m_pEnemy->Init(m_pPhysics);
}

void SceneGame::Update(MyEngine::Input input)
{
	m_pPlayer->Update(shared_from_this(), input);
	m_pEnemy->Update(shared_from_this());
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->Update();
	m_pPhysics->Update();
	
	if (input.IsTrigger(Game::InputId::kPause))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager));
		return;
	}
}

void SceneGame::Draw()
{
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	DrawString(0, 0, "SceneGame", GetColor(255, 255, 255));
}

void SceneGame::End()
{
}
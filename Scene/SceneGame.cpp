#include "Game.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "Physics.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "AttackBase.h"

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
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->Init();
}

void SceneGame::Update(MyEngine::Input input)
{
	m_pPlayer->Update(shared_from_this(), input);
	m_pEnemy->Update(shared_from_this());
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->Update();
	

	//std::vector<int> deleteAttackNum;
	//int count = 0;
	for (auto& attack : m_pAttack)
	{
		attack->Update();
		////ˆ—‚ð‚µ‚È‚¢UŒ‚‚¾‚Á‚½‚ç
		//if (!attack->GetExist())
		//{
		//	deleteAttackNum.push_back(count);
		//}
		//count++;
	}
	//for (auto& item : deleteAttackNum)
	//{
	//	//m_pAttack[item] = nullptr;
	//}
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
	m_pPhysics->DebugDraw();
	DrawString(0, 0, "SceneGame", GetColor(255, 255, 255));
}

void SceneGame::End()
{
}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttack.push_back(pAttack);
}

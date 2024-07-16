#include "Game.h"
#include "SceneGame.h"
#include "SceneSelect.h"
#include "Physics.h"
#include "Player.h"
#include "Camera.h"
#include "Enemy.h"
#include "AttackBase.h"
#include "LoadCsv.h"

SceneGame::SceneGame(SceneManager& sceneManager) : SceneBase(sceneManager)
{
	m_pPhysics = std::make_shared<Physics>();
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pEnemy = std::make_shared<Enemy>();

	m_pAttack.reserve(400);
}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	m_pPlayer->Init(m_pPhysics);
	m_pEnemy->Init(m_pPhysics);
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->Init();

	//外部データ
	LoadCsv data;

	//必殺技のデータをロードする
	data.LoadAttackData();
	//プレイヤーに必殺技のデータを入れる
	m_pPlayer->SetAttackData(data.GetAttackData());
	//エネミーに必殺技のデータを入れる
	m_pEnemy->SetAttackData(data.GetAttackData());
}

void SceneGame::Update(MyEngine::Input input)
{
	m_pPhysics->Update();
	m_pPlayer->Update(shared_from_this(), input);
	m_pEnemy->Update(shared_from_this());
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	m_pCamera->Update();


	//std::vector<int> deleteAttackNum;
	int count = 0;
	for (auto& attack : m_pAttack)
	{
		attack->Update(m_pEnemy->GetPos());
		//処理をしない攻撃だったら
		if (!attack->GetIsExist())
		{
			attack->Final(m_pPhysics);
		}
		count++;
	}
	DrawFormatString(0, 400, GetColor(255, 255, 255), "攻撃ポインタ数:%d", count);

	for (int i = 0; i < m_pAttack.size(); i++)
	{
		if (!m_pAttack[i]->GetIsExist())
		{
			m_pAttack.erase(m_pAttack.begin() + i);
			i--;
		}
	}
	//m_pAttack.shrink_to_fit();
	printfDx("size:%d,capacity%d\n", m_pAttack.size(),m_pAttack.capacity());

	//auto res = std::remove_if(m_pAttack.begin(), m_pAttack.end(),
	//	[](const auto& attack)
	//	{
	//		return !attack->GetIsExist();
	//	});
	//m_pAttack.erase(res, m_pAttack.end());
	//for (auto& item : deleteAttackNum)
	//{
	//	//m_pAttack[item] = nullptr;
	//}



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

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
	handle = MV1LoadModel("data/model/Dome.mv1");
	//当たり判定管理クラスのポインタ
	m_pPhysics = std::make_shared<Physics>();
	//プレイヤーのポインタ
	m_pPlayer = std::make_shared<Player>();
	//カメラのポインタ
	m_pCamera = std::make_shared<Camera>();
	//エネミーのポインタ
	m_pEnemy = std::make_shared<Enemy>();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	MV1SetPosition(handle, VGet(0, 0, 0));
	MV1SetScale(handle, VGet(500, 500, 500));


	//プレイヤーの初期化(当たり判定を登録する)
	m_pPlayer->Init(m_pPhysics);
	//エネミーの初期化(当たり判定を登録する)
	m_pEnemy->Init(m_pPhysics);
	//カメラにプレイヤーの座標を渡す
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	//カメラの初期化
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
	//当たり判定の更新
	m_pPhysics->Update();
	//プレイヤーの更新
	m_pPlayer->Update(shared_from_this(), input);
	//エネミーの更新
	m_pEnemy->Update(shared_from_this());
	//プレイヤーにエネミーの座標を渡す
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//カメラにプレイヤーの座標を渡す
	m_pCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pCamera->SetTargetPos(m_pEnemy->GetPos());
	//カメラにプレイヤーが敵を中心にどのくらい回転しているかを渡す
	m_pCamera->SetPlayerRota(m_pPlayer->GetRota());
	//カメラの更新
	m_pCamera->Update();

	for (auto& attack : m_pAttacks)
	{
		//攻撃の更新
		attack->Update(m_pEnemy->GetPos());
		//処理をしない攻撃だったら
		if (!attack->GetIsExist())
		{
			//当たり判定を消す
			attack->Final(m_pPhysics);
		}
	}

	for (int i = 0; i < m_pAttacks.size(); i++)
	{
		//攻撃が消えていたら
		if (!m_pAttacks[i]->GetIsExist())
		{
			//配列から消す
			m_pAttacks.erase(m_pAttacks.begin() + i);
			i--;
		}
	}
	printfDx("size:%d,capacity%d\n", m_pAttacks.size(),m_pAttacks.capacity());

	if (input.IsTrigger(Game::InputId::kPause))
	{
		m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager));
		return;
	}
}

void SceneGame::Draw()
{
	MV1DrawModel(handle);

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
	m_pAttacks.push_back(pAttack);
}

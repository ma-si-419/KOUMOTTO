#include "Game.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "Physics.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Ui.h"
#include "AttackBase.h"
#include "LoadCsv.h"
#include "EffekseerForDXLib.h"

namespace
{
	//ゲームオーバー時の選択肢の数
	constexpr int kGameOverItemNum = 1;
	//バトル開始までの時間
	constexpr int kStandByTime = 120;
}

SceneGame::SceneGame(SceneManager& sceneManager, DataManager& dataManager) :
	SceneBase(sceneManager, dataManager),
	m_isGameOver(false),
	m_gameOverSelectItemNum(0),
	m_isStartBattle(false),
	m_standByTime(0)
{
	handle = MV1LoadModel("data/model/Dome.mv1");
	//当たり判定管理クラスのポインタ
	m_pPhysics = std::make_shared<Physics>();
	//プレイヤーのポインタ
	m_pPlayer = std::make_shared<Player>();
	//カメラのポインタ
	m_pGameCamera = std::make_shared<GameCamera>();
	//エネミーのポインタ
	m_pEnemy = std::make_shared<Enemy>();
	//Uiのポインタ
	m_pUi = std::make_shared<Ui>();

	m_dataManager.LoadAnimationFile();
	m_dataManager.LoadEffekseerHandle();

	m_pPlayer->SetAnimationData(m_dataManager.GetAnimationData(), true);
	m_pEnemy->SetAnimationData(m_dataManager.GetAnimationData(), false);

	m_pPlayer->SetEffekseerHandle(m_dataManager.GetEffekseerHandle());
	m_pEnemy->SetEffekseerHandle(m_dataManager.GetEffekseerHandle());

}

SceneGame::~SceneGame()
{

}

void SceneGame::Init()
{
	MV1SetPosition(handle, VGet(0, 0, 0));
	MV1SetScale(handle, VGet(500, 500, 500));

	//エネミーの初期化(当たり判定を登録する)
	m_pEnemy->Init(m_pPhysics);
	//エネミーのStateパターンの初期化
	m_pEnemy->InitAiState(shared_from_this());
	m_pEnemy->SetUi(m_pUi);

	//エネミーの座標をプレイヤーに渡す
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//プレイヤーにシーンのポインタを渡す
	m_pPlayer->SetGameScene(shared_from_this());
	//プレイヤーの初期化(当たり判定を登録する)
	m_pPlayer->Init(m_pPhysics);
	//エネミーにプレイヤーの座標を渡す
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());

	//カメラにプレイヤーの座標を渡す
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//カメラにプレイヤーの移動量を渡す
//	m_pGameCamera->SetPlayerVelo(m_pPlayer->GetVelo());
	//カメラの初期化
	m_pGameCamera->Init();

	//プレイヤーに必殺技のデータを入れる
	m_pPlayer->SetAttackData(m_dataManager.GetAttackData());
	//エネミーに必殺技のデータを入れる
	m_pEnemy->SetAttackData(m_dataManager.GetAttackData());
	//エネミーにAIのデータを入れる
	m_pEnemy->SetAiData(m_dataManager.GetAiData());

	//UIに画像のデータを入れる
	m_pUi->LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kGame));
}

void SceneGame::RetryInit()
{
	m_pPlayer->RetryInit();
	m_pEnemy->RetryInit();
	//カメラにプレイヤーの座標を渡す
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//カメラにプレイヤーが敵を中心にどのくらい回転しているかを渡す
	//m_pGameCamera->SetPlayerRota(m_pPlayer->GetRota());
	//カメラの初期化
	m_pGameCamera->Init();
	m_isGameOver = false;
	m_isStartBattle = false;
	m_standByTime = 0;
}

void SceneGame::Update(MyEngine::Input input)
{
	//バトルが開始していないときの処理
	if (!m_isStartBattle)
	{
		m_standByTime++;
		//一定時間待機したら
		if (m_standByTime > kStandByTime)
		{
			//バトルを開始する
			m_isStartBattle = true;
		}

	}
	//バトル開始後の処理
	else
	{

		//ゲームオーバー時の処理
		if (m_isGameOver)
		{
			//上入力
			if (input.IsTrigger(Game::InputId::kUp))
			{
				m_gameOverSelectItemNum--;
				if (m_gameOverSelectItemNum < 0)
				{
					m_gameOverSelectItemNum = 0;
				}
			}
			//下入力
			if (input.IsTrigger(Game::InputId::kDown))
			{
				m_gameOverSelectItemNum++;
				if (m_gameOverSelectItemNum > static_cast<int>(Ui::GameOverItem::kItemNum))
				{
					m_gameOverSelectItemNum = static_cast<int>(Ui::GameOverItem::kItemNum);
				}
			}
			//決定入力
			if (input.IsTrigger(Game::InputId::kOk))
			{
				if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kRetry))
				{
					RetryInit();
				}
				else if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kEnd))
				{
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager));
					return;
				}
			}
		}
		//ゲームオーバーしていないときの処理
		else
		{
			//当たり判定の更新
			m_pPhysics->Update();
			//プレイヤーの更新
			m_pPlayer->Update(shared_from_this(), input);
			//エネミーの更新
			m_pEnemy->Update(shared_from_this());
			m_pEnemy->StateUpdate(m_pPlayer);
		}
		//プレイヤーにエネミーの座標を渡す
		m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
		//エネミーにプレイヤーの座標を渡す
		m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
		//カメラにプレイヤーの座標を渡す
		m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
		//カメラにエネミーの座標を渡す
		m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
		//カメラにプレイヤーの回転行列を渡す
		m_pGameCamera->SetPlayerRotaMat(m_pPlayer->GetModelRotaMatrix());
		//カメラの更新
		m_pGameCamera->Update();
		for (auto& attack : m_pAttacks)
		{
			//攻撃の更新
			if (attack->GetTag() == ObjectTag::kPlayerAttack)
			{
				attack->Update(m_pEnemy->GetPos());
			}
			else if (attack->GetTag() == ObjectTag::kEnemyAttack)
			{
				attack->Update(m_pPlayer->GetPos());
			}
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
		printfDx("%d\n", m_pAttacks.size());


#ifdef _DEBUG
		if (input.IsTrigger(Game::InputId::kPause))
		{
			m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager, m_dataManager));
			return;
		}
#endif
	}
	//エフェクトを更新する
	UpdateEffekseer3D();

}

void SceneGame::Draw()
{

	//スカイドームの描画(仮処理)
	MV1DrawModel(handle);

	//プレイヤーの描画
	m_pPlayer->Draw();
	//エネミーの描画
	m_pEnemy->Draw();
	//エフェクトを描画する

#ifdef _DEBUG
	//当たり判定の描画
	m_pPhysics->DebugDraw();
#endif
	DrawEffekseer3D();
	//プレイヤーとエネミーの体力バーを表示する
	m_pUi->DrawStateBar(m_pPlayer, m_pEnemy);
	//与えたダメージの表示
	m_pUi->DrawDamage();
	//コマンドの表示
	m_pUi->DrawCommand(m_pPlayer->GetIsOpenSpecialPallet(), m_pPlayer->GetSetSpecialAttackName());
	//コンボ数の表示
	m_pUi->DrawComboCount();

	MyEngine::Vector3 pos = m_pGameCamera->GetPos();

	DrawFormatString(200,550,GetColor(0,0,0),"%f,%f,%f",pos.x,pos.y,pos.z);

	//ゲームオーバー時のUIの表示
	if (m_isGameOver)
	{
		m_pUi->DrawGameOver(m_gameOverSelectItemNum);
	}
	//ゲーム開始時のReadyやFightの表示
	m_pUi->DrawStartSign(m_isStartBattle);
}

void SceneGame::End()
{
	m_pPlayer->Final(m_pPhysics);
	m_pEnemy->Final(m_pPhysics);
	MV1DeleteModel(handle);

}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttacks.push_back(pAttack);
}
#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "EnergyAttack.h"
#include "PhysicalAttack.h"
#include "SpecialAttack.h"
namespace
{
	//移動速度
	constexpr float kMoveSpeed = 0.1f;
	constexpr float kAttackPos = 400;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_targetPos(),
	m_rota(0)
{
}

Player::~Player()
{
}

void Player::Init(Physics* physics)
{
	m_pPhysics = physics;
	Collidable::Init(physics);
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{

	//移動ベクトル
	MyEngine::Vector3 velo;

	//移動処理の計算
	velo = Move(velo, input);

	//攻撃処理
	Attack(scene, input);


	//リギットボディにベロシティを設定する
	m_rigidbody.SetVelo(velo);

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	MV1DrawModel(m_modelHandle);
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//スティックでの移動
	{
		//スティックの入力情報
		MyEngine::Input::StickInfo stick = input.GetStickInfo();
		//プレイヤーの移動方向ベクトル
		MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

		//プレイヤーが上下移動するかするかどうか
		bool isMoveVertical = input.IsPress("LB");

		if (stickDir.sqLength() != 0)
		{

			MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();

			//回転速度(横移動の速さ)
			float HMoveSpeed = 0;

			if (stickDir.x != 0)
			{
				HMoveSpeed = (stickDir.x * kMoveSpeed) / toTargetVec.Length();
			}

			DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", HMoveSpeed);

			m_rota += HMoveSpeed;

			//左右移動は敵の周囲を回る
			velo.x += (m_targetPos.x + cosf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().x;
			velo.z += (m_targetPos.z + sinf(m_rota) * toTargetVec.Length()) - m_rigidbody.GetPos().z;

			/*if (isMoveVertical)
			{
				velo.y += -stickDir.z * kMoveSpeed;
			}
			else
			{
			}*/

			velo += toTargetVec.Normalize() * (stickDir.z * kMoveSpeed);
			DrawFormatString(400, 0, GetColor(255, 255, 255), "%f", velo.Length());;

			//MATRIX mat = MGetRotY(toTargetVec.y);

			//stickDir = stickDir.Normalize();


			//velo = stickDir * kHSpeed + toTargetVec * kHSpeed;

			//velo = velo.MatTransform(mat);

		}
	}

	//キーボードでの移動

	//スティックでの入力が行われていなかったら
	//if (velo.sqLength() == 0)
	//{
	//	//プレイヤーの移動方向ベクトル
	//	MyEngine::Vector3 stickDir;
	//	//上入力
	//	if (input.IsPress(Game::InputId::kUp))
	//	{
	//		stickDir.z = 1;
	//	}
	//	//下入力
	//	else if (input.IsPress(Game::InputId::kDown))
	//	{
	//		stickDir.z = -1;
	//	}
	//	//右入力
	//	if (input.IsPress(Game::InputId::kRight))
	//	{
	//		stickDir.x = 1;
	//	}
	//	//左入力
	//	else if (input.IsPress(Game::InputId::kLeft))
	//	{
	//		stickDir.x = -1;
	//	}
	//	//J入力
	//	if (input.IsPress(Game::InputId::kLb))
	//	{
	//		stickDir.y = stickDir.z;
	//		stickDir.z = 0;
	//	}
	//	if (stickDir.sqLength() != 0)
	//	{
	//		MATRIX mat = MGetRotY(m_targetPos.y);

	//		stickDir = stickDir.Normalize();

	//		velo = stickDir * kHSpeed;

	//		velo = velo.MatTransform(mat);

	//	}
	//}

	return velo;
}

void Player::Attack(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//プレイヤーから敵へのベクトルを作成
	MyEngine::Vector3 toEnemyVec = m_targetPos - m_rigidbody.GetPos();

	//攻撃を出す座標を作成
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toEnemyVec.Normalize() * kAttackPos;

	//必殺技パレットを開いていないとき
	if (!input.IsPress("RB"))
	{
		//気弾攻撃
		if (input.IsTrigger("X"))
		{
			//気弾攻撃のポインタ作成
			std::shared_ptr<EnergyAttack> attack = std::make_shared<EnergyAttack>();
				//気弾攻撃の初期化
				attack->Init(m_pPhysics, attackPos);
				//ステータス設定
				AttackBase::AttackStatus status;
			status.atk = 10;
			status.spd = 80.0f;
			status.rad = 100.0f;
			status.dir = toEnemyVec.Normalize();
			attack->SetStatus(status);
			//シーンに気弾攻撃追加
			scene->AddAttack(attack);
		}
		//格闘攻撃
		else if (input.IsTrigger("A"))
		{
			//格闘攻撃のポインタ作成
			std::shared_ptr<PhysicalAttack> attack = std::make_shared<PhysicalAttack>();
			//格闘攻撃の初期化
			attack->Init(m_pPhysics, attackPos);
			//ステータス設定
			AttackBase::AttackStatus status;
			status.atk = 30;
			status.spd = 0.0f;
			status.rad = 200.0f;
			status.dir = toEnemyVec.Normalize();
			attack->SetStatus(status);
			//シーンに格闘攻撃追加
			scene->AddAttack(attack);
		}
	}
	//必殺技パレットを開いているとき
	//else
	//{
	//	//外部ファイル読み込みをする↓

	//	//かめはめ波
	//	if (input.IsTrigger("Y"))
	//	{
	//		//気弾攻撃のポインタ作成
	//		std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>(ObjectTag::kEnergyAttack,ColliderData::Kind::kCapsule);
	//		//気弾攻撃の初期化
	//		attack->Init(m_pPhysics, attackPos);
	//		//ステータス設定
	//		AttackBase::AttackStatus status;
	//		status.atk = 10;
	//		status.spd = 80.0f;
	//		status.rad = 100.0f;
	//		status.dir = toEnemyVec.Normalize();
	//		attack->SetStatus(status);
	//		//シーンに気弾攻撃追加
	//		scene->AddAttack(attack);
	//	}
	//	//格闘必殺1
	//	else if (input.IsTrigger("B"))
	//	{
	//		//格闘攻撃のポインタ作成
	//		std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>();
	//		//格闘攻撃の初期化
	//		attack->Init(m_pPhysics, attackPos);
	//		//ステータス設定
	//		AttackBase::AttackStatus status;
	//		status.atk = 30;
	//		status.spd = 0.0f;
	//		status.rad = 200.0f;
	//		status.dir = toEnemyVec.Normalize();
	//		attack->SetStatus(status);
	//		//シーンに格闘攻撃追加
	//		scene->AddAttack(attack);
	//	}
	//	//格闘必殺2
	//	else if (input.IsTrigger("X"))
	//	{
	//		//格闘攻撃のポインタ作成
	//		std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>();
	//		//格闘攻撃の初期化
	//		attack->Init(m_pPhysics, attackPos);
	//		//ステータス設定
	//		AttackBase::AttackStatus status;
	//		status.atk = 30;
	//		status.spd = 0.0f;
	//		status.rad = 200.0f;
	//		status.dir = toEnemyVec.Normalize();
	//		attack->SetStatus(status);
	//		//シーンに格闘攻撃追加
	//		scene->AddAttack(attack);
	//	}
	//	//気弾連打
	//	else if (input.IsTrigger("A"))
	//	{
	//		//格闘攻撃のポインタ作成
	//		std::shared_ptr<SpecialAttack> attack = std::make_shared<SpecialAttack>();
	//		//格闘攻撃の初期化
	//		attack->Init(m_pPhysics, attackPos);
	//		//ステータス設定
	//		AttackBase::AttackStatus status;
	//		status.atk = 30;
	//		status.spd = 0.0f;
	//		status.rad = 200.0f;
	//		status.dir = toEnemyVec.Normalize();
	//		attack->SetStatus(status);
	//		//シーンに格闘攻撃追加
	//		scene->AddAttack(attack);
	//	}
	//}

}

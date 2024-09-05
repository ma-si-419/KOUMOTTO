#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
#include "CapsuleColliderData.h"
#include "PlayerStateIdle.h"
namespace
{
	//移動速度
	constexpr float kMoveSpeed = 100.0f;
	//攻撃を出す座標
	constexpr float kAttackPos = 500.0f;
	//格闘攻撃を出す時の敵との距離(当たり判定の大きさの倍率)
	constexpr float kPhysicalAttackLange = 0.9f;
	//当たり判定の大きさ
	constexpr float kColScale = 100.0f;
	//トリガーが反応する
	constexpr int kTriggerReaction = 200;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0),
	m_lastAttackTime(0),
	m_isOpenSpecialPallet(false)
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	SetSpecialAttack();

	MV1SetScale(m_modelHandle, VGet(3, 3, 3));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = 10;

	m_pPhysics = physics;
	Collidable::Init(physics);

	//Y軸を中心とした回転をするので
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	m_rigidbody.SetPos(pos);

	//当たり判定
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = 10;


	//Y軸を中心とした回転をするので
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	//当たり判定
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);

	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;

	m_rigidbody.SetPos(pos);

	//Stateの初期化
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->m_nextState->GetKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}
#ifdef _DEBUG
	std::string stateKind = "empty";

	if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kAttack)
	{
		stateKind = "Attack";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kCharge)
	{
		stateKind = "Charge";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kDodge)
	{
		stateKind = "Dodge";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kGuard)
	{
		stateKind = "Guard";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		stateKind = "HitAttack";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kIdle)
	{
		stateKind = "Idle";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kMove)
	{
		stateKind = "Move";
	}

	printfDx(stateKind.c_str());
#endif // _DEBUG

	m_isOpenSpecialPallet = input.IsPress(Game::InputId::kLb);

	//Stateの更新
	m_pState->Update(input);

	//当たり判定の更新
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//体力が0以下になるとゲームオーバー
	if (m_nowHp <= 0)
	{
		scene->GameOver();
	}
#ifdef _DEBUG
	if (input.IsPress(Game::InputId::kSelect))
	{
		m_nowHp -= 1000;
	}
#endif
}

void Player::Draw()
{
	DrawFormatString(200, 500, GetColor(0, 0, 0), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "HP:%f,MP:%f", m_nowHp, m_nowMp);
	MV1DrawModel(m_modelHandle);
}

bool Player::GetAttackKind(std::string attackId)
{
	return m_attackData[attackId].isEnergy;
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - static_cast<int>(m_status.def);
		if (damage < 0)
		{
			damage = 2;
		}

		m_nowHp -= damage;
	}
}

std::map<std::string, std::string> Player::GetSetSpecialAttackName()
{
	std::map<std::string, std::string> ans;

	ans[Game::InputId::kA] = m_attackData[m_setSpecialAttackId[Game::InputId::kA]].name;
	ans[Game::InputId::kB] = m_attackData[m_setSpecialAttackId[Game::InputId::kB]].name;
	ans[Game::InputId::kX] = m_attackData[m_setSpecialAttackId[Game::InputId::kX]].name;
	ans[Game::InputId::kY] = m_attackData[m_setSpecialAttackId[Game::InputId::kY]].name;

	return ans;
}

std::shared_ptr<AttackBase> Player::CreateAttack(std::string id)
{
	//エネミーの攻撃を作成
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);

	//攻撃を出す座標を作成

	MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//ステータス設定
	ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos(), m_status.atk);
	ans->Init(m_pPhysics, attackPos, m_effekseerHandle[m_attackData[id].effekseerName]);

	return ans;
}

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//スティックでの移動

	//スティックの入力情報
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//プレイヤーの移動方向ベクトル
	MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

	//プレイヤーが上下移動するかするかどうか
	bool isMoveVertical = input.GetTriggerInfo().left > kTriggerReaction;

	if (stickDir.sqLength() != 0)
	{

		stickDir = stickDir.Normalize();

		//Y軸を中心とした回転をするので
		MyEngine::Vector3 rotationShaftPos = m_targetPos;
		//Y座標が関係しないようにプレイヤーと同じ座標にする
		rotationShaftPos.y = m_rigidbody.GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

		//回転速度(横移動の速さ)
		float hMoveSpeed = 0;

		if (stickDir.x != 0.0f)
		{
			hMoveSpeed = (stickDir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		//MyEngine::Vector3 a = rotationShaftPos - m_rigidbody.GetPos();

		//m_rota = atan2f(a.z,a.x);

		m_rota += hMoveSpeed;

		//左右移動は敵の周囲を回る

		//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
		//現在の座標からその座標に向かうベクトルを作成する
		velo.x = (rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length()) - m_rigidbody.GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length()) - m_rigidbody.GetPos().z;

		//上下移動入力されていたら
		if (isMoveVertical)
		{
			//前後入力を上下のベクトルに変換
			velo.y += stickDir.z * kMoveSpeed;
		}
		//されていなかった場合
		else
		{
			//前後入力を回転の中心に向かうベクトルに変換
			MyEngine::Vector3 toCenterVec = m_targetPos - m_rigidbody.GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (stickDir.z * kMoveSpeed);
		}


		DrawFormatString(400, 0, GetColor(255, 255, 255), "%f", velo.Length());;

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

void Player::Attack(MyEngine::Input input)
{

	//必殺技パレットを開いていないとき
	if (!input.IsPress("LB"))
	{
		//気弾攻撃
		if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kEnergyAttack1;
			//消費MPが現在のMPよりも少なかったら
			if (m_nowMp >= GetAttackCost(attackId))
			{
				SetNormalAttack(false, m_lastAttackTime);
			}
		}
		//格闘攻撃
		else if (input.IsTrigger("B"))
		{
			SetNormalAttack(true, m_lastAttackTime);
		}
	}
	//必殺技パレットを開いているとき
	else
	{
		//かめはめ波
		if (input.IsTrigger("Y"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttackId["Y"]))
			{
				PlaySpecialAttack(m_setSpecialAttackId["Y"]);
			}
		}
		//格闘必殺1
		else if (input.IsTrigger("B"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttackId["B"]))
			{
				PlaySpecialAttack(m_setSpecialAttackId["B"]);
			}
		}
		//格闘必殺2
		else if (input.IsTrigger("X"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttackId["X"]))
			{
				PlaySpecialAttack(m_setSpecialAttackId["X"]);
			}
		}
		//気弾連打
		else if (input.IsTrigger("A"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttackId["A"]))
			{
				PlaySpecialAttack(m_setSpecialAttackId["A"]);
			}
		}
	}

}

void Player::ChangeState(std::shared_ptr<PlayerStateBase> state)
{
	m_pState = state;
}

void Player::SetSpecialAttack()
{
	m_setSpecialAttackId["A"] = CommandId::kSpEnergyAttack;
	m_setSpecialAttackId["B"] = CommandId::kSpStanAttack;
	m_setSpecialAttackId["X"] = CommandId::kSpSlamAttack;
	m_setSpecialAttackId["Y"] = CommandId::kSpLaserAttack;

}

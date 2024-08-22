#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
#include "CapsuleColliderData.h"
#include "PlayerStateBase.h"
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
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0),
	m_lastAttackTime(0)
{
	LoadAnimationData(true);

	m_pState = std::make_shared<PlayerStateIdle>();
	m_pState->m_nextState = std::make_shared<PlayerStateIdle>();
	//ChangeAnim("Idle");
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
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

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;

}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//移動ベクトル
	MyEngine::Vector3 velo;
	//スタン中は操作できないようにする
	if (m_stanTime < 0)
	{

		//気をためる処理
		if (input.IsPress("Y"))
		{
			//マックスじゃなければ
			if (m_nowMp < m_status.mp)
			{
				m_nowMp++;
			}
		}

		//攻撃処理
		Attack(input);

		m_lastAttackTime++;

		//移動処理
		velo = Move(velo, input);
		//スタンしていない時は常に敵の方向を向き続ける
		//プレイヤーを常にエネミーの方向に向ける
		//TODO：ターゲットがずれた時に一瞬で向くようになってるからそれを
		//一瞬ではなく向いている感じを出す
		MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	}
	//操作できない時間
	else
	{
		//動けない時間を減らす
		m_stanTime--;
		//攻撃を出している状態だったら
		if (m_isAttack)
		{
			//1操作で攻撃を複数回出す技であれば
			if (m_attackData[m_attackId].attackNum > 1)
			{
				//今攻撃を出し始めて何秒か数える
				int time = m_attackData[m_attackId].actionTime - m_stanTime;
				//攻撃のスパンを取得する
				int span = m_attackData[m_attackId].attackTime / m_attackData[m_attackId].attackNum;
				//攻撃を出し切る時間までしか攻撃を出せないようにする
				if (time % span == 0 && time <= m_attackData[m_attackId].attackTime)
				{
					//攻撃を作成
					std::shared_ptr<AttackBase> attack = CreateAttack(m_pPhysics, m_attackId, true);
					//レーザー状の攻撃であれば消える時間をそろえる
					if (m_attackData[m_attackId].isLaser)
					{
						//消えるまでの時間
						int lifeTime = m_attackData[m_attackId].lifeTime - time;

						attack->SetAttackTime(lifeTime);
					}
					//攻撃を出す
					scene->AddAttack(attack);
					m_lastAttackTime = 0;
				}
			}
			//単発攻撃であれば
			else
			{
				//格闘攻撃だった場合
				if (m_attackData[m_attackId].isEnergy == false)
				{
					//攻撃の当たり判定の距離
					float attackLange = ((m_targetPos - m_rigidbody.GetPos()).Normalize() * kAttackPos).Length() + m_attackData[m_attackId].radius * kPhysicalAttackLange;

					//敵との距離が近くなったら
					if ((m_targetPos - m_rigidbody.GetPos()).Length() < attackLange)
					{
						m_isNearTarget = true;
					}

					//まだ敵との距離が遠ければ
					if (!m_isNearTarget)
					{
						//敵に近づく
						velo = (m_targetPos - m_rigidbody.GetPos()).Normalize() * kMoveSpeed;
						//敵が近くにいない場合攻撃のカウントを止める
						m_stanTime++;
					}

					//攻撃モーションに入って何秒か求める
					int time = m_attackData[m_attackId].actionTime - m_stanTime;
					//攻撃を出す時間になったら
					if (time > m_attackData[m_attackId].attackTime)
					{
						//攻撃を出す
						scene->AddAttack(CreateAttack(m_pPhysics, m_attackId, true));
						m_isAttack = false;
						m_lastAttackTime = 0;
					}
				}
				//気弾攻撃だった場合
				else
				{
					//攻撃モーションに入って何秒か求める
					int time = m_attackData[m_attackId].actionTime - m_stanTime;
					//攻撃を出す時間になったら
					if (time > m_attackData[m_attackId].attackTime)
					{
						//攻撃を出す
						scene->AddAttack(CreateAttack(m_pPhysics, m_attackId, true));
						m_isAttack = false;
						m_lastAttackTime = 0;
					}

				}
			}
		}
	}

	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->m_nextState->GetKind() != m_pState->GetKind())
	{
		//Stateを変更する
		m_pState = m_pState->m_nextState;
	}

	//Stateの更新
	m_pState->Update(std::static_pointer_cast<Player>(shared_from_this()), input);

	//リギットボディにベロシティを設定する
	m_rigidbody.SetVelo(velo);

	//アニメーションを進める
	m_animTime += m_animPlaySpeed;
	if (m_animTime > m_totalAnimTime)
	{
		m_animTime = m_animLoopTime;
	}
	for (auto item : m_playAnims)
	{
		MV1SetAttachAnimTime(m_modelHandle, item.first, m_animTime);
	}
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
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

MyEngine::Vector3 Player::Move(MyEngine::Vector3 velo, MyEngine::Input input)
{
	//スティックでの移動

	//スティックの入力情報
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//プレイヤーの移動方向ベクトル
	MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

	//プレイヤーが上下移動するかするかどうか
	bool isMoveVertical = input.IsPress(Game::InputId::kLb);

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

		MyEngine::Vector3 a = rotationShaftPos - m_rigidbody.GetPos();

		/*m_rota = atan2f(a.z,a.x);

		m_rota += hMoveSpeed;*/

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

	MoveAnim(stickDir);



	m_lastInput = stickDir;

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
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["Y"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["Y"]);
			}
		}
		//格闘必殺1
		else if (input.IsTrigger("B"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["B"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["B"]);
			}
		}
		//格闘必殺2
		else if (input.IsTrigger("X"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["X"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["X"]);
			}
		}
		//気弾連打
		else if (input.IsTrigger("A"))
		{
			//MPが十分にあったら
			if (m_nowMp >= GetAttackCost(m_setSpecialAttack["A"]))
			{
				PlaySpecialAttack(m_setSpecialAttack["A"]);
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
	m_setSpecialAttack["A"] = CommandId::kSpEnergyAttack;
	m_setSpecialAttack["B"] = CommandId::kSpStanAttack;
	m_setSpecialAttack["X"] = CommandId::kSpSlamAttack;
	m_setSpecialAttack["Y"] = CommandId::kSpLaserAttack;

}

#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
namespace
{
	//移動速度
	constexpr float kMoveSpeed = 100.0f;
	//攻撃を出す座標
	constexpr float kAttackPos = 500.0f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0)
{
}

Player::~Player()
{
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	MV1SetScale(m_modelHandle, VGet(300, 300, 300));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

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
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	m_stanTime--;
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
		Attack(scene, input);

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
		//攻撃を出している状態だったら
		if (m_isAttack)
		{
			//攻撃を複数回出す技であれば
			if (m_attackData[m_attackId].attackNum > 1)
			{
				//今攻撃を出し始めて何秒か
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
				}
			}
			//攻撃が一度のみであれば
			else
			{
				//攻撃を出す
				scene->AddAttack(CreateAttack(m_pPhysics, m_attackId,true));
				m_isAttack = false;
			}
		}
	}
	//リギットボディにベロシティを設定する
	m_rigidbody.SetVelo(velo);

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Player::Draw()
{
	DrawFormatString(0, 100, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 300, GetColor(255, 255, 255), "HP:%d,MP:%d", m_nowHp, m_nowMp);
	MV1DrawModel(m_modelHandle);
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - m_status.def;
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
	{
		//スティックの入力情報
		MyEngine::Input::StickInfo stick = input.GetStickInfo();
		//プレイヤーの移動方向ベクトル
		MyEngine::Vector3 stickDir(stick.leftStickX, 0, -stick.leftStickY);

		//プレイヤーが上下移動するかするかどうか
		bool isMoveVertical = input.IsPress("LB");

		if (stickDir.sqLength() != 0)
		{

			stickDir = stickDir.Normalize();

			//Y軸を中心とした回転をするので
			MyEngine::Vector3 rotationShaftPos = m_targetPos;
			//Y座標が関係しないようにプレイヤーと同じ座標にする
			rotationShaftPos.y = m_rigidbody.GetPos().y;

			MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

			//回転速度(横移動の速さ)
			float HMoveSpeed = 0;

			if (stickDir.x != 0.0f)
			{
				HMoveSpeed = (stickDir.x * kMoveSpeed) / toShaftPosVec.Length();
			}

			DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", HMoveSpeed);

			/*MyEngine::Vector3 a = rotationShaftPos - m_rigidbody.GetPos();

			m_rota = atan2f(a.z,a.x);*/

			m_rota += HMoveSpeed;

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

	//必殺技パレットを開いていないとき
	if (!input.IsPress("RB"))
	{
		//気弾攻撃
		if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kEnergyAttack;
			//消費MPが現在のMPよりも少なかったら
			if (m_nowMp > GetAttackCost(attackId))
			{
				//気弾攻撃のみ移動中に出せる技であるのでここで技を出す
				m_nowMp -= m_attackData[attackId].cost;
				m_attackTarget = m_targetPos;
				scene->AddAttack(CreateAttack(m_pPhysics, attackId,true));

			}
		}
		//格闘攻撃
		else if (input.IsTrigger("B"))
		{
			std::string attackId = CommandId::kPhysicalAttack;
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
	}
	//必殺技パレットを開いているとき
	else
	{
		//かめはめ波
		if (input.IsTrigger("Y"))
		{
			std::string attackId = CommandId::kSpLaserAttack;
			//MPが十分にあったら
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//格闘必殺1
		else if (input.IsTrigger("B"))
		{
			std::string attackId = CommandId::kSpStanAttack;
			//MPが十分にあったら
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//格闘必殺2
		else if (input.IsTrigger("X"))
		{
			std::string attackId = CommandId::kSpSlamAttack;
			//MPが十分にあったら
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
		//気弾連打
		else if (input.IsTrigger("A"))
		{
			std::string attackId = CommandId::kSpEnergyAttack;
			//MPが十分にあったら
			if (m_nowMp > GetAttackCost(attackId))
			{
				SetAttack(attackId);
			}
		}
	}

}

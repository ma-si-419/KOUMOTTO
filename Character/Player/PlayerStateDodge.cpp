#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include <cmath>

namespace
{
	constexpr float kDodgeSpeed = 200.0f;
	//回避の時間
	constexpr int kDodgeTime = 10;
	//敵との最短距離
	constexpr int kShortestEnemyDistance = 1500;
}
void PlayerStateDodge::Init(MyEngine::Vector3 dir)
{
	m_moveDir = dir;
	m_pPlayer->PlaySE("Dodge",DX_PLAYTYPE_BACK);
	m_pPlayer->ChangeAnim("Move");
	m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Dodge"));
	m_pPlayer->SetUpFov(true);
}
void PlayerStateDodge::Update(MyEngine::Input input)
{
	//視野角を広げる
	m_pPlayer->SetUpFov(true);
	//エネミーの座標
	MyEngine::Vector3 targetPos = m_pPlayer->GetTargetPos();
	//Y軸を中心とした回転をするので
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	MyEngine::Vector3 rotationShaftPos = targetPos;
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	//プレイヤーから回転の中心へのベクトル
	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	MyEngine::Vector3 nextPos;

	//回転の大きさ
	float hMoveScale = 0;

	//現状の回転度を取得する
	float x = m_pPlayer->GetPos().x - rotationShaftPos.x;
	float z = m_pPlayer->GetPos().z - rotationShaftPos.z;

	float angle = std::atan2(z, x);

	if (m_moveDir.x != 0.0f)
	{
		hMoveScale = (m_moveDir.x * kDodgeSpeed) / toShaftPosVec.Length();
	}

	//敵に近すぎたら周囲を回るようにする
	if (toShaftPosVec.Length() < kShortestEnemyDistance)
	{
		//前入力されている場合
		if (m_moveDir.z > 0)
		{
			//横入力の値で回る方向を決める
			if (m_moveDir.x > 0)
			{
				//前入力で回転する
				hMoveScale += (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			}
			else
			{
				//前入力で回転する
				hMoveScale -= (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			}
		}
		//後ろ入力されている場合
		else
		{
			MyEngine::Vector3 toTargetVec = targetPos - m_pPlayer->GetPos();

			//エネミーから離れていくベクトル
			nextPos = nextPos + toTargetVec.Normalize() * m_moveDir.z * kDodgeSpeed;
		}
	}
	//敵から一定距離離れていた場合
	else
	{
		//前後入力されたら敵に向かっていく
		MyEngine::Vector3 toTargetVec = targetPos - m_pPlayer->GetPos();
		nextPos = nextPos + toTargetVec.Normalize() * m_moveDir.z * kDodgeSpeed;
	}

	//現在の角度に横移動の大きさを足す
	angle += hMoveScale;

	nextPos.x += cosf(angle) * toShaftPosVec.Length() + rotationShaftPos.x;
	nextPos.z += sinf(angle) * toShaftPosVec.Length() + rotationShaftPos.z;

	m_pPlayer->SetVelo(nextPos - m_pPlayer->GetPos());

	m_time++;

	//回避の時間を見る
	if (m_time < kDodgeTime)
	{
		//自分のポインタを返す
		m_nextState = shared_from_this();
	}
	//回避の時間が終わったら
	else
	{
		//アイドル状態に戻る
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer,m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		m_pPlayer->StopEffect();
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//攻撃に当たらないようにする
	return 0;
}

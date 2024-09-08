#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"
#include "Player.h"


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
	m_pPlayer->ChangeAnim("Move");
	m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Dodge"));
	m_pPlayer->SetUpFov(true);
}
void PlayerStateDodge::Update(MyEngine::Input input)
{
	//視野角を広げる
	m_pPlayer->SetUpFov(true);
	//移動と同じ処理
	MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	MyEngine::Vector3 velo(0, 0, 0);

	//回転速度(横移動の速さ)
	float hMoveSpeed = 0;

	if (m_moveDir.x != 0.0f)
	{
		hMoveSpeed = (m_moveDir.x * kDodgeSpeed) / toShaftPosVec.Length();
	}

	m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

	velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
	velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

	//敵に近すぎたら周囲を回るようにする
	if (toShaftPosVec.Length() > kShortestEnemyDistance)
	{
		//前後入力を回転の中心に向かうベクトルに変換
		toShaftPosVec.y = 0;
		velo += toShaftPosVec.Normalize() * (m_moveDir.z * kDodgeSpeed);
	}
	else
	{
		//前入力を横移動に後ろ入力を回転の中心から離れるベクトルに変換
		if (m_moveDir.z > 0)
		{
			hMoveSpeed = (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);
			velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
			velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;
		}
		else if (m_moveDir.z < 0)
		{
			velo += toShaftPosVec.Normalize() * (m_moveDir.z * kDodgeSpeed);
		}
	}

	m_pPlayer->SetVelo(velo);

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

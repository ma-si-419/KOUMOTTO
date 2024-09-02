#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"

namespace
{
	constexpr float kDodgeScale = 200.0f;
	//回避の時間
	constexpr int kDodgeTime = 10;
}
void PlayerStateDodge::Update(MyEngine::Input input)
{

	MyEngine::Vector3 dir = m_moveDir.Normalize();

	//移動と同じ処理
	MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	//回転速度(横移動の速さ)
	float hMoveSpeed = 0;

	if (dir.x != 0.0f)
	{
		hMoveSpeed = (dir.x * kDodgeScale) / toShaftPosVec.Length();
	}

	MyEngine::Vector3 a = rotationShaftPos - m_pPlayer->GetPos();

	//m_rota = atan2f(a.z,a.x);

	m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

	MyEngine::Vector3 velo(0, 0, 0);

	velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
	velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

	MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
	toCenterVec.y = 0;
	velo += toCenterVec.Normalize() * (dir.z * kDodgeScale);

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
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//攻撃に当たらないようにする
	return 0;
}

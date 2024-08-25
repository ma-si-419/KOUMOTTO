#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"

namespace
{
	constexpr float kDodgeScale = 200.0f;
	//回避の時間
	constexpr int kDodgeTime = 10;
}
void PlayerStateDodge::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{

	MyEngine::Vector3 dir = m_moveDir.Normalize();

	//移動と同じ処理
	MyEngine::Vector3 rotationShaftPos = player->GetTargetPos();
	rotationShaftPos.y = player->GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - player->GetPos();

	//回転速度(横移動の速さ)
	float hMoveSpeed = 0;

	if (dir.x != 0.0f)
	{
		hMoveSpeed = (dir.x * kDodgeScale) / toShaftPosVec.Length();
	}

	MyEngine::Vector3 a = rotationShaftPos - player->GetPos();

	//m_rota = atan2f(a.z,a.x);

	player->SetRota(player->GetRota() + hMoveSpeed);

	MyEngine::Vector3 velo(0, 0, 0);

	velo.x = (rotationShaftPos.x + cosf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().x;
	velo.z = (rotationShaftPos.z + sinf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().z;

	MyEngine::Vector3 toCenterVec = player->GetTargetPos() - player->GetPos();
	toCenterVec.y = 0;
	velo += toCenterVec.Normalize() * (dir.z * kDodgeScale);

	player->SetVelo(velo);

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
		m_nextState = std::make_shared<PlayerStateIdle>();
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//攻撃に当たらないようにする
	return 0;
}

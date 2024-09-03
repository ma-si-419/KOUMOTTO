#include "EnemyStateCharge.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 30;
}

void EnemyStateCharge::Update()
{
	//時間を計測する
	m_time++;

	//Charge状態は中身的には何もしない

	//このフレームにいる最低時間を超えたら確率で別のフレームに行く
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();

	return damage;
}

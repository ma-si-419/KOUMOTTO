#include "EnemyStateHitAttack.h"

namespace
{
	//軽い攻撃を受けたときのノックバックの速さ
	constexpr float kLightKnockBackSpeed = 30.0f;
	//吹き飛ばし攻撃を受けた時のノックバックの速さ
	constexpr float kBurstKnockBackSpeed = 120.0f;
	//ガードした時のノックバックの速さ
	constexpr float kGuardKnockBackSpeed = 10.0f;
	//軽い攻撃を受けた時の動けない時間
	constexpr int kLightHitStunTime = 5;
	//吹き飛ばし攻撃を受けた時の動けない時間
	constexpr int kBurstHitStunTime = 30;
	//スタン攻撃を受けた時の動けない時間
	constexpr int kStunHitStunTime = 40;
	//ガードした時の動けない時間
	constexpr int kGuardStunTime = 3;

}

void EnemyStateHitAttack::Init(int effect)
{
	if (effect == static_cast<int>(HitEffect::kBurst))
	{
		m_effect = HitEffect::kBurst;
	}
	else if (effect == static_cast<int>(HitEffect::kLightHit))
	{
		m_effect = HitEffect::kLightHit;
	}
	else if (effect == static_cast<int>(HitEffect::kStun))
	{
		m_effect = HitEffect::kStun;
	}
	else if (effect == static_cast<int>(HitEffect::kGuard))
	{
		m_effect = HitEffect::kGuard;
	}
}

void EnemyStateHitAttack::Update()
{
	//経過時間を計測する
	m_time++;
	//移動ベクトル
	MyEngine::Vector3 velo;

	//軽い攻撃を受けた時
	if (m_effect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//軽い攻撃を受けた時のスタン時間が経過したら
		if (m_time > kLightHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//吹き飛ばし攻撃を受けた時
	else if (m_effect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//吹き飛ばし攻撃を受けた時のスタン時間が経過したら
		if (m_time > kBurstHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//スタン攻撃を受けた時
	else if(m_effect == HitEffect::kStun)
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//スタン攻撃を受けた時のスタン時間が経過したら
		if (m_time > kStunHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//ガードしていた時
	else
	{
		velo = m_burstDir * kGuardKnockBackSpeed;
		//ガードした時のスタン時間が経過したら
		if (m_time > kGuardStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	m_pEnemy->SetVelo(velo);
}

int EnemyStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();

	return damage;
}

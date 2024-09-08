#include "EnemyStateCharge.h"
#include "EffekseerForDXLib.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 120;
}

void EnemyStateCharge::Init()
{
	//エフェクトの設定
	m_pEnemy->SetPlayEffect(m_pEnemy->GetEffekseerData("Charge"));

	//アニメーションの設定
	m_pEnemy->ChangeAnim("Charge");
}

void EnemyStateCharge::Update()
{
	//時間を計測する
	m_time++;

	//Charge状態は中身的には何もしない
	m_pEnemy->SetVelo(MyEngine::Vector3(0, 0, 0));

	//アニメーションの更新
	m_pEnemy->PlayAnim();

	//このフレームにいる最低時間を超えたら確率で別のフレームに行く
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
		StopEffekseer3DEffect(m_playEffectHandle);
		m_pEnemy->StopEffect();
	}
}

int EnemyStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();
	StopEffekseer3DEffect(m_playEffectHandle);
	DeleteEffekseerEffect(m_effectHandle);
	m_isChangeState = true;
	return damage;
}

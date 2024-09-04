#include "EnemyStateGuard.h"
#include "EffekseerForDXLib.h"
namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 120;
	//ダメージの軽減率
	constexpr float kDamageCutRate = 0.25f;
}
void EnemyStateGuard::Init()
{
	m_effectHandle = LoadEffekseerEffect("data/effekseer/barrior.efk");
	m_playEffectHandle = PlayEffekseer3DEffect(m_effectHandle);
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	SetPosPlayingEffekseer3DEffect(m_playEffectHandle,pos.x,pos.y,pos.z);
	m_pEnemy->ChangeAnim("Guard");
}
void EnemyStateGuard::Update()
{
	//時間を計測する
	m_time++;

	m_pEnemy->PlayAnim();

	//Guard状態は移動しない
	m_pEnemy->SetVelo(MyEngine::Vector3(0, 0, 0));

	

	//このフレームにいる最低時間を超えたら確率で別のフレームに行く
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
		StopEffekseer3DEffect(m_playEffectHandle);
		DeleteEffekseerEffect(m_effectHandle);
	}
}

int EnemyStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージカットする
	damage = (attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def))) * kDamageCutRate;
	//状態を変化させない
	return damage;
}

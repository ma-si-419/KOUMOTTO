#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
namespace
{
	//ダメージの軽減率
	constexpr float kDamageCutRate = 0.1f;
}
void PlayerStateGuard::Init()
{
	m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Guard"));
	m_pPlayer->ChangeAnim("Guard");
}
void PlayerStateGuard::Update(MyEngine::Input input)
{
	//敵の方向を向くようにする
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//移動しないようにする
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	//アニメーションを動かす
	m_pPlayer->PlayAnim();
	//状態の変化
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	//ガード入力がされていなければ
	else
	{
		//状態を変化させる
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		m_pPlayer->StopEffect();
		return;
	}
}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそカットして返す
	damage = attack->GetDamage() * kDamageCutRate;

	return damage;
}

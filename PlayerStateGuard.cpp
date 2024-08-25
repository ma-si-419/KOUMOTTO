#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"

void PlayerStateGuard::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	else
	{
		m_nextState = std::make_shared<PlayerStateIdle>();
	}
}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>();
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}

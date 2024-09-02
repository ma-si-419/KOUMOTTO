#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"

void PlayerStateGuard::Update(MyEngine::Input input)
{
	//敵の方向を向くようにする
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//移動しないようにする
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	//状態の変化
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	else
	{
		//次の状態に合わせたアニメーションに変化させる
		m_pPlayer->ChangeAnim("Idle");
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		return;
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
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}

#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
namespace
{
	//ダメージの軽減率
	constexpr float kDamageCutRate = 0.25f;
}
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
	//ダメージをそカットして返す
	damage = attack->GetDamage() * kDamageCutRate;

	return damage;
}

#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalPhysicalAttack : public PlayerStateBase
{
public:
	PlayerStateNormalPhysicalAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalPhysicalAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//今何段目の攻撃をしているのか
	int m_attackNum = 0;
	//状態遷移して何秒目か
	int m_time = 0;
	//攻撃の入力がされたかどうか
	bool m_isAttackInput = false;
	//敵の近くにいるかどうか
	bool m_isNearEnemy = false;
};


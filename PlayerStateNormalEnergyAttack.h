#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalEnergyAttack : public PlayerStateBase
{
public:
	PlayerStateNormalEnergyAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackNum(int num) { m_attackNum = num; }
private:
	//今何段目の攻撃をしているのか
	int m_attackNum = 0;
	//状態遷移して何秒目か
	int m_time = 0;
	//攻撃の入力がされたかどうか
	bool m_isAttackInput = false;
};


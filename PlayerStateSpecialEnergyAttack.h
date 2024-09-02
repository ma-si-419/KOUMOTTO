#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpecialEnergyAttack : public PlayerStateBase
{
public:
	PlayerStateSpecialEnergyAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetTime(int time) { m_actionTime = time; }
private:
	//âΩïbä‘Ç≈å≥ÇÃèÛë‘Ç…ñﬂÇÈÇ©
	int m_actionTime = 0;
};
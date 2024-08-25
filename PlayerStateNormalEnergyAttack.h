#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalEnergyAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackNum(int num) { m_attackNum = num; }
private:
	//¡‰½’i–Ú‚ÌUŒ‚‚ğ‚µ‚Ä‚¢‚é‚Ì‚©
	int m_attackNum = 0;
	//ó‘Ô‘JˆÚ‚µ‚Ä‰½•b–Ú‚©
	int m_time = 0;
	//UŒ‚‚Ì“ü—Í‚ª‚³‚ê‚½‚©‚Ç‚¤‚©
	bool m_isAttackInput = false;
};


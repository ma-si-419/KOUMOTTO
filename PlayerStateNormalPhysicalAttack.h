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
	//¡‰½’i–Ú‚ÌUŒ‚‚ğ‚µ‚Ä‚¢‚é‚Ì‚©
	int m_attackNum = 0;
	//ó‘Ô‘JˆÚ‚µ‚Ä‰½•b–Ú‚©
	int m_time = 0;
	//UŒ‚‚Ì“ü—Í‚ª‚³‚ê‚½‚©‚Ç‚¤‚©
	bool m_isAttackInput = false;
	//“G‚Ì‹ß‚­‚É‚¢‚é‚©‚Ç‚¤‚©
	bool m_isNearEnemy = false;
};


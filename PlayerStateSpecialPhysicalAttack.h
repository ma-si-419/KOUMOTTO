#pragma once
#include "PlayerStateBase.h"
class PlayerStateSpecialPhysicalAttack : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialPhysicalAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetTime(int time) { m_actionTime = time; }
private:
	//ó‘Ô‘JˆÚ‚µ‚Ä‰½•b–Ú‚©
	int m_time = 0;
	//‰½•bŠÔ‚ÅŒ³‚Ìó‘Ô‚É–ß‚é‚©
	int m_actionTime = 0;
	//“G‚Ì‹ß‚­‚É‚¢‚é‚©‚Ç‚¤‚©
	bool m_isNearEnemy = false;
};


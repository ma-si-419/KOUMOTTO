#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kSpecialEnergyAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAttackId(std::string id) { m_attackId = id; }
private:
	//Œo‰ßŠÔ
	int m_time = 0;
	//‰½•bŠÔ‚ÅŒ³‚Ìó‘Ô‚É–ß‚é‚©
	int m_actionTime = 0;
	//‰½‚Ì‹Z‚ğo‚µ‚Ä‚¢‚é‚©
	std::string m_attackId = "empty";
	//“G‚ª‹ß‚­‚É‚¢‚é‚©‚Ç‚¤‚©
	bool m_isNearEnemy = false;
	//UŒ‚‚Ì“ü—Í‚ª‚³‚ê‚½‚©‚Ç‚¤‚©
	bool m_isAttackInput = false;
	//’ÊíUŒ‚‚ğo‚µ‚Ä‚¢‚éê‡A‰½’i–Ú‚ÌUŒ‚‚ğo‚µ‚Ä‚¢‚é‚©
	int m_normalAttackNum = 0;
};


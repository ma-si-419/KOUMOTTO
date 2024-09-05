#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack(std::shared_ptr<Player> player , std::shared_ptr<SceneGame> scene) : PlayerStateBase(player,scene) {}

	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	/// <param name="button">‰Ÿ‚µ‚½ƒ{ƒ^ƒ“</param>
	/// <param name="isSpecial">•KE‹Z‚©‚Ç‚¤‚©</param>
	void Init(std::string button,bool isSpecial);

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

private:

	/// <summary>
	/// Ÿ‚Ì’ÊíUŒ‚‚ÌID‚ğ•Ô‚·
	/// </summary>
	/// <param name="id">¡o‚µ‚Ä‚¢‚é’ÊíUŒ‚</param>
	/// <returns>Ÿ‚Éo‚·’ÊíUŒ‚</returns>
	std::string GetNextNormalAttack(std::string id);

	//Œo‰ßŠÔ
	int m_time = 0;
	//‰½•bŠÔ‚ÅŒ³‚Ìó‘Ô‚É–ß‚é‚©
	int m_actionTime = 0;
	//‰½‚Ì‹Z‚ğo‚µ‚Ä‚¢‚é‚©
	std::string m_attackId = "empty";
	//Ÿ‚Éo‚·—\’è‚ÌID
	std::string m_nextAttackId = "empty";
	//“G‚ª‹ß‚­‚É‚¢‚é‚©‚Ç‚¤‚©
	bool m_isGoTarget = false;
	//UŒ‚‚Ìƒ‚[ƒVƒ‡ƒ“‚É“ü‚Á‚Ä‚¢‚é‚©‚Ç‚¤‚©‚Ç‚¤‚©
	bool m_isStartAttack = false;
	//•ø‚¢‚Ä‚¢‚éUŒ‚‚ª’ÊíUŒ‚‚©‚Ç‚¤‚©
	bool m_isNormalAttack = false;
	//UŒ‚‚ªI‚í‚Á‚½‚©‚Ç‚¤‚©
	bool m_isAttackEnd = false;
};


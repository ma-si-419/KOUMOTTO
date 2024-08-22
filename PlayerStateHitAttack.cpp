#include "PlayerStateHitAttack.h"

void PlayerStateHitAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//Œy‚¢UŒ‚‚ğó‚¯‚½
	if (m_hitEffect == HitEffect::kLightHit)
	{

	}
	//‚«”ò‚Î‚µUŒ‚‚ğó‚¯‚½
	else if (m_hitEffect == HitEffect::kBurst)
	{

	}
	//ƒXƒ^ƒ“UŒ‚‚ğó‚¯‚½
	else
	{

	}
}

int PlayerStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

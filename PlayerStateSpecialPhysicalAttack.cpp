#include "PlayerStateSpecialPhysicalAttack.h"

void PlayerStateSpecialPhysicalAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateSpecialPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

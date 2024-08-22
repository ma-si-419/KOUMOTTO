#include "PlayerStateSpecialEnergyAttack.h"

void PlayerStateSpecialEnergyAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateSpecialEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

#include "PlayerStateNormalEnergyAttack.h"

void PlayerStateNormalEnergyAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateNormalEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

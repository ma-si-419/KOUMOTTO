#include "PlayerStateDodge.h"

void PlayerStateDodge::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

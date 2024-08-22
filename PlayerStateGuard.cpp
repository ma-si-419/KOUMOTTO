#include "PlayerStateGuard.h"

void PlayerStateGuard::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

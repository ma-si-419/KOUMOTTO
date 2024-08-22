#include "PlayerStateMove.h"

void PlayerStateMove::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

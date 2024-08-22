#include "PlayerStateNormalPhysicalAttack.h"

void PlayerStateNormalPhysicalAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

int PlayerStateNormalPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

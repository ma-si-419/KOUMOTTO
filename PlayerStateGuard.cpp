#include "PlayerStateGuard.h"

std::shared_ptr<PlayerStateBase> PlayerStateGuard::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	return shared_from_this();
}

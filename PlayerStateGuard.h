#pragma once
#include "PlayerStateBase.h"
class PlayerStateGuard : public PlayerStateBase
{
public:

	PlayerStateGuard(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kGuard; }
	
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
	
};


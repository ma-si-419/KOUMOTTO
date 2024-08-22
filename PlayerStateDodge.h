#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge : public PlayerStateBase
{
public:
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kDodge; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

};


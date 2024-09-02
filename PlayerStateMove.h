#pragma once
#include "PlayerStateBase.h"
class PlayerStateMove : public PlayerStateBase
{
public:
	PlayerStateMove(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kMove; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

};



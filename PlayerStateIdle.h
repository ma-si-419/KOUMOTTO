#pragma once
#include "PlayerStateBase.h"
class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player) : PlayerStateBase(player) {}

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kIdle; }
	
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};


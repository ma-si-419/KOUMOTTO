#pragma once
#include "PlayerStateBase.h"
class PlayerStateNormalPhysicalAttack : public PlayerStateBase
{
public:
	virtual std::shared_ptr<PlayerStateBase> Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kNormalPhysicalAttack; }
};


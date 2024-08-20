#pragma once
#include <memory>
#include "Input.h"
#include "Collidable.h"
class Player;
class PlayerStateBase : public std::enable_shared_from_this<PlayerStateBase>
{
public:
	enum class PlayerStateKind
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kCharge,
		kBoostAttack,
		kSpecialEnergyAttack,
		kNormalEnergyAttack,
		kSpecialPhysicalAttack,
		kNormalPhysicalAttack,
		kGuard,
		kDown,
		kHitLightAttack,
		kHitHeavyAttack
	};
	virtual std::shared_ptr<PlayerStateBase> Update(std::shared_ptr<Player> player, MyEngine::Input input) abstract;
	//åªç›ÇÃStateÇï‘Ç∑
	virtual PlayerStateKind GetKind() abstract;
};
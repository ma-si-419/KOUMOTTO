#pragma once
#include <memory>
#include "Input.h"
#include "Collidable.h"
#include "AttackBase.h"
#include "Player.h"

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
		kHitAttack
	};
	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) abstract;
	//現在のStateを返す
	virtual PlayerStateKind GetKind() abstract;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//次移行するStateを保存する
	std::shared_ptr<PlayerStateBase> m_nextState;
};
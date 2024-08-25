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
	//���݂�State��Ԃ�
	virtual PlayerStateKind GetKind() abstract;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//���ڍs����State��ۑ�����
	std::shared_ptr<PlayerStateBase> m_nextState;
};
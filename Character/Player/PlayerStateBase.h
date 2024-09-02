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
		kAttack,
		kGuard,
		kCharge,
		kHitAttack,
		kSpecialEnergyAttack,
		kNormalEnergyAttack,
		kSpecialPhysicalAttack,
		kNormalPhysicalAttack
	};
	PlayerStateBase(std::shared_ptr<Player> player) { m_pPlayer = player; };

	virtual void Update(MyEngine::Input input) abstract;
	//���݂�State��Ԃ�
	virtual PlayerStateKind GetKind() abstract;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//���ڍs����State��ۑ�����
	std::shared_ptr<PlayerStateBase> m_nextState;
	//�v���C���[�̃|�C���^�������Ă���
	std::shared_ptr<Player> m_pPlayer;
};
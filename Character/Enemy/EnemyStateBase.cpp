#include "EnemyStateBase.h"
void EnemyStateBase::CheckSituation(PlayerStateBase::PlayerStateKind playerState)
{
	//���Ɉڍs����State��ݒ肷��
	std::shared_ptr<EnemyStateBase> nextState;

	//�v���C���[��State�ɂ���Ďg���f�[�^��ς���
	std::string data;
	if (playerState == PlayerStateBase::PlayerStateKind::kIdle)
	{
		data = "Idle";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kMove)
	{
		data = "Move";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kDash)
	{
		data = "Dash";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kDodge)
	{
		data = "Dodge";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kAttack)
	{
		data = "Attack";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kGuard)
	{
		data = "Guard";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kCharge)
	{
		data = "Charge";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		data = "HitAttack";
	}

	
	
}

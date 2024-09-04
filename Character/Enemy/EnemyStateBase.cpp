#include "EnemyStateBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateAttack.h"
#include "EnemyStateCharge.h"
#include "EnemyStateDash.h"
#include "EnemyStateDodge.h"
#include "EnemyStateGuard.h"
#include "EnemyStateMove.h"
#include "EnemyStateHitAttack.h"


void EnemyStateBase::CheckSituation(std::shared_ptr<Player> player)
{

	//�v���C���[��State�ɂ���Ďg���f�[�^��ς���
	std::string data;
	if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kIdle)
	{
		data = "Idle";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kMove)
	{
		data = "Move";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kDash)
	{
		data = "Dash";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kDodge)
	{
		data = "Dodge";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kAttack)
	{
		data = "Attack";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kGuard)
	{
		data = "Guard";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kCharge)
	{
		data = "Charge";
	}
	else if (player->GetStateKind() == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		data = "HitAttack";
	}

	//���ׂĂ̊m���𑫂��ă����_���͈̔͂����肷��
	int randRange = 0;

	//�v���C���[�̏�Ԃ����Ċm�����Q�Ƃ���
	for (auto item : m_aiData[data])
	{
		randRange += item;
	}

	//�����_���͈̔͂����܂����烉���_���Ōv�Z����
	int ans = GetRand(randRange);

	//���ʂ��킩�����炻��ɑΉ�����State��T��
	int stateIndex = 0;

	for (auto item : m_aiData[data])
	{
		ans -= item;
		if (ans <= 0)
		{
			break;
		}
		stateIndex++;
	}

#ifdef _DEBUG

	stateIndex = static_cast<int>(EnemyStateKind::kAttack);

#endif // _DEBUG

	if (m_isChangeState)
	{
		//��Ɏ��̍s�����X�V��������

		//�������Ȃ�
		if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kIdle)
		{
			m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateIdle>(m_nextState);
			state->Init();
		}
		//�U��
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kAttack)
		{
			m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateAttack>(m_nextState);
			state->Init(player);
		}
		//�_�b�V��
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kDash)
		{
			m_nextState = std::make_shared<EnemyStateDash>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateDash>(m_nextState);
			state->Init(player->GetPos());
		}
		//���
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kDodge)
		{
			m_nextState = std::make_shared<EnemyStateDodge>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateDodge>(m_nextState);
			state->Init();
		}
		//�C�����߂�(�A�j���[�V���������ňړ��͂��Ȃ�)
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kCharge)
		{
			m_nextState = std::make_shared<EnemyStateCharge>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateCharge>(m_nextState);
			state->Init();
		}
		//�K�[�h
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kGuard)
		{
			m_nextState = std::make_shared<EnemyStateGuard>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateGuard>(m_nextState);
			state->Init();
		}
		//�ړ�
		else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kMove)
		{
			m_nextState = std::make_shared<EnemyStateMove>(m_pEnemy, m_pScene);
			auto state = std::dynamic_pointer_cast<EnemyStateMove>(m_nextState);
			state->Init(player->GetPos());
		}

		//�_���[�W���󂯂Ă����炻��ɉ�����State�ɕω�������
		if (m_hitEffect >= 0)
		{
			m_nextState = std::make_shared<EnemyStateHitAttack>(m_pEnemy, m_pScene);
			//�_���[�W�̎�ނ�ݒ肷��
		}
	}
}

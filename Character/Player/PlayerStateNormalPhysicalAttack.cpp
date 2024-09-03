#include "PlayerStateNormalPhysicalAttack.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"

namespace
{
	//�U�����o���Ă���Ƃ��̈ړ����x
	constexpr float kMoveSpeed = 120;
	//�G���߂��ɂ��锻��ɂȂ鋗��
	constexpr float kNearEnemyLength = 200;
	//�G�ɋ߂Â����Ԃ̍ő�
	constexpr int kGoEnemyTime = 60;
	//�U���̍ő�R���{��
	constexpr int kMaxCombo = 2;
}

void PlayerStateNormalPhysicalAttack::Update(MyEngine::Input input)
{
	m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
	return;

	//�ړ��x�N�g���̐���
	MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
	MyEngine::Vector3 velo = moveVec.Normalize() * kMoveSpeed;

	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			m_isAttackInput = true;
		}
	}

	//�G���߂��ɂ��邩�ǂ����𒲂ׂ�
	float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
	if (length < kNearEnemyLength)
	{
		m_isNearEnemy = true;
	}
	//�G���߂��ɂ�����ړ�����߂čU�����o��
	if (m_isNearEnemy)
	{
		//�ړ�����߂鏈��
		velo = MyEngine::Vector3(0, 0, 0);
		//TODO : �U�����s�����������
	}
	//��ԑJ�ڂ��ĉ��b������
	m_time++;
	//�G�Ɍ��������Ԃ����l�𒴂�����
	if (m_time > kGoEnemyTime)
	{
		//TODO : �U�����s�����������

		//�U�����͂��s���Ă�����
		if (m_isAttackInput)
		{
			//���Ԃ����Z�b�g��
			m_time = 0;
			//�U�������̒i�Ɉڍs����
			m_attackNum++;
			//�U���̍ő吔�𒴂��Ă�����
			if (m_attackNum > kMaxCombo)
			{
				//�A�C�h����Ԃɖ߂�
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}
			//�G���߂��ɂ���t���O��false�ɂ���
			m_isNearEnemy = false;
		}
		//�U�����͂�����Ă��Ȃ�������
		else
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}

	//�ړ��x�N�g��������
	m_pPlayer->SetVelo(velo);
	//��ŏ�Ԃ̕ω����Ȃ������獡�̏�Ԃ�Ԃ�
	m_nextState = shared_from_this();
}

int PlayerStateNormalPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->SetEffect(attack->GetHitEffect());

	return damage;
}

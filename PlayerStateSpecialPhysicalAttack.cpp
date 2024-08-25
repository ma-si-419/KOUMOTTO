#include "PlayerStateSpecialPhysicalAttack.h"
#include "PlayerStateIdle.h"


namespace
{
	//�U�����o���Ă���Ƃ��̈ړ����x
	constexpr float kMoveSpeed = 150;
	//�G���߂��ɂ��锻��ɂȂ鋗��
	constexpr float kNearEnemyLength = 200;
	//�G�ɋ߂Â����Ԃ̍ő�
	constexpr int kGoEnemyTime = 60;
}

void PlayerStateSpecialPhysicalAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//�o�ߎ��Ԃ��v������
	m_time++;

	//�ړ��x�N�g���̐���
	MyEngine::Vector3 moveVec = player->GetTargetPos() - player->GetPos();
	MyEngine::Vector3 velo = moveVec.Normalize() * kMoveSpeed;

	//�G���߂��ɂ��邩�ǂ����𒲂ׂ�
	float length = (player->GetTargetPos() - player->GetPos()).Length();
	//�G���߂��ɂ��邩�A�o�ߎ��Ԃ���莞�Ԃ𒴂�����
	if (length < kNearEnemyLength || m_time > kGoEnemyTime)
	{
		//�G���߂��ɂ���t���O�𗧂Ă�
		m_isNearEnemy = true;
	}
	//�G���߂��ɂ�����ړ�����߂čU�����o��
	if (m_isNearEnemy)
	{
		//�ړ�����߂鏈��
		velo = MyEngine::Vector3(0, 0, 0);
		//TODO : �U�����s�����������
		//�U���̏��������鎞�Ԃ��ւ炵�Ă���
		m_actionTime--;
		if (m_actionTime < 0)
		{
			//�U���̏��������鎞�Ԃ��I�������
			m_nextState = std::make_shared<PlayerStateIdle>();
		}
		
	}

	//�ړ��x�N�g��������
	player->SetVelo(velo);
	m_nextState = shared_from_this();
}

int PlayerStateSpecialPhysicalAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

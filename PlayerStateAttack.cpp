#include "PlayerStateAttack.h"
#include "PlayerStateIdle.h"
namespace
{
	//�C�e���o���Ă���Ƃ��̈ړ����x
	constexpr float kEnergyAttackMoveSpeed = 80.0f;
	//�U�����o���Ă���Ƃ��̈ړ����x
	constexpr float kPhysicalAttackMoveSpeed = 150;
	//�G���߂��ɂ��锻��ɂȂ鋗��
	constexpr float kNearEnemyLength = 200;
	//�G�ɋ߂Â����Ԃ̍ő�
	constexpr int kGoEnemyTime = 60;
	//�ʏ�U���̃R���{���͎�t����
	constexpr int kComboTime = 40;
	//�C�e�U���̍ő�R���{��
	constexpr int kEnergyComboMax = 3;
	//�i���U���̍ő�R���{��
	constexpr int kPhysicalComboMax = 2;
}

void PlayerStateAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
    DataManager::AttackInfo attackData = player->GetAttackData(m_attackId);
	//�o�ߎ��Ԃ��v������
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;
	//�X�e�B�b�N����
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//�ړ�����
	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

    //�C�e�U���̂ݏo���Ȃ���ړ��ł���
    if (attackData.attackTime = 0)
    {
		//�ړ����͂�����Ă���Ƃ�
		if (dir.sqLength() != 0)
		{
			dir = dir.Normalize();

			//Y���𒆐S�Ƃ�����]������̂�
			MyEngine::Vector3 rotationShaftPos = player->GetTargetPos();
			//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
			rotationShaftPos.y = player->GetPos().y;

			MyEngine::Vector3 toShaftPosVec = rotationShaftPos - player->GetPos();

			//��]���x(���ړ��̑���)
			float hMoveSpeed = 0;

			if (dir.x != 0.0f)
			{
				hMoveSpeed = (dir.x * kEnergyAttackMoveSpeed) / toShaftPosVec.Length();
			}

			player->SetRota(player->GetRota() + hMoveSpeed);

			//���E�ړ��͓G�̎��͂����

			//�G�̍��W����]�x���Q�Ƃ��A���̉�]�x�������玟�͂ǂ̍��W�ɂȂ邩�v�Z��
			//���݂̍��W���炻�̍��W�Ɍ������x�N�g�����쐬����
			velo.x = (rotationShaftPos.x + cosf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().x;
			velo.z = (rotationShaftPos.z + sinf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().z;

			//�㉺�ړ����͂���Ă�����
			if (input.IsPress(Game::InputId::kLb))
			{
				//�O����͂��㉺�̃x�N�g���ɕϊ�
				velo.y += dir.z * kEnergyAttackMoveSpeed;
			}
			//����Ă��Ȃ������ꍇ
			else
			{
				//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
				MyEngine::Vector3 toCenterVec = player->GetTargetPos() - player->GetPos();
				toCenterVec.y = 0;
				velo += toCenterVec.Normalize() * (dir.z * kEnergyAttackMoveSpeed);
			}
		}
    }
	//�i���U���������瑊��Ɍ������Ă���
	if (!attackData.isEnergy)
	{
		//�ړ��x�N�g���̐���
		MyEngine::Vector3 moveVec = player->GetTargetPos() - player->GetPos();
		velo = moveVec.Normalize() * kPhysicalAttackMoveSpeed;

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
				return;
			}

		}
	}
	//�ʏ�U���̃R���{�̓���
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (attackData.isEnergy && input.IsTrigger(Game::InputId::kX))
		{
			//���ԓ��ɍU�����͂����Ă���Ύ��i�̍U���Ɉڍs����t���O�𗧂Ă�
			m_isAttackInput = true;
		}
		//�i���U���������ꍇ
		if (!attackData.isEnergy && input.IsTrigger(Game::InputId::kB))
		{
			//���ԓ��ɍU�����͂����Ă���Ύ��i�̍U���Ɉڍs����t���O�𗧂Ă�
			m_isAttackInput = true;
		}
	}

	//���͑ҋ@���Ԃ𒴂�����
	if (m_time > kComboTime)
	{
		//�ʏ�U�����o��
		player->Attack(attackData.name);

		//�U���̓��͂�����Ă�����
		if (m_isAttackInput)
		{
			//���i�̍U���Ɉڍs����
			m_normalAttackNum++;
			//���܂������C�e�U�����ŏI�i��������
			if (attackData.isEnergy && m_normalAttackNum > kEnergyComboMax)
			{
				//�A�C�h����Ԃɖ߂�
				m_nextState = std::make_shared<PlayerStateIdle>();
				return;
			}
			//���܂������i���U�����ŏI�i��������
			else if (!attackData.isEnergy && m_normalAttackNum > kPhysicalComboMax)
			{
				//�A�C�h����Ԃɖ߂�
				m_nextState = std::make_shared<PlayerStateIdle>();
				return;
			}
			//�U���̓��͂����Z�b�g����
			m_isAttackInput = false;
		}
		//�U���̓��͂�����Ă��Ȃ�������
		else
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>();
			return;
		}
	}
	//�ړ��x�N�g��������
	player->SetVelo(velo);

	m_nextState = shared_from_this();
}

int PlayerStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
    return 0;
}
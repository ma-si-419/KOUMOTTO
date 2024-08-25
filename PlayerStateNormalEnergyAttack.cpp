#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateIdle.h"

namespace
{
	//���̍U���Ɉڂ�鎞��
	constexpr int kComboTime = 40;
	//�ړ����x
	constexpr float kMoveSpeed = 80;
	//�C�e�̃R���{�̐�
	constexpr int kComboMax = 3;
}

void PlayerStateNormalEnergyAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

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
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		MyEngine::Vector3 a = rotationShaftPos - player->GetPos();

		//m_rota = atan2f(a.z,a.x);

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
			velo.y += dir.z * kMoveSpeed;
		}
		//����Ă��Ȃ������ꍇ
		else
		{
			//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
			MyEngine::Vector3 toCenterVec = player->GetTargetPos() - player->GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
		}
	}
	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			m_attackNum++;
			if (m_attackNum > kComboMax)
			{
				m_attackNum = 0;
			}
			m_time = 0;
		}
	}

	//�o�ߎ��ԊǗ�
	m_time++;
	if (m_time < kComboTime)
	{
		m_nextState = shared_from_this();
	}
	else
	{
		m_nextState = std::make_shared<PlayerStateIdle>();
	}
	player->SetVelo(velo);
}

int PlayerStateNormalEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

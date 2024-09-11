#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateCharge.h"
#include "PlayerStateAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"


namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 100.0f;
	//�g���K�[����������
	constexpr int kTriggerReaction = 200;
	//�G�Ƃ̈�ԋ߂�����(�^��ɂ����Ȃ��悤��)
	constexpr int kShortestEnemyDistance = 1500;
	//�_�b�V������Ƃ��̈ړ����x�̔{��
	constexpr float kDashSpeedRate = 1.8f;
}

void PlayerStateMove::Init()
{
	m_pPlayer->ChangeAnim("MoveFront");
}

void PlayerStateMove::Update(MyEngine::Input input)
{
	//State�̕ω�
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->StopEffect();
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX, false);
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->StopEffect();
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->StopEffect();
			//State��Guard�ɕύX����
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
			state->Init();
			return;
		}
		//�`���[�W���͂���Ă�����
		if (input.IsPress(Game::InputId::kY))
		{
			//State��Charge�ɕύX����
			m_nextState = std::make_shared<PlayerStateCharge>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateCharge>(m_nextState);
			state->Init();
			return;
		}
	}
	//�K�E�Z�p���b�g���J���Ă���ꍇ
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttackId();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->StopEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->StopEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kB, true);
				return;
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->StopEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kX, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->StopEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
			}
		}
	}

	//��L�̏�ԑJ�ڂɓ��Ă͂܂�Ȃ�������
	//���̈ړ��������s��

	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//�ړ����͂�����Ă���Ƃ�
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();

		//Y���𒆐S�Ƃ�����]������̂�
		MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
		//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
		rotationShaftPos.y = m_pPlayer->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

		//�_�b�V���{�^����������Ă��邩
		bool isDash = input.IsPress(Game::InputId::kA);

		//�_�b�V���{�^����������Ă����王��p���L����
		m_pPlayer->SetUpFov(isDash);


		//��]���x(���ړ��̑���)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
			//�_�b�V���{�^���������Ă����牡�ړ��̑����Ƀ_�b�V���̔{����������
			if (isDash)
			{
				hMoveSpeed *= kDashSpeedRate;
			}
		}

		m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

		//���E�ړ��͓G�̎��͂����

		//�G�̍��W����]�x���Q�Ƃ��A���̉�]�x�������玟�͂ǂ̍��W�ɂȂ邩�v�Z��
		//���݂̍��W���炻�̍��W�Ɍ������x�N�g�����쐬����
		velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

		MyEngine::Vector3 zDirVec;

		//�㉺�ړ����͂���Ă�����
		if (input.GetTriggerInfo().left > kTriggerReaction)
		{
			//�O����͂��㉺�̃x�N�g���ɕϊ�
			zDirVec.y += dir.z * kMoveSpeed;
		}
		//����Ă��Ȃ������ꍇ
		else
		{
			//�G�ɋ߂���������͂����悤�ɂ���
			if (toShaftPosVec.Length() > kShortestEnemyDistance)
			{
				//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
				toShaftPosVec.y = 0;
				zDirVec += toShaftPosVec.Normalize() * (dir.z * kMoveSpeed);
			}
			else
			{
				//�O���͂����ړ��Ɍ����͂���]�̒��S���痣���x�N�g���ɕϊ�
				if (dir.z > 0)
				{
					hMoveSpeed = (zDirVec.z * kMoveSpeed) / toShaftPosVec.Length();
					m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);
					zDirVec.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
					zDirVec.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;
				}
				else if (dir.z < 0)
				{
					zDirVec += toShaftPosVec.Normalize() * (dir.z * kMoveSpeed);
				}
			}
			//�_�b�V���{�^����������Ă�����
			if (isDash)
			{
				zDirVec *= kDashSpeedRate;
				m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Dash"));
			}
			else
			{
				m_pPlayer->StopEffect();
			}
		}
		velo += zDirVec;

		//�K�E�Z�p���b�g���J����Ă��Ȃ�
		if (!input.IsPress(Game::InputId::kLb))
		{
			//����s���̓��͂����ꂽ��
			if (input.IsTrigger(Game::InputId::kA))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->StopEffect();
				//State��Dodge�ɕύX����
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
				//����̕�����ݒ肷��
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				state->Init(dir);
				return;
			}
		}
		m_pPlayer->PlayAnim();
		m_pPlayer->SetVelo(velo);

		//���ʂ̍��W
		MyEngine::Vector3 frontPos = m_pPlayer->GetPos() + velo;
		//�㉺�ړ����s���ۂ͓G�̕��������Ȃ��瓮���悤�ɂ���
		if (frontPos.y != m_pPlayer->GetPos().y)
		{
			frontPos = m_pPlayer->GetTargetPos();
		}

		//�ړ���̍��W�������悤�ɂ���
		m_pPlayer->SetModelFront(frontPos);

		//�����̃|�C���^��Ԃ�
		m_nextState = shared_from_this();
		return;
	}
	//�ړ����͂�����Ă��Ȃ��Ƃ���
	else
	{
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->StopEffect();
			//State��Dodge�ɕύX����
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			//�G�Ɍ������x�N�g�����쐬����
			MyEngine::Vector3 dir = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Normalize();
			state->Init(dir);
			return;
		}
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}

}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//�q�b�g�G�t�F�N�g��\������
	int effect = PlayEffekseer3DEffect(m_pPlayer->GetEffekseerData("Hit").first);
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	SetPosPlayingEffekseer3DEffect(effect, pos.x, pos.y, pos.z);

	return damage;
}
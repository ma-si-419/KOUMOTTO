#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"
#include "Player.h"


namespace
{
	constexpr float kDodgeSpeed = 200.0f;
	//����̎���
	constexpr int kDodgeTime = 10;
	//�G�Ƃ̍ŒZ����
	constexpr int kShortestEnemyDistance = 1500;
}
void PlayerStateDodge::Init(MyEngine::Vector3 dir)
{
	m_moveDir = dir;
	m_pPlayer->ChangeAnim("Move");
	m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Dodge"));
	m_pPlayer->SetUpFov(true);
}
void PlayerStateDodge::Update(MyEngine::Input input)
{
	//����p���L����
	m_pPlayer->SetUpFov(true);
	//�ړ��Ɠ�������
	MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	MyEngine::Vector3 velo(0, 0, 0);

	//��]���x(���ړ��̑���)
	float hMoveSpeed = 0;

	if (m_moveDir.x != 0.0f)
	{
		hMoveSpeed = (m_moveDir.x * kDodgeSpeed) / toShaftPosVec.Length();
	}

	m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

	velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
	velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

	//�G�ɋ߂���������͂����悤�ɂ���
	if (toShaftPosVec.Length() > kShortestEnemyDistance)
	{
		//�O����͂���]�̒��S�Ɍ������x�N�g���ɕϊ�
		toShaftPosVec.y = 0;
		velo += toShaftPosVec.Normalize() * (m_moveDir.z * kDodgeSpeed);
	}
	else
	{
		//�O���͂����ړ��Ɍ����͂���]�̒��S���痣���x�N�g���ɕϊ�
		if (m_moveDir.z > 0)
		{
			hMoveSpeed = (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);
			velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
			velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;
		}
		else if (m_moveDir.z < 0)
		{
			velo += toShaftPosVec.Normalize() * (m_moveDir.z * kDodgeSpeed);
		}
	}

	m_pPlayer->SetVelo(velo);

	m_time++;

	//����̎��Ԃ�����
	if (m_time < kDodgeTime)
	{
		//�����̃|�C���^��Ԃ�
		m_nextState = shared_from_this();
	}
	//����̎��Ԃ��I�������
	else
	{
		//�A�C�h����Ԃɖ߂�
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer,m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		m_pPlayer->StopEffect();
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�U���ɓ�����Ȃ��悤�ɂ���
	return 0;
}

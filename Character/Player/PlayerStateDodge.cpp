#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"
#include "Player.h"


namespace
{
	constexpr float kDodgeScale = 200.0f;
	//����̎���
	constexpr int kDodgeTime = 10;
}
void PlayerStateDodge::Init(MyEngine::Vector3 dir)
{
	m_moveDir = dir;
	m_pPlayer->ChangeAnim("Move");
}
void PlayerStateDodge::Update(MyEngine::Input input)
{
	//�ړ��Ɠ�������
	MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	//��]���x(���ړ��̑���)
	float hMoveSpeed = 0;

	if (m_moveDir.x != 0.0f)
	{
		hMoveSpeed = (m_moveDir.x * kDodgeScale) / toShaftPosVec.Length();
	}

	MyEngine::Vector3 a = rotationShaftPos - m_pPlayer->GetPos();

	//m_rota = atan2f(a.z,a.x);

	m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

	MyEngine::Vector3 velo(0, 0, 0);

	velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
	velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

	MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
	toCenterVec.y = 0;
	velo += toCenterVec.Normalize() * (m_moveDir.z * kDodgeScale);

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
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�U���ɓ�����Ȃ��悤�ɂ���
	return 0;
}

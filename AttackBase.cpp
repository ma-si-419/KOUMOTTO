#include "AttackBase.h"
#include "CapsuleColliderData.h"

namespace
{
}

AttackBase::AttackBase(ObjectTag tag):
	Collidable(tag,ColliderData::Kind::kCapsule),
	m_status(),
	m_dir(),
	m_isExist(true),
	m_lifeTime(0)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos)
{
	Collidable::Init(physics);
	m_rigidbody.SetPos(pos);
}

void AttackBase::SetStatus(Game::AttackInfo status,MyEngine::Vector3 target, MyEngine::Vector3 playerPos)
{
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	m_status = status;
	colData->m_radius = m_status.radius;
	m_dir = (target - playerPos).Normalize();
	colData->m_isMoveStartPos = status.isLaser;
	colData->m_startPos = m_rigidbody.GetPos();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);

	m_rigidbody.SetVelo(m_dir * m_status.speed);

	//���C�t�^�C�����J�E���g����
	m_lifeTime++;

	//���C�t�^�C�����ǔ����鎞�Ԃ�������
	if (m_lifeTime > m_status.trackStartTime && m_lifeTime < m_status.trackEndTime)
	{
		MyEngine::Vector3 toTargetDir = (targetPos - m_rigidbody.GetPos()).Normalize();
		//�����ϊ����}�ȃJ�[�u�����Ȃ��悤��
		//TODO:���������Ă����������^�[�Q�b�g�̕����ɏ������������Ă���		    
	}

	//���C�t�^�C��������𒴂����珈�����Ȃ��悤�ɂ���
	if (m_lifeTime > m_status.lifeTime)
	{
		m_isExist = false;
	}
}

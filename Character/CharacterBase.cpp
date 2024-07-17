#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0)
{
	m_modelHandle = MV1LoadModel(model);
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id)
{
	std::shared_ptr<AttackBase> ans;
	//�U���f�[�^�̎�ނ��擾
	if (m_attackData[id].isEnergy)
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kEnergyAttack);
	}
	else
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kPhysicalAttack);
	}
	//�U�����o�����W���쐬
	MyEngine::Vector3 toEnemyVec = m_targetPos - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toEnemyVec.Normalize() * m_attackData[id].radius;

	ans->Init(physics, attackPos);
	//�X�e�[�^�X�ݒ�
	ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos());

	return ans;
}

void CharacterBase::SetAttack(std::string id)
{
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[id].actionTime;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;
	//�o���Ă���U���̎�ނ��擾����
	m_attackId = id;
}

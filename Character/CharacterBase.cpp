#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0),
	m_isAttack(false)
{
	m_modelHandle = MV1LoadModel(model);
	//auto& coldata = std::dynamic_pointer_cast<CapsuleColliderData>;
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id,bool isPlayer)
{
	std::shared_ptr<AttackBase> ans;
	//�U���f�[�^�̎�ނ��擾
	if (isPlayer)
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	}
	else
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	}
	//�U�����o�����W���쐬
	MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	ans->Init(physics, attackPos);
	//�X�e�[�^�X�ݒ�
	ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(),m_status.atk);

	return ans;
}

void CharacterBase::SetAttack(std::string id)
{
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[id].actionTime;
	//�Z�̏���C�͕��A���݂̋C�͂����炷
	m_nowMp -= m_attackData[id].cost;
	//�U���̃^�[�Q�b�g���W�����b�N����
	m_attackTarget = m_targetPos;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;
	//�o���Ă���U���̎�ނ��擾����
	m_attackId = id;
	//�G���߂��ɂ��邩�ǂ�����false�ɂ���
	m_isNearTarget = false;
}

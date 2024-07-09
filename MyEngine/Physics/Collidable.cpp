#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "Physics.h"
#include <cassert>

Collidable::Collidable(ObjectTag tag, ColliderData::Kind colKind) :
	m_pColData(nullptr),
	m_tag(tag)
{
	m_pColData = CreateColliderData(colKind);
}

Collidable::~Collidable()
{
	//�R���C�_�[�f�[�^�������B
	if (m_pColData != nullptr)
	{
		delete m_pColData;
		m_pColData = nullptr;
	}
}

void Collidable::Init(Physics* physics)
{
	//�������o�^
	physics->Entry(this);
}

void Collidable::Final(Physics* physics)
{
	//����������
	physics->Exit(this);
}

ColliderData* Collidable::CreateColliderData(ColliderData::Kind kind)
{
	if (m_pColData != nullptr)
	{
		assert(0 && "colliderData�͂��łɍ���Ă��܂�");
		return m_pColData;
	}
	if (kind == ColliderData::Kind::kCapsule)
	{
		//�J�v�Z���R���C�_�[�̏�������
		return new CapsuleColliderData();
	}
	else if (kind == ColliderData::Kind::kSphere)
	{
		//�X�t�B�A�R���C�_�[�̏�������
		return new SphereColliderData();
	}
	else
	{
		return nullptr;
	}
}

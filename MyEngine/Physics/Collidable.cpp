#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "Physics.h"
#include <cassert>

Collidable::Collidable(ObjectTag tag, ColliderData::Kind colKind):
	m_colData(nullptr),
	m_tag(tag)
{
	m_colData = CreateColliderData(colKind);
}

Collidable::~Collidable()
{
	//�R���C�_�[�f�[�^�������B
	if (m_colData != nullptr)
	{
		delete m_colData;
		m_colData = nullptr;
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
	if (m_colData != nullptr)
	{
		assert(0 && "colliderData�͂��łɍ���Ă��܂�");
		return m_colData;
	}
	if (kind == ColliderData::Kind::kCapsule)
	{
		//�J�v�Z���R���C�_�[�̏�������
		m_colData = new CapsuleColliderData();
	}
	else if (kind == ColliderData::Kind::kSphere)
	{
		//�X�t�B�A�R���C�_�[�̏�������
		m_colData = new SphereColliderData();
	}
	return nullptr;
}

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
	//コライダーデータを消す。
	if (m_colData != nullptr)
	{
		delete m_colData;
		m_colData = nullptr;
	}
}

void Collidable::Init(Physics* physics)
{
	//物理情報登録
	physics->Entry(this);
}

void Collidable::Final(Physics* physics)
{
	//物理情報解除
	physics->Exit(this);
}

ColliderData* Collidable::CreateColliderData(ColliderData::Kind kind)
{
	if (m_colData != nullptr)
	{
		assert(0 && "colliderDataはすでに作られています");
		return m_colData;
	}
	if (kind == ColliderData::Kind::kCapsule)
	{
		//カプセルコライダーの情報を入れる
		m_colData = new CapsuleColliderData();
	}
	else if (kind == ColliderData::Kind::kSphere)
	{
		//スフィアコライダーの情報を入れる
		m_colData = new SphereColliderData();
	}
	return nullptr;
}

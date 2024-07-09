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
	//コライダーデータを消す。
	if (m_pColData != nullptr)
	{
		delete m_pColData;
		m_pColData = nullptr;
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
	if (m_pColData != nullptr)
	{
		assert(0 && "colliderDataはすでに作られています");
		return m_pColData;
	}
	if (kind == ColliderData::Kind::kCapsule)
	{
		//カプセルコライダーの情報を入れる
		return new CapsuleColliderData();
	}
	else if (kind == ColliderData::Kind::kSphere)
	{
		//スフィアコライダーの情報を入れる
		return new SphereColliderData();
	}
	else
	{
		return nullptr;
	}
}

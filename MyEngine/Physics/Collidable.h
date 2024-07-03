#pragma once
#include "ObjectTag.h"
#include "ColliderData.h"
#include "Rigidbody.h"
class Physics;
class Collidable abstract
{
public:
	Collidable(ObjectTag tag, ColliderData::Kind colKind);
	virtual ~Collidable();
	virtual void Init(Physics* physics);
	virtual void Final(Physics* physics);

	ObjectTag GetTag() const { return m_tag; }

protected:
	//�����f�[�^
	Rigidbody m_rigidbody;
	//�����蔻��f�[�^
	ColliderData* m_colData;
private:
	ColliderData* CreateColliderData(ColliderData::Kind kind);

	ObjectTag m_tag;

	MyEngine::Vector3 m_nextPos;
	//Physics��Collidable�����R�ɊǗ��ł���悤��
	friend Physics;
};


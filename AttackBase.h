#pragma once
#include "Collidable.h"
class AttackBase : public Collidable
{
public:
	struct AttackStatus
	{
		int atk = 0;
		float spd = 0;
		float rad = 0;
		MyEngine::Vector3 dir;
	};

	AttackBase(ObjectTag tag,ColliderData::Kind kind);
	virtual ~AttackBase();

	void Init(Physics* physics,MyEngine::Vector3 pos);
	void SetStatus(AttackStatus status);
	void Update();
	virtual void Draw() = 0;

	bool GetExist() { return m_isExist; }
protected:
	int m_atk;
	float m_spd;
	float m_radius;
	MyEngine::Vector3 m_dir;
	bool m_isExist;
	int m_lifeTime;
};


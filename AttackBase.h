#pragma once
#include "Vector3.h"
class AttackBase
{
public:
	AttackBase();
	virtual ~AttackBase();

	void Init(MyEngine::Vector3 pos);
	void Update();

private:
	MyEngine::Vector3 m_pos;
	MyEngine::Vector3 m_velo;
};


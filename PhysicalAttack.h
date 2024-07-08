#pragma once
#include "AttackBase.h"
class PhysicalAttack : public AttackBase
{
public:
	PhysicalAttack();
	virtual ~PhysicalAttack();

	virtual void Draw();
};


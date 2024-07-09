#pragma once
#include "AttackBase.h"
class NormalAttack : public AttackBase
{
public:
	NormalAttack(ObjectTag tag);
	~NormalAttack();

	virtual void Draw() {}
};


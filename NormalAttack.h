#pragma once
#include "AttackBase.h"
class NormalAttack : public AttackBase
{
public:
	NormalAttack(ObjectTag tag);
	virtual  ~NormalAttack();

	virtual void Draw() {}
};
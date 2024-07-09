#pragma once
#include "AttackBase.h"
class SpecialAttack : public AttackBase
{
public:
	SpecialAttack(ObjectTag tag);
	virtual ~SpecialAttack();

	virtual void Draw();
private:

};


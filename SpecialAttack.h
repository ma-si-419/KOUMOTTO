#pragma once
#include "AttackBase.h"
class SpecialAttack : public AttackBase
{
public:
	SpecialAttack(ObjectTag tag, ColliderData::Kind kind);
	virtual ~SpecialAttack();

	virtual void Draw();
};


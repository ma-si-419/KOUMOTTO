#pragma once
#include "CharacterBase.h"
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(Physics* physics);
	void Update();
	void Draw();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
private:

};

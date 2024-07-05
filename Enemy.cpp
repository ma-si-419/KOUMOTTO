#include "Enemy.h"

Enemy::Enemy() :
	CharacterBase("data/model/Player.mv1",ObjectTag::kEnemy)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Physics* physics)
{
	Collidable::Init(physics);
}

void Enemy::Update()
{
	m_rigidbody.SetPos(MyEngine::Vector3(100, 0, 100));
}

void Enemy::Draw()
{
}

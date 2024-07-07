#include "Enemy.h"
#include "SceneGame.h"
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

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	m_rigidbody.SetPos(MyEngine::Vector3(0, 0, 1000));
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Enemy::Draw()
{
	MV1DrawModel(m_modelHandle);
}

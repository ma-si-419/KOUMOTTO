#include "Enemy.h"
#include "SceneGame.h"
Enemy::Enemy() :
	CharacterBase("data/model/Enemy.mv1",ObjectTag::kEnemy)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(std::shared_ptr<Physics> physics)
{
	Collidable::Init(physics);
}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	m_rigidbody.SetPos(MyEngine::Vector3(1, 1, 1000));
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
}

void Enemy::Draw()
{
	DrawFormatString(0, 200, GetColor(255, 255, 255), "%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);

	MV1DrawModel(m_modelHandle);
}

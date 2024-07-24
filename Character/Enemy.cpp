#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
namespace
{
	//当たり判定の大きさ
	constexpr float kColScale = 100.0f;
	//初期位置
	const MyEngine::Vector3 kInitPos(0,0,1000);
}
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
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rigidbody.SetPos(kInitPos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());


}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{

	//debug.x++;
	debug.y += 0.01f;
	MyEngine::Vector3 pos(sinf(debug.x) * 100, sinf(debug.y) * 3000,1000.0f);

	m_rigidbody.SetPos(pos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//当たり判定の縦幅
	pos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = pos;

}

void Enemy::Draw()
{
	DrawFormatString(1300, 300, GetColor(0, 0, 0), "HP:%f\nMP:%f", m_nowHp, m_nowMp);

	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	//プレイヤーの攻撃に当たった時
	if (collider->GetTag() == ObjectTag::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = attack->GetDamage() - static_cast<int>(m_status.def);
		if (damage < 0)
		{
			damage = 2;
		}
		m_nowHp -= damage;
		if (m_nowHp < 0)
		{
			m_nowHp = 0;
		}
	}
}

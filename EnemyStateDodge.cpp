#include "EnemyStateDodge.h"
namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 10;
	//移動速度
	constexpr float kMoveSpeed = 300.0f;
	//動きの方向の数
	constexpr int kMoveDirNum = 8;
	//動きの方向の数の半分
	constexpr int kMoveDirNumHalf = kMoveDirNum * 0.5;
}
void EnemyStateDodge::Init()
{
	MyEngine::Vector3 moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf).Normalize();
	m_velo = moveDir * kMoveSpeed;
}
void EnemyStateDodge::Update()
{
	//経過時間
	m_time++;
	//移動
	m_pEnemy->SetVelo(m_velo);
	//時間になったらすぐにこのStateを抜ける
	if (m_time > kShortestTime)
	{
		m_isChangeState = true;
	}
}

int EnemyStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//すべて0で返す
	return 0;
}

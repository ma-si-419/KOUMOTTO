#include "EnemyStateMove.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 120;
	//上下移動を優先するY座標のずれの大きさ
	constexpr float kYGapScale = 300;
}

void EnemyStateMove::Init(MyEngine::Vector3 playerPos)
{
	//playerの座標に合わせて動く方向を決定する
	
	//Y座標がずれていたらできるだけ合わせるように動く
	float gapPosY = abs(m_pEnemy->GetPos().y - playerPos.y);
	//Y座標のずれが大きかったら上下移動するようにする
	if (gapPosY > kYGapScale)
	{
		m_isMoveVertical = true;
	}
}

void EnemyStateMove::Update()
{

}

int EnemyStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

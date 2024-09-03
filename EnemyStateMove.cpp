#include "EnemyStateMove.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 120;
	//�㉺�ړ���D�悷��Y���W�̂���̑傫��
	constexpr float kYGapScale = 300;
}

void EnemyStateMove::Init(MyEngine::Vector3 playerPos)
{
	//player�̍��W�ɍ��킹�ē������������肷��
	
	//Y���W������Ă�����ł��邾�����킹��悤�ɓ���
	float gapPosY = abs(m_pEnemy->GetPos().y - playerPos.y);
	//Y���W�̂��ꂪ�傫��������㉺�ړ�����悤�ɂ���
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

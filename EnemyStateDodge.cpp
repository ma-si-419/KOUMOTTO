#include "EnemyStateDodge.h"
namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 10;
	//�ړ����x
	constexpr float kMoveSpeed = 300.0f;
	//�����̕����̐�
	constexpr int kMoveDirNum = 8;
	//�����̕����̐��̔���
	constexpr int kMoveDirNumHalf = kMoveDirNum * 0.5;
}
void EnemyStateDodge::Init()
{
	MyEngine::Vector3 moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf).Normalize();
	m_velo = moveDir * kMoveSpeed;
}
void EnemyStateDodge::Update()
{
	//�o�ߎ���
	m_time++;
	//�ړ�
	m_pEnemy->SetVelo(m_velo);
	//���ԂɂȂ����炷���ɂ���State�𔲂���
	if (m_time > kShortestTime)
	{
		m_isChangeState = true;
	}
}

int EnemyStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//���ׂ�0�ŕԂ�
	return 0;
}

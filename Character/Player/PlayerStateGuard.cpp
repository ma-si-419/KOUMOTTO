#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
namespace
{
	//�_���[�W�̌y����
	constexpr float kDamageCutRate = 0.25f;
}
void PlayerStateGuard::Update(MyEngine::Input input)
{
	//�G�̕����������悤�ɂ���
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//�ړ����Ȃ��悤�ɂ���
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	//��Ԃ̕ω�
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	else
	{
		//���̏�Ԃɍ��킹���A�j���[�V�����ɕω�������
		m_pPlayer->ChangeAnim("Idle");
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		return;
	}

}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����J�b�g���ĕԂ�
	damage = attack->GetDamage() * kDamageCutRate;

	return damage;
}

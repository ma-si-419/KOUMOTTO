#include "PlayerStateCharge.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"

namespace
{
	//1�t���[���Œ��܂�܂��̗�
	constexpr float kAddMpPoint = 1.0f;
}

void PlayerStateCharge::Init()
{
    m_pPlayer->ChangeAnim("Guard");
}

void PlayerStateCharge::Update(MyEngine::Input input)
{
	//�G�̕����������悤�ɂ���
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//�ړ����Ȃ��悤�ɂ���
	m_pPlayer->SetVelo(MyEngine::Vector3(0, 0, 0));
	//�A�j���[�V�����𓮂���
	m_pPlayer->PlayAnim();
	//��Ԃ̕ω�
	if (input.IsPress(Game::InputId::kY))
	{
		m_pPlayer->AddMp(kAddMpPoint);
		m_nextState = shared_from_this();
		return;
	}
	//�`���[�W���͂�����Ă��Ȃ����
	else
	{
		//��Ԃ�ω�������
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
}

int PlayerStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);

	return damage;
}

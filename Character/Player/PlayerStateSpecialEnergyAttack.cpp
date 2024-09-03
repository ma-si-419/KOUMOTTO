#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"

void PlayerStateSpecialEnergyAttack::Update(MyEngine::Input input)
{
	//TODO : �U�����o���������쐬����

	//�J�ڂ��Ă���̎��Ԃ��v������
	m_actionTime--;
	//�ݒ肵�����ԂɂȂ�����
	if (m_actionTime < 0)
	{
		//��Ԃ����ɖ߂�
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		return;
	}
	//�ړ����Ȃ�
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));

	//��ŏ�Ԃ��ω����Ȃ������玩�g�̃|�C���^��Ԃ�
	m_nextState = shared_from_this();

}

int PlayerStateSpecialEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();

	return damage;
}

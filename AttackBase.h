#pragma once
#include "Collidable.h"
#include "Game.h"
#include "DataManager.h"
class AttackBase : public Collidable
{
public:
	AttackBase(ObjectTag tag);
	virtual ~AttackBase();

	void Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos, int effekseerHandle);
	void SetStatus(DataManager::AttackInfo status, MyEngine::Vector3 target, MyEngine::Vector3 playerPos, float power);
	void Update(MyEngine::Vector3 targetPos);
	void Draw() {};

	//�_���[�W��Ԃ�
	int GetDamage() { return m_status.damage; }
	//�X�^���_���[�W��Ԃ�
	int GetStanDamage() { return m_status.stanDamage; }
	//���W��Ԃ�
	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	//�����ɓ����������̏���
	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//�����������̏�����Ԃ�
	int GetHitEffect() { return m_status.hitEffect; }

	//���C�t�^�C����ݒ肷��
	void SetAttackTime(int time) { m_status.lifeTime = time; };

	void Final(std::shared_ptr<Physics> physics);
	bool GetIsExist() { return m_isExist; }

	void SetLeaveEffect() { m_isLeaveEffect = true; }
private:
	/*�X�e�[�^�X*/
	DataManager::AttackInfo m_status;
	//�U���̕���
	MyEngine::Vector3 m_dir;
	//���݂��Ă��邩
	bool m_isExist;
	//���݂�������
	int m_lifeTime;
	//���������̓G�Ƃ̋���
	float m_targetLength;
	//�ړ���������
	float m_moveLength;
	//�G�t�F�N�g�̃n���h��
	int m_effectHandle;
	//�v���C���̃G�t�F�N�g�̃n���h��
	int m_playEffectHandle;
	//�����蔻�肪��������ɃG�t�F�N�g���c�����ǂ���
	bool m_isLeaveEffect;
};


#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "Game.h"
#include <map>
#include <string>

class AttackBase;
class Physics;
class CharacterBase : public Collidable
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CharacterBase(const TCHAR* graph,ObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(std::shared_ptr<Physics> physics) = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

	//�ő�̗͂�Ԃ�
	int GetMaxHp() { return m_status.hp; }
	//���݂̗̑͂�Ԃ�
	int GetNowHp() { return m_nowHp; }
	//���b�N�I�����Ă���G�̍��W��ݒ肷��
	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }
	//�U���̏���ݒ肷��
	void SetAttackData(std::map<std::string, Game::AttackInfo> data) { m_attackData = data; }
	//�����Ȃ����Ԃ�ݒ肷��
	void SetStanTime(int time) { m_stanTime = time; }
	//�U���ɕK�v�ȋC�͗ʂ��擾����
	int GetAttackCost(std::string Id) { return m_attackData[Id].cost; }

protected:
	enum class State
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kCharge,
		kBoostAttack,
		kSuperArmor,
		kGuard,
		kDown,
		kHitNormalAttack,
		kHitSpecialAttack
	};
	struct Status
	{
		float hp = 100000;
		float mp = 1000;
		float atk = 100.0f;
		float def = 100.0f;
	};

	//���f���n���h��
	int m_modelHandle;
	//�o�^����Physics�������Ă���
	std::shared_ptr<Physics> m_pPhysics;
	//TODO:���̕ϐ��A�O���f�[�^��
	
	//�U���̃f�[�^
	std::map<std::string,Game::AttackInfo> m_attackData;

	Status m_status;
	//�̗�
	float m_nowHp;
	//�C��
	float m_nowMp;
	//�����Ȃ�����
	int m_stanTime;
	//�U�����o���Ă��邩�ǂ���
	bool m_isAttack;
	//�o���Ă���U��
	std::string m_attackId;
	//�U�����o�����^�C�~���O�̓G�̍��W
	MyEngine::Vector3 m_attackTarget;
	//���b�N�I�����Ă��鑊��̍��W
	MyEngine::Vector3 m_targetPos;
	/// <summary>
	/// �U�����쐬����
	/// </summary>
	/// <param name="id">�U����ID</param>
	/// <returns>�U���̃|�C���^</returns>
	std::shared_ptr<AttackBase> CreateAttack(std::shared_ptr<Physics> physics,std::string id,bool isPlayer);

	//�U�����o���Ă����Ԃɕω�������
	void SetAttack(std::string id);
};
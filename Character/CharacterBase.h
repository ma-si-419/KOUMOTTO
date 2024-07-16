#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "Game.h"
#include <map>
#include <string>


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

	void SetAttackData(std::map<std::string, Game::AttackInfo> data) { m_attackData = data; }

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
		int hp = 0;
		int mp = 1000;
		float atk = 0;
		float def = 0;
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
	int m_nowHp;
	//�C��
	int m_nowMp;
};
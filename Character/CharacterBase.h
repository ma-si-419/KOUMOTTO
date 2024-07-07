#pragma once
#include "DxLib.h"
#include "Collidable.h"
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
	virtual void Init(Physics* physics) = 0;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

protected:
	//���f���n���h��
	int m_modelHandle;
	//�o�^����Physics�������Ă���
	Physics* m_pPhysics;
	//TODO:���̕ϐ��A�O���f�[�^��
	//�̗�
	int m_hp;
	//�C��
	int m_mp;
};
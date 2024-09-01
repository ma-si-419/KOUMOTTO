#pragma once
#include "CharacterBase.h"
#include "Input.h"
class SceneGame;
class PlayerStateBase;
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input);
	void Draw();

	float GetRota() { return m_rota; }

	void SetRota(float rota) { m_rota = rota; }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	MyEngine::Vector3 GetTargetPos() { return m_targetPos; }

	DataManager::AttackInfo GetAttackData(std::string id) { return m_attackData[id]; }
	
	bool GetIsOpenSpecialPallet() { return m_isOpenSpecialPallet; }

	void Attack(std::string id);

	/// <summary>
	/// �U���̎�ނ��擾����
	/// </summary>
	/// <param name="attackId">���ׂ����U����ID</param>
	/// <returns>true:Energy,false:Physical</returns>
	bool GetAttackKind(std::string attackId);

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	std::map<std::string, std::string> GetSetSpecialAttack() { return m_setSpecialAttack; };

	std::map<std::string, std::string> GetSetSpecialAttackName();

private:

	/// <summary>
	/// �ړ������̌v�Z������
	/// </summary>
	/// <param name="velo">�ړ��x�N�g��</param>
	/// <param name="input">����</param>
	MyEngine::Vector3 Move(MyEngine::Vector3 velo, MyEngine::Input input);

	/// <summary>
	/// �U���֌W����̏������s��
	/// </summary>
	/// <param name="input">����</param>
	void Attack(MyEngine::Input input);

	float m_rota;

	//�Ō�ɍU�����o���Ă���̎��Ԃ��J�E���g����
	int m_lastAttackTime;
	//�O�̃t���[���̓��͂�ۑ����Ă���
	MyEngine::Vector3 m_lastInput;
	//�K�E�Z�p���b�g���J���Ă��邩�ǂ���
	bool m_isOpenSpecialPallet;

	void ChangeState(std::shared_ptr<PlayerStateBase> state);

	void SetSpecialAttack();

	//�ݒ肵�Ă���Z
	std::map<std::string, std::string> m_setSpecialAttack;

	//State�p�^�[��
	std::shared_ptr<PlayerStateBase> m_pState;
};


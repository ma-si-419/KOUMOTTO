#pragma once
#include "CharacterBase.h"
#include "Input.h"
#include <memory>
class SceneGame;
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(Physics* physics);
	void Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input);
	void Draw();

	//���b�N�I�����Ă���G�̍��W��ݒ肷��
	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
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
	void Attack(std::shared_ptr<SceneGame> scene,MyEngine::Input input);

	float m_rota;

	//���b�N�I�����Ă���G�̍��W
	MyEngine::Vector3 m_targetPos;

};


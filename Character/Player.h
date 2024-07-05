#pragma once
#include "CharacterBase.h"
#include "Input.h"
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(Physics* physics);
	void Update(MyEngine::Input input);
	void Draw();

	/// <summary>
	/// �J�����̕�����ݒ肷��
	/// </summary>
	/// <param name="dir">�J�����̕���</param>
	void SetCameraDir(MyEngine::Vector3 dir) { m_cameraDir = dir; }
	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
private:
	/// <summary>
	/// �ړ������̌v�Z������
	/// </summary>
	/// <param name="velo">�ړ��x�N�g��</param>
	/// <param name="input">����</param>
	MyEngine::Vector3 Move(MyEngine::Vector3 velo, MyEngine::Input input);

	//�J�����̕���
	MyEngine::Vector3 m_cameraDir;
};


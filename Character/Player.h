#pragma once
#include "CharacterBase.h"
#include "Input.h"
class SceneGame;
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Physics> physics);
	void Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input);
	void Draw();

	float GetRota() { return m_rota; }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;
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


};


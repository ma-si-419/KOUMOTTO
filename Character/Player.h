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
	/// 移動処理の計算をする
	/// </summary>
	/// <param name="velo">移動ベクトル</param>
	/// <param name="input">入力</param>
	MyEngine::Vector3 Move(MyEngine::Vector3 velo, MyEngine::Input input);

	/// <summary>
	/// 攻撃関係操作の処理を行う
	/// </summary>
	/// <param name="input">入力</param>
	void Attack(MyEngine::Input input);

	float m_rota;

	//最後に攻撃を出してからの時間をカウントする
	int m_lastAttackTime;
	//前のフレームの入力を保存しておく
	MyEngine::Vector3 m_lastInput;
};


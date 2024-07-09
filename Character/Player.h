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

	//ロックオンしている敵の座標を設定する
	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
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
	void Attack(std::shared_ptr<SceneGame> scene,MyEngine::Input input);

	float m_rota;

	//ロックオンしている敵の座標
	MyEngine::Vector3 m_targetPos;

};


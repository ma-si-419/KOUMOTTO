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
	/// カメラの方向を設定する
	/// </summary>
	/// <param name="dir">カメラの方向</param>
	void SetCameraDir(MyEngine::Vector3 dir) { m_cameraDir = dir; }
	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
private:
	/// <summary>
	/// 移動処理の計算をする
	/// </summary>
	/// <param name="velo">移動ベクトル</param>
	/// <param name="input">入力</param>
	MyEngine::Vector3 Move(MyEngine::Vector3 velo, MyEngine::Input input);

	//カメラの方向
	MyEngine::Vector3 m_cameraDir;
};


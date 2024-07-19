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
	/// ˆÚ“®ˆ—‚ÌŒvZ‚ğ‚·‚é
	/// </summary>
	/// <param name="velo">ˆÚ“®ƒxƒNƒgƒ‹</param>
	/// <param name="input">“ü—Í</param>
	MyEngine::Vector3 Move(MyEngine::Vector3 velo, MyEngine::Input input);

	/// <summary>
	/// UŒ‚ŠÖŒW‘€ì‚Ìˆ—‚ğs‚¤
	/// </summary>
	/// <param name="input">“ü—Í</param>
	void Attack(std::shared_ptr<SceneGame> scene,MyEngine::Input input);

	float m_rota;


};


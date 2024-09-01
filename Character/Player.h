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
	/// 攻撃の種類を取得する
	/// </summary>
	/// <param name="attackId">調べたい攻撃のID</param>
	/// <returns>true:Energy,false:Physical</returns>
	bool GetAttackKind(std::string attackId);

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	std::map<std::string, std::string> GetSetSpecialAttack() { return m_setSpecialAttack; };

	std::map<std::string, std::string> GetSetSpecialAttackName();

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
	//必殺技パレットを開いているかどうか
	bool m_isOpenSpecialPallet;

	void ChangeState(std::shared_ptr<PlayerStateBase> state);

	void SetSpecialAttack();

	//設定している技
	std::map<std::string, std::string> m_setSpecialAttack;

	//Stateパターン
	std::shared_ptr<PlayerStateBase> m_pState;
};


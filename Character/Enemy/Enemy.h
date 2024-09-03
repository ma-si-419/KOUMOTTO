#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"
#include <memory>
class EnemyStateBase;
class SceneGame;
class Ui;
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene);
	void Draw();

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	void SetUi(std::shared_ptr<Ui> pUi);

	void InitAiState(std::shared_ptr<SceneGame> scene);

	void SetAiData(std::map<std::string, std::vector<int>> data) { m_aiStateData = data; }

	std::map<std::string, std::vector<int>> GetAiData() { return m_aiStateData; }

	//スタンゲージが今全体の何割あるかを返す
	float GetStanPointRate();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	//Stateの更新
	void StateUpdate(std::shared_ptr<Player> player);
private:
	//スタンゲージ
	float m_stanPoint;
	//最後に攻撃を受けてから立った時間
	int m_lastHitDamageTime;
	//間隔を開けずに攻撃を受けた数
	int m_comboCount;
	//受けたダメージを表示するためにUi管理クラスの参照を持つ
	std::shared_ptr<Ui> m_pUi;
	//Stateパターン
	std::shared_ptr<EnemyStateBase> m_pState;
	//敵のStateによって動きの確率を変化させる
	std::map<std::string, std::vector<int>> m_aiStateData;
};

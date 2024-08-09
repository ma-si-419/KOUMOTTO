#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "DataManager.h"
#include "Game.h"
#include <map>
#include <string>

class AttackBase;
class Physics;
class CharacterBase : public Collidable
{
public:
	struct Status
	{
		float hp = 30000;
		float mp = 300;
		float atk = 100;
		float def = 100;
	};
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterBase(const TCHAR* graph,ObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(std::shared_ptr<Physics> physics) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

	//最大体力を返す
	Status GetStatus() { return m_status; }
	//現在の体力を返す
	float GetNowHp() { return m_nowHp; }
	//現在の気力量を返す
	float GetNowMp() { return m_nowMp; }
	//ロックオンしている敵の座標を設定する
	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }
	//攻撃の情報を設定する
	void SetAttackData(std::map<std::string, DataManager::AttackInfo> data) { m_attackData = data; }
	//動けない時間を設定する
	void SetStanTime(int time) { m_stanTime = time; }
	//攻撃に必要な気力量を取得する
	int GetAttackCost(std::string Id) { return m_attackData[Id].cost; }

protected:
	enum class State
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kCharge,
		kBoostAttack,
		kSuperArmor,
		kGuard,
		kDown,
		kHitLightAttack,
		kHitHeavyAttack
	};
	

	//モデルハンドル
	int m_modelHandle;
	//登録したPhysicsを持っておく
	std::shared_ptr<Physics> m_pPhysics;
	//TODO:下の変数、外部データ化
	
	//攻撃のデータ
	std::map<std::string,DataManager::AttackInfo> m_attackData;
	//基本的なステータス
	Status m_status;
	//現在の体力
	float m_nowHp;
	//現在の気力
	float m_nowMp;
	//動けない時間
	int m_stanTime;
	//攻撃を出しているかどうか
	bool m_isAttack;
	//ターゲットが近くにいるかどうか
	bool m_isNearTarget;
	//出している攻撃
	std::string m_attackId;
	//攻撃を出したタイミングの敵の座標
	MyEngine::Vector3 m_attackTarget;
	//ロックオンしている相手の座標
	MyEngine::Vector3 m_targetPos;
	/// <summary>
	/// 攻撃を作成する
	/// </summary>
	/// <param name="id">攻撃のID</param>
	/// <returns>攻撃のポインタ</returns>
	std::shared_ptr<AttackBase> CreateAttack(std::shared_ptr<Physics> physics,std::string id,bool isPlayer);

	//アニメーションを変化させる
	void ChangeAnim(int animNum);

	//必殺技を出している状態に変化させる
	void SetSpecialAttack(std::string id);
	//通常攻撃を出してる状態に変化させる
	void SetNormalAttack(bool isPhysical,int time);
};
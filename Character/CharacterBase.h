#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "Game.h"
#include <map>
#include <string>


class Physics;
class CharacterBase : public Collidable
{
public:
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

	void SetAttackData(std::map<std::string, Game::AttackInfo> data) { m_attackData = data; }

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
		kHitNormalAttack,
		kHitSpecialAttack
	};
	struct Status
	{
		int hp = 0;
		int mp = 1000;
		float atk = 0;
		float def = 0;
	};

	//モデルハンドル
	int m_modelHandle;
	//登録したPhysicsを持っておく
	std::shared_ptr<Physics> m_pPhysics;
	//TODO:下の変数、外部データ化
	
	//攻撃のデータ
	std::map<std::string,Game::AttackInfo> m_attackData;

	Status m_status;
	//体力
	int m_nowHp;
	//気力
	int m_nowMp;
};
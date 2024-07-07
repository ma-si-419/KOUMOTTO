#pragma once
#include "DxLib.h"
#include "Collidable.h"
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
	virtual void Init(Physics* physics) = 0;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

protected:
	//モデルハンドル
	int m_modelHandle;
	//登録したPhysicsを持っておく
	Physics* m_pPhysics;
	//TODO:下の変数、外部データ化
	//体力
	int m_hp;
	//気力
	int m_mp;
};
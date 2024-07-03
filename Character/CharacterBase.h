#pragma once
#include "DxLib.h"
#include "Collidable.h"
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
	virtual void Init(Physics* physics) {};

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;

protected:
	//モデルハンドル
	int m_modelHandle;
};
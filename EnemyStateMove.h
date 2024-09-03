#pragma once
#include "EnemyStateBase.h"
class EnemyStateMove : public EnemyStateBase
{
public:
	EnemyStateMove(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//どの行動をするかを決める
	void Init(MyEngine::Vector3 playerPos);
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	enum class MoveKind
	{
		kFront,//プレイヤーに向かっていく
		kBack,//プレイヤーから離れる
		kRight,//右方向に移動
		kLeft//左方向に移動
	};
	MoveKind m_moveKind = MoveKind::kFront;

	//上下移動するか
	bool m_isMoveVertical = false;
};
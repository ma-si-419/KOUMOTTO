#pragma once
#include "Enemy.h"
#include "SceneGame.h"
#include "PlayerStateBase.h"

class EnemyStateBase
{
public:

	enum class EnemyStateKind
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kAttack,
		kGuard,
		kCharge,
		kHitAttack
	};
	EnemyStateBase(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) { m_pEnemy = enemy; m_pScene = scene; }
	//状況を把握し続ける更新
	void CheckSituation(PlayerStateBase::PlayerStateKind playerState);
	//プレイヤーのStateを見て次の動きを決定する
	virtual void Update() abstract;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//次移行するStateを保存する
	std::shared_ptr<PlayerStateBase> m_nextState;
	//エネミーのポインターを持っておく
	std::shared_ptr<Enemy> m_pEnemy;
	//ゲームシーンのポインターを持っておく
	std::shared_ptr<SceneGame> m_pScene;
};


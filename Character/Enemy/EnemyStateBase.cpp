#include "EnemyStateBase.h"
#include "EnemyStateIdle.h"
#include "EnemyStateAttack.h"
#include "EnemyStateCharge.h"
#include "EnemyStateDash.h"
#include "EnemyStateDodge.h"
#include "EnemyStateGuard.h"
#include "EnemyStateMove.h"
#include "EnemyStateHitAttack.h"


void EnemyStateBase::CheckSituation(PlayerStateBase::PlayerStateKind playerState)
{

	//プレイヤーのStateによって使うデータを変える
	std::string data;
	if (playerState == PlayerStateBase::PlayerStateKind::kIdle)
	{
		data = "Idle";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kMove)
	{
		data = "Move";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kDash)
	{
		data = "Dash";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kDodge)
	{
		data = "Dodge";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kAttack)
	{
		data = "Attack";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kGuard)
	{
		data = "Guard";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kCharge)
	{
		data = "Charge";
	}
	else if (playerState == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		data = "HitAttack";
	}

	//すべての確率を足してランダムの範囲を決定する
	int randRange = 0;

	//プレイヤーの状態を見て確率を参照する
	for (auto item : m_aiData[data])
	{
		randRange += item;
	}
	
	//ランダムの範囲が決まったらランダムで計算する
	int ans = GetRand(randRange);

	//結果がわかったらそれに対応したStateを探す
	int stateIndex = 0;

	for (auto item : m_aiData[data])
	{	
		ans -= item;
		if (ans < 0)
		{
			break;
		}
		stateIndex++;
	}
	
	//常に次の行動を更新し続け
	if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kIdle)
	{
		m_nextState = std::make_shared<EnemyStateIdle>(m_pEnemy,m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kAttack)
	{
		m_nextState = std::make_shared<EnemyStateAttack>(m_pEnemy, m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kDash)
	{
		m_nextState = std::make_shared<EnemyStateDash>(m_pEnemy, m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kDodge)
	{
		m_nextState = std::make_shared<EnemyStateDodge>(m_pEnemy, m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kCharge)
	{
		m_nextState = std::make_shared<EnemyStateCharge>(m_pEnemy, m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kGuard)
	{
		m_nextState = std::make_shared<EnemyStateGuard>(m_pEnemy, m_pScene);
	}
	else if (static_cast<EnemyStateKind>(stateIndex) == EnemyStateKind::kMove)
	{
		m_nextState = std::make_shared<EnemyStateMove>(m_pEnemy, m_pScene);
	}

	//ダメージを受けていたらそれに応じたStateに変化させる
	if (m_hitEffect == 0)
	{
		m_nextState = std::make_shared<EnemyStateHitAttack>(m_pEnemy, m_pScene);
	}
}

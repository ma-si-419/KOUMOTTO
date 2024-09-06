#include "PlayerStateCharge.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"


namespace
{
	//1フレームで貯まるまその量
	constexpr float kAddMpPoint = 1.0f;
}

void PlayerStateCharge::Init()
{
	m_pPlayer->SetPlayEffect(m_pPlayer->GetEffekseerData("Charge"));
    m_pPlayer->ChangeAnim("Guard");
}

void PlayerStateCharge::Update(MyEngine::Input input)
{
	//敵の方向を向くようにする
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//移動しないようにする
	m_pPlayer->SetVelo(MyEngine::Vector3(0, 0, 0));
	//アニメーションを動かす
	m_pPlayer->PlayAnim();
	//状態の変化
	if (input.IsPress(Game::InputId::kY))
	{
		m_pPlayer->AddMp(kAddMpPoint);
		m_nextState = shared_from_this();
		return;
	}
	//チャージ入力がされていなければ
	else
	{
		//状態を変化させる
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		m_pPlayer->StopEffect();
		return;
	}
}

int PlayerStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);

	return damage;
}

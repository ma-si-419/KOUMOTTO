#include "PlayerStateSpecialEnergyAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"

void PlayerStateSpecialEnergyAttack::Update(MyEngine::Input input)
{
	//TODO : 攻撃を出す処理を作成する

	//遷移してからの時間を計測する
	m_actionTime--;
	//設定した時間になったら
	if (m_actionTime < 0)
	{
		//状態を元に戻す
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
		return;
	}
	//移動しない
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));

	//上で状態が変化しなかったら自身のポインタを返す
	m_nextState = shared_from_this();

}

int PlayerStateSpecialEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();

	return damage;
}

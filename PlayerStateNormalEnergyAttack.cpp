#include "PlayerStateNormalEnergyAttack.h"
#include "PlayerStateIdle.h"

namespace
{
	//次の攻撃に移れる時間
	constexpr int kComboTime = 40;
	//移動速度
	constexpr float kMoveSpeed = 80;
	//気弾のコンボの数
	constexpr int kComboMax = 3;
}

void PlayerStateNormalEnergyAttack::Update(std::shared_ptr<Player> player, MyEngine::Input input)
{
	//移動ベクトル
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//移動入力がされているとき
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();

		//Y軸を中心とした回転をするので
		MyEngine::Vector3 rotationShaftPos = player->GetTargetPos();
		//Y座標が関係しないようにプレイヤーと同じ座標にする
		rotationShaftPos.y = player->GetPos().y;

		MyEngine::Vector3 toShaftPosVec = rotationShaftPos - player->GetPos();

		//回転速度(横移動の速さ)
		float hMoveSpeed = 0;

		if (dir.x != 0.0f)
		{
			hMoveSpeed = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		}

		DrawFormatString(200, 0, GetColor(255, 255, 255), "%f", hMoveSpeed);

		MyEngine::Vector3 a = rotationShaftPos - player->GetPos();

		//m_rota = atan2f(a.z,a.x);

		player->SetRota(player->GetRota() + hMoveSpeed);

		//左右移動は敵の周囲を回る

		//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
		//現在の座標からその座標に向かうベクトルを作成する
		velo.x = (rotationShaftPos.x + cosf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().x;
		velo.z = (rotationShaftPos.z + sinf(player->GetRota()) * toShaftPosVec.Length()) - player->GetPos().z;

		//上下移動入力されていたら
		if (input.IsPress(Game::InputId::kLb))
		{
			//前後入力を上下のベクトルに変換
			velo.y += dir.z * kMoveSpeed;
		}
		//されていなかった場合
		else
		{
			//前後入力を回転の中心に向かうベクトルに変換
			MyEngine::Vector3 toCenterVec = player->GetTargetPos() - player->GetPos();
			toCenterVec.y = 0;
			velo += toCenterVec.Normalize() * (dir.z * kMoveSpeed);
		}
	}
	//必殺技パレットを開いていないとき
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			m_attackNum++;
			if (m_attackNum > kComboMax)
			{
				m_attackNum = 0;
			}
			m_time = 0;
		}
	}

	//経過時間管理
	m_time++;
	if (m_time < kComboTime)
	{
		m_nextState = shared_from_this();
	}
	else
	{
		m_nextState = std::make_shared<PlayerStateIdle>();
	}
	player->SetVelo(velo);
}

int PlayerStateNormalEnergyAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	return 0;
}

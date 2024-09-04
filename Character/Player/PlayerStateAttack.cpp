#include "PlayerStateAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"
namespace
{
	//気弾を出しているときの移動速度
	constexpr float kEnergyAttackMoveSpeed = 80.0f;
	//攻撃を出しているときの移動速度
	constexpr float kPhysicalAttackMoveSpeed = 150;
	//敵が近くにいる判定になる距離
	constexpr float kNearEnemyLength = 200;
	//敵に近づく時間の最大
	constexpr int kGoEnemyTime = 60;
	//通常攻撃のコンボ入力受付時間
	constexpr int kComboTime = 40;
	//気弾攻撃の最大コンボ数
	constexpr int kEnergyComboMax = 3;
	//格闘攻撃の最大コンボ数
	constexpr int kPhysicalComboMax = 2;
}

void PlayerStateAttack::Update(MyEngine::Input input)
{
    DataManager::AttackInfo attackData = m_pPlayer->GetAttackData(m_attackId);
	//経過時間を計測する
	m_time++;
	//移動ベクトル
	MyEngine::Vector3 velo;
	//スティック入力
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//移動方向
	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

    //気弾攻撃のみ出しながら移動できる
    if (attackData.attackEndTime = 0)
    {
		//移動入力がされているとき
		if (dir.sqLength() != 0)
		{
			dir = dir.Normalize();

			//Y軸を中心とした回転をするので
			MyEngine::Vector3 rotationShaftPos = m_pPlayer->GetTargetPos();
			//Y座標が関係しないようにプレイヤーと同じ座標にする
			rotationShaftPos.y = m_pPlayer->GetPos().y;

			MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

			//回転速度(横移動の速さ)
			float hMoveSpeed = 0;

			if (dir.x != 0.0f)
			{
				hMoveSpeed = (dir.x * kEnergyAttackMoveSpeed) / toShaftPosVec.Length();
			}

			m_pPlayer->SetRota(m_pPlayer->GetRota() + hMoveSpeed);

			//左右移動は敵の周囲を回る

			//敵の座標を回転度を参照し、次の回転度だったら次はどの座標になるか計算し
			//現在の座標からその座標に向かうベクトルを作成する
			velo.x = (rotationShaftPos.x + cosf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().x;
			velo.z = (rotationShaftPos.z + sinf(m_pPlayer->GetRota()) * toShaftPosVec.Length()) - m_pPlayer->GetPos().z;

			//上下移動入力されていたら
			if (input.IsPress(Game::InputId::kLb))
			{
				//前後入力を上下のベクトルに変換
				velo.y += dir.z * kEnergyAttackMoveSpeed;
			}
			//されていなかった場合
			else
			{
				//前後入力を回転の中心に向かうベクトルに変換
				MyEngine::Vector3 toCenterVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
				toCenterVec.y = 0;
				velo += toCenterVec.Normalize() * (dir.z * kEnergyAttackMoveSpeed);
			}
		}
    }
	//格闘攻撃だったら相手に向かっていく
	if (!attackData.isEnergy)
	{
		//移動ベクトルの生成
		MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
		velo = moveVec.Normalize() * kPhysicalAttackMoveSpeed;

		//敵が近くにいるかどうかを調べる
		float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
		//敵が近くにいるか、経過時間が一定時間を超えたら
		if (length < kNearEnemyLength || m_time > kGoEnemyTime)
		{
			//敵が近くにいるフラグを立てる
			m_isNearEnemy = true;
		}
		//敵が近くにいたら移動をやめて攻撃を出す
		if (m_isNearEnemy)
		{
			//移動をやめる処理
			velo = MyEngine::Vector3(0, 0, 0);
			//TODO : 攻撃を行う処理を作る
			//攻撃の処理をする時間をへらしていく
			m_actionTime--;
			if (m_actionTime < 0)
			{
				//攻撃の処理をする時間が終わったら
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}

		}
	}
	//通常攻撃のコンボの入力
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (attackData.isEnergy && input.IsTrigger(Game::InputId::kX))
		{
			//時間内に攻撃入力をしていれば次段の攻撃に移行するフラグを立てる
			m_isAttackInput = true;
		}
		//格闘攻撃をした場合 
		if (!attackData.isEnergy && input.IsTrigger(Game::InputId::kB))
		{
			//時間内に攻撃入力をしていれば次段の攻撃に移行するフラグを立てる
			m_isAttackInput = true;
		}
	}

	//入力待機時間を超えたら
	if (m_time > kComboTime)
	{
		//通常攻撃を出す
		m_pPlayer->Attack(attackData.name);

		//攻撃の入力がされていたら
		if (m_isAttackInput)
		{
			//次段の攻撃に移行する
			m_normalAttackNum++;
			//いまだした気弾攻撃が最終段だったら
			if (attackData.isEnergy && m_normalAttackNum > kEnergyComboMax)
			{
				//アイドル状態に戻る
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}
			//いまだした格闘攻撃が最終段だったら
			else if (!attackData.isEnergy && m_normalAttackNum > kPhysicalComboMax)
			{
				//アイドル状態に戻る
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
				return;
			}
			//攻撃の入力をリセットする
			m_isAttackInput = false;
		}
		//攻撃の入力がされていなかったら
		else
		{
			//アイドル状態に戻る
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer);
			return;
		}
	}
	//移動ベクトルを入れる
	m_pPlayer->SetVelo(velo);

	m_nextState = shared_from_this();
}

int PlayerStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
    return 0;
}

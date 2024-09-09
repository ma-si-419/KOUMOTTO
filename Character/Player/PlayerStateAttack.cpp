#include "PlayerStateAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateDodge.h"
#include "Player.h"
#include "SceneGame.h"
#include "CommandIdList.h"
#include "EffekseerForDXLib.h"


namespace
{
	//気弾を出しているときの移動速度
	constexpr float kEnergyAttackMoveSpeed = 80.0f;
	//攻撃を出しているときの移動速度
	constexpr float kPhysicalAttackMoveSpeed = 300;
	//敵が近くにいる判定になる距離
	constexpr float kNearEnemyLength = 1500;
	//敵に近づく時間の最大
	constexpr int kGoEnemyTime = 60;
}

void PlayerStateAttack::Init(std::string button, bool isSpecial)
{
	//必殺技の場合
	if (isSpecial)
	{
		auto attackDataId = m_pPlayer->GetSetSpecialAttackId();

		m_attackId = attackDataId[button];
		m_isNormalAttack = false;
		m_isGoTarget = !m_pPlayer->GetAttackData(m_attackId).isEnergy;
	}
	//通常攻撃の場合
	else
	{
		//気弾攻撃
		if (button == Game::InputId::kX)
		{
			m_attackId = CommandId::kEnergyAttack1;
			m_isGoTarget = false;
		}
		//格闘攻撃
		else if (button == Game::InputId::kB)
		{
			m_attackId = CommandId::kPhysicalAttack1;
			m_isGoTarget = true;
		}
		m_isNormalAttack = true;
	}
	//アニメーションを変更
	m_pPlayer->ChangeAnim(m_pPlayer->GetAttackData(m_attackId).animationName);
	//プレイヤーのMPを減らす
	m_pPlayer->SubMp(m_pPlayer->GetAttackData(m_attackId).cost);
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

	//気弾攻撃かどうか
	bool isNormalEnergy =
		m_attackId == CommandId::kEnergyAttack1 ||
		m_attackId == CommandId::kEnergyAttack2 ||
		m_attackId == CommandId::kEnergyAttack3 ||
		m_attackId == CommandId::kEnergyAttack4;

	//気弾攻撃のみ出しながら移動できる
	if (isNormalEnergy)
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
			//気弾攻撃を行っているときは回避行動にすぐに移行できる
			if (!input.IsPress(Game::InputId::kLb))
			{
				//回避行動の入力がされたら
				if (input.IsTrigger(Game::InputId::kA))
				{
					//StateをDodgeに変更する
					m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
					//回避の方向を設定する
					auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
					state->Init(dir);
					return;
				}
			}
		}
		//移動入力がされていないときに
		if (!input.IsPress(Game::InputId::kLb))
		{
			//回避行動の入力がされたら
			if (input.IsTrigger(Game::InputId::kA))
			{
				//StateをDodgeに変更する
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
				//回避の方向を敵に向かっていく方向に変更する
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				MyEngine::Vector3 dodgeDir = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Normalize();
				state->Init(dodgeDir);
				return;
			}
		}

	}
	//格闘攻撃だったら相手に向かっていく
	if (m_isGoTarget)
	{
		//移動ベクトルの生成
		MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
		velo = moveVec.Normalize() * kPhysicalAttackMoveSpeed;
	}
	//敵が近くにいるかどうかを調べる
	float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
	//敵が近くにいるか、経過時間が一定時間を超えたら
	if (length <= kNearEnemyLength || m_time > kGoEnemyTime && !attackData.isEnergy)
	{
		//敵に向かっていくのをやめる
		m_isGoTarget = false;
		//移動をしない処理
		velo = MyEngine::Vector3(0, 0, 0);
	}
	//敵に向かっていく処理が終わっていたら
	if (!m_isGoTarget)
	{
		//攻撃の処理をする時間をカウントする
		m_actionTime++;
	}
	//攻撃が終わっていなければ
	if (!m_isAttackEnd)
	{
		//攻撃が単発な場合
		if (attackData.attackNum == 1)
		{
			//攻撃を出す時間になったら
			if (m_actionTime > attackData.attackStartTime)
			{
				std::shared_ptr<AttackBase> attack = m_pPlayer->CreateAttack(m_attackId,MyEngine::Vector3(0,0,0));
				//格闘攻撃だったら
				if (!attackData.isEnergy)
				{
					//エフェクトを残すように設定をする
					attack->SetLeaveEffect();
				}
				//攻撃を出す
				m_pScene->AddAttack(attack);
				//攻撃が終了したフラグを立てる
				m_isAttackEnd = true;
				//アニメーションループを止める
				m_pPlayer->StopAnimLoop();
			}
		}
		//攻撃が複数回の場合
		else
		{
			//攻撃のスパンを取得する
			int span = (attackData.attackEndTime - attackData.attackStartTime) / attackData.attackNum;
			//攻撃のタイミングが来たら攻撃を出すようにする
			if (m_actionTime % span == 0 && m_actionTime > attackData.attackStartTime)
			{
				//レーザーを出す座標が指定されていなかったら
				if (m_laserTargetPos.sqLength() == 0)
				{
					m_laserTargetPos = m_pPlayer->GetTargetPos();
				}
				//攻撃を作成
				std::shared_ptr<AttackBase> attack = m_pPlayer->CreateAttack(m_attackId,m_laserTargetPos);
				//レーザー状の攻撃の設定
				if (attackData.isLaser)
				{
					//消えるまでの時間
					int lifeTime = attackData.lifeTime - m_actionTime;

					attack->SetAttackTime(lifeTime);
					//エフェクトを残すようにする
					attack->SetLeaveEffect();
					//一度目の攻撃以外エフェクトを出さないようにする
					if (m_popAttackNum != 0)
					{
						attack->SetNotPopEffect();
					}

					m_popAttackNum++;
				}
				//攻撃を出す
				m_pScene->AddAttack(attack);
			}
			//攻撃を出すタイミングが終了したら
			if (m_actionTime > attackData.attackEndTime)
			{
				m_isAttackEnd = true;
				m_pPlayer->StopAnimLoop();
			}
		}
	}
	//攻撃が終了した後
	else
	{
		//通常攻撃を出した後の処理
		if (m_isNormalAttack)
		{
			//次の攻撃が入力されていたら
			if (m_nextAttackId != "empty")
			{
				//攻撃のコスト分MPがあれば
				if (m_pPlayer->GetAttackCost(m_nextAttackId) < m_pPlayer->GetNowMp())
				{
					//MPを減らす
					m_pPlayer->SubMp(m_pPlayer->GetAttackCost(m_nextAttackId));
					//次の攻撃に移行する
					m_attackId = m_nextAttackId;
					//攻撃の情報を初期化する
					m_actionTime = 0;
					m_time = 0;
					m_isAttackEnd = false;
					auto attack = m_pPlayer->GetAttackData(m_nextAttackId);
					m_isNormalAttack = !attack.isSpecial;
					m_isGoTarget = !attack.isEnergy;
					m_pPlayer->ChangeAnim(attack.animationName);
					m_nextAttackId = "empty";
				}
				//コストが足りなければ
				else
				{
					//攻撃の全体フレームが終わってからIdle状態に戻る
					if (m_actionTime > attackData.actionTotalTime)
					{
						m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
						auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
						state->Init();
						return;
					}
				}
			}
			//次の攻撃が入力されていなければ
			else
			{
				//攻撃の全体フレームが終わってからIdle状態に戻る
				if (m_actionTime > attackData.actionTotalTime)
				{
					m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
					auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
					state->Init();
					return;
				}
			}
		}
		//必殺技を出した後の処理
		else
		{
			//攻撃の全体フレームが終わってからIdle状態に戻る
			if (m_actionTime > attackData.actionTotalTime)
			{
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
		}
	}

	//攻撃が始まってからコンボの入力の受付を行う
	if (m_actionTime > 0)
	{
		//通常攻撃のコンボの入力
		if (!input.IsPress(Game::InputId::kLb))
		{
			//気弾攻撃をした場合
			if (input.IsTrigger(Game::InputId::kX) && attackData.isEnergy)
			{
				//時間内に攻撃入力をしていれば次段の攻撃に移行するフラグを立てる
				m_nextAttackId = GetNextNormalAttack(m_attackId);
			}
			//格闘攻撃をした場合 
			if (input.IsTrigger(Game::InputId::kB) && !attackData.isEnergy)
			{
				//時間内に攻撃入力をしていれば次段の攻撃に移行するフラグを立てる
				m_nextAttackId = GetNextNormalAttack(m_attackId);
			}
		}
		else
		{
			//セットしている必殺技のId
			auto attackDataId = m_pPlayer->GetSetSpecialAttackId();
			//各種必殺技を入力した場合
			//通常技を出しているなら続けて必殺技を出す
			if (input.IsTrigger(Game::InputId::kA))
			{
				m_nextAttackId = attackDataId[Game::InputId::kA];
			}
			if (input.IsTrigger(Game::InputId::kB))
			{
				m_nextAttackId = attackDataId[Game::InputId::kB];
			}
			if (input.IsTrigger(Game::InputId::kX))
			{
				m_nextAttackId = attackDataId[Game::InputId::kX];
			}
			if (input.IsTrigger(Game::InputId::kY))
			{
				m_nextAttackId = attackDataId[Game::InputId::kY];
			}
		}
	}

	//移動ベクトルを入れる
	m_pPlayer->SetVelo(velo);

	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());

	bool endFlag = m_pPlayer->PlayAnim();

	//アニメーションが終了したタイミング
	if (endFlag)
	{
		//気弾の必殺技を打ったあとで
		if (attackData.isSpecial && attackData.isEnergy)
		{
			//気弾の必殺技を打った後のアニメーションが行われていたら
			if (m_isEndSpecialAttackAnim)
			{
				//Stateを戻す
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
			//行われていなかったら
			else
			{
				//必殺技のあと隙の時間分アニメーションを入れる
				m_pPlayer->SetAttackEndAnim(static_cast<float>(attackData.actionTotalTime - attackData.attackEndTime));
				m_isEndSpecialAttackAnim = true;
			}
		}
		//気団の必殺技以外のもの
		else
		{
			//次のアニメーションが設定されていなかったら
			if (m_actionTime == 0)
			{
				//アニメーションが終わったら
				//Stateを戻す
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
		}

	}

	m_nextState = shared_from_this();
}

int PlayerStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//気弾を打っているときのみ状態を変化させる
	if (m_pPlayer->GetAttackData(m_attackId).isEnergy && !m_pPlayer->GetAttackData(m_attackId).isSpecial)
	{
		//HitAttackに変化させる
		m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
		//受けた攻撃の種類を設定する
		auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
		state->Init(collider);
	}
	//ヒットエフェクトを表示する
	int effect = PlayEffekseer3DEffect(m_pPlayer->GetEffekseerData("Hit").first);
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	SetPosPlayingEffekseer3DEffect(effect, pos.x, pos.y, pos.z);
	return damage;
}

std::string PlayerStateAttack::GetNextNormalAttack(std::string id)
{
	if (id == CommandId::kEnergyAttack1)
	{
		return CommandId::kEnergyAttack2;
	}
	else if (id == CommandId::kEnergyAttack2)
	{
		return CommandId::kEnergyAttack3;
	}
	else if (id == CommandId::kEnergyAttack3)
	{
		return CommandId::kEnergyAttack4;
	}
	else if (id == CommandId::kPhysicalAttack1)
	{
		return CommandId::kPhysicalAttack2;
	}
	else if (id == CommandId::kPhysicalAttack2)
	{
		return CommandId::kPhysicalAttack3;
	}
	else
	{
		return "empty";
	}
}

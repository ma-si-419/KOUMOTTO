#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"
#include "CommandIdList.h"
#include "LoadCsv.h"
#include "EffekseerForDXLib.h"

namespace
{
	//格闘攻撃を連続で出したかどうかを判断する時間
	constexpr int kPhysicalAttackConnectTime = 30;
	//気弾攻撃を連続で出したかどうかを判断する時間
	constexpr int kEnergyAttackConnectTime = 30;
}

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0),
	m_isAttack(false),
	m_animTime(0),
	m_animPlaySpeed(0),
	m_totalAnimTime(0),
	m_playEffectFrame(0)
{
	m_playEffectData.first = -1;
	m_playEffectData.second = -1;

	m_modelHandle = MV1LoadModel(model);
	//auto& coldata = std::dynamic_pointer_cast<CapsuleColliderData>;
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

void CharacterBase::SetTargetPos(MyEngine::Vector3 pos)
{
	m_targetPos = pos;
}

void CharacterBase::SetAnimationData(std::vector<std::vector<std::string>> data, bool isPlayer)
{
	for (auto& item : data)
	{
		//入れるデータ
		AnimationInfo pushData;
		//プレイヤーかどうかが一致していればデータを入れる
		if (isPlayer == static_cast<bool>(std::stoi(item[static_cast<int>(AnimationInfoSort::kIsPlayer)])))
		{
			pushData.number = std::stoi(item[static_cast<int>(AnimationInfoSort::kNumber)]);
			pushData.loopFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kLoopFrame)]);
			pushData.endFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kEndFrame)]);
			pushData.playSpeed = std::stof(item[static_cast<int>(AnimationInfoSort::kPlaySpeed)]);

			m_animData[item[static_cast<int>(AnimationInfoSort::kName)]] = pushData;
		}
	}
}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer)
{
	//std::shared_ptr<AttackBase> ans;
	////攻撃データの種類を取得
	//if (isPlayer)
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	//}
	//else
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	//}
	////攻撃を出す座標を作成
	//MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	//MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);
	////ステータス設定
	//ans->Init(physics, attackPos, m_effekseerHandle);

	//return ans;
	return 0;
}

void CharacterBase::ChangeAnim(std::string animName)
{
	//前のアニメーションをデタッチする
	MV1DetachAnim(m_modelHandle, m_playAnim);
	//アニメの再生速度を設定
	m_animPlaySpeed = m_animData[animName].playSpeed;
	//アニメの再生時間をリセット
	m_animTime = 0;
	//アニメーションのループするフレームを設定
	m_animLoopTime = m_animData[animName].loopFrame;
	//新しいアニメーションをアタッチする
	m_playAnim = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	//アニメーションの総再生時間を設定する
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].number);
}

void CharacterBase::PlayAnim()
{
	//アニメーションの再生フレームを進める
	m_animTime += m_animPlaySpeed;
	//アニメーションの総再生フレームを超えたら
	if (m_animTime > m_totalAnimTime)
	{
		//ループするフレームに戻す
		m_animTime = m_animLoopTime;
	}
	//再生フレームを反映させる
	MV1SetAttachAnimTime(m_modelHandle, m_playAnim, m_animTime);
}


void CharacterBase::PlayEffect()
{
	//エフェクトのハンドルが入っていてまだ再生していなかったら
	if (m_playEffectData.first != -1 && m_playEffectHandle == -1)
	{
		//エフェクトをプレイする
		m_playEffectHandle = PlayEffekseer3DEffect(m_playEffectData.first);
		//プレイヤーの座標
		MyEngine::Vector3 pos = m_rigidbody.GetPos();
		//エフェクトをプレイヤーの座標に設定
		SetPosPlayingEffekseer3DEffect(m_playEffectHandle, pos.x, pos.y, pos.z);
	}
	//エフェクトを再生しているときの処理
	if (m_playEffectHandle != -1)
	{
		//プレイヤーの座標
		MyEngine::Vector3 pos = m_rigidbody.GetPos();
		//エフェクトをプレイヤーの座標に設定
		SetPosPlayingEffekseer3DEffect(m_playEffectHandle, pos.x, pos.y, pos.z);
		//エフェクトの再生時間を更新
		m_playEffectFrame++;
	}
	//エフェクトをループさせる
	if (m_playEffectFrame > m_playEffectData.second)
	{
		//エフェクトの再生時間を0にする
		m_playEffectFrame = 0;
		//エフェクトを止める
		StopEffekseer3DEffect(m_playEffectHandle);
		//エフェクトを再び再生する
		m_playEffectHandle = PlayEffekseer3DEffect(m_playEffectData.first);
		//プレイヤーの座標
		MyEngine::Vector3 pos = m_rigidbody.GetPos();
		//エフェクトをプレイヤーの座標に設定
		SetPosPlayingEffekseer3DEffect(m_playEffectHandle, pos.x, pos.y, pos.z);
	}
}

void CharacterBase::PlaySpecialAttack(std::string id)
{
	//技で動けない時間を設定する
	m_stanTime = m_attackData[id].actionTotalTime;
	//技の消費気力分、現在の気力を減らす
	m_nowMp -= m_attackData[id].cost;
	//攻撃のターゲット座標をロックする
	m_attackTarget = m_targetPos;
	//攻撃を出している状態に変える
	m_isAttack = true;
	//出している攻撃の種類を取得する
	m_attackId = id;
	//敵が近くにいるかどうかをfalseにする
	m_isNearTarget = false;
}

void CharacterBase::SetModelFront(MyEngine::Vector3 pos)
{
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - pos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void CharacterBase::SetNormalAttack(bool isPhysical, int time)
{
	//今出している攻撃の確認して出す攻撃を設定する


	//格闘攻撃の場合
	if (isPhysical)
	{
		if (time < kPhysicalAttackConnectTime)
		{

			//格闘攻撃の一段目を出していたら
			if (m_attackId == CommandId::kPhysicalAttack1)
			{
				//格闘攻撃の二段目を出す
				m_attackId = CommandId::kPhysicalAttack2;
			}
			//格闘攻撃の二段目を出していたら
			else if (m_attackId == CommandId::kPhysicalAttack2)
			{
				//格闘攻撃の三段目を出す
				m_attackId = CommandId::kPhysicalAttack3;
			}
			//格闘技の一段目、二段目を出していなければ
			else
			{
				m_attackId = CommandId::kPhysicalAttack1;
			}
		}
		//格闘攻撃がコンボになるタイミングでなければ
		else
		{
			//格闘攻撃の一段目を出す
			m_attackId = CommandId::kPhysicalAttack1;
		}
		//敵が近くにいるかどうかをfalseにする
		m_isNearTarget = false;
	}
	//気弾攻撃の場合
	else
	{
		if (time < kEnergyAttackConnectTime)
		{

			//気弾攻撃の一段目を出していたら
			if (m_attackId == CommandId::kEnergyAttack1)
			{
				//気弾攻撃の二段目を出す
				m_attackId = CommandId::kEnergyAttack2;
			}
			//気弾攻撃の二段目を出していたら
			else if (m_attackId == CommandId::kEnergyAttack2)
			{
				//気弾攻撃の三段目を出す
				m_attackId = CommandId::kEnergyAttack3;
			}
			//気弾攻撃の三段目を出していたら
			else if (m_attackId == CommandId::kEnergyAttack3)
			{
				//気弾攻撃の四段目を出す
				m_attackId = CommandId::kEnergyAttack4;
			}
			//気弾攻撃の一、二、三段目でなければ
			else
			{
				//気弾攻撃の一段目を出す
				m_attackId = CommandId::kEnergyAttack1;
			}
		}
		//気弾攻撃がコンボになるタイミング出なければ
		else
		{
			//気弾攻撃の1段目を出す
			m_attackId = CommandId::kEnergyAttack1;
		}
	}
	//攻撃のターゲット座標をロックする
	m_attackTarget = m_targetPos;
	//技で動けない時間を設定する
	m_stanTime = m_attackData[m_attackId].actionTotalTime;
	//攻撃を出している状態に変える
	m_isAttack = true;

}


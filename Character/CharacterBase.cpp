#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"
#include "CommandIdList.h"

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
	m_isAttack(false)
{
	m_modelHandle = MV1LoadModel(model);
	//auto& coldata = std::dynamic_pointer_cast<CapsuleColliderData>;
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer)
{
	std::shared_ptr<AttackBase> ans;
	//攻撃データの種類を取得
	if (isPlayer)
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	}
	else
	{
		ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	}
	//攻撃を出す座標を作成
	MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	ans->Init(physics, attackPos);
	//ステータス設定
	ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);

	return ans;
}

void CharacterBase::ChangeAnim(int animNum)
{
}

void CharacterBase::SetSpecialAttack(std::string id)
{
	//技で動けない時間を設定する
	m_stanTime = m_attackData[id].actionTime;
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
			else if (m_attackId == CommandId::kPhysicalAttack3)
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
			else if (m_attackId == CommandId::kEnergyAttack4)
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
	m_stanTime = m_attackData[m_attackId].actionTime;
	//攻撃を出している状態に変える
	m_isAttack = true;

}


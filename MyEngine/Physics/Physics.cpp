#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
void Physics::Entry(Collidable* col)
{
	// 登録
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (!found)
	{
		collidables.emplace_back(col);
	}
	// 既に登録されてたらエラー
	else
	{
		assert(0 && "指定のcollidableは登録済です。");
	}
}

void Physics::Exit(Collidable* col)
{
	// 登録解除
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (found)
	{
		collidables.remove(col);
	}
	// 登録されてなかったらエラー
	else
	{
		assert(0 && "指定のcollidableが登録されていません。");
	}
}

void Physics::Update()
{
	//仮処理
	for (auto& item : collidables)
	{
		item->m_nextPos = (item->m_rigidbody.GetPos() + item->m_rigidbody.GetVelo());
	}
	//ネクストポスを使い当たり判定を行う

	FixPosition();
}

void Physics::DebugDraw()
{
	for (auto& item : collidables)
	{
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleData = dynamic_cast<CapsuleColliderData*>(item->m_pColData);

			DrawCapsule3D(capsuleData->m_startPos.CastVECTOR(), item->m_rigidbody.GetPos().CastVECTOR(),
				capsuleData->m_radius, 5, GetColor(50, 50, 255), GetColor(255, 255, 255), false);
		}
		else if (item->m_pColData->GetKind() == ColliderData::Kind::kSphere)
		{
			auto sphereData = dynamic_cast<SphereColliderData*>(item->m_pColData);
			
			DrawSphere3D(item->m_rigidbody.GetPos().CastVECTOR(), sphereData->m_radius, 5, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
		}
	}
}

void Physics::FixPosition()
{
	for (auto& item : collidables)
	{
		// Posを更新するので、velocityもそこに移動するvelocityに修正
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos();
		item->m_rigidbody.SetVelo(toFixedPos);

		// 位置確定
		item->m_rigidbody.SetPos(item->m_nextPos);

		//当たり判定がカプセルだったら
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleCol = dynamic_cast<CapsuleColliderData*>(item->m_pColData);
			//伸びるカプセルかどうか取得する
			if (!capsuleCol->m_isMoveStartPos)
			{
				//伸びないカプセルだったら初期位置を一緒に動かす
				capsuleCol->m_startPos = item->m_nextPos;
			}
		}
	}
}

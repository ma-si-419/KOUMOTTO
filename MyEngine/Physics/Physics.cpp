#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
void Physics::Entry(std::shared_ptr<Collidable> col)
{
	// 登録
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (!found)
	{
		m_collidables.emplace_back(col);
	}
	// 既に登録されてたらエラー
	else
	{
		assert(0 && "指定のcollidableは登録済です。");
	}
}

void Physics::Exit(std::shared_ptr<Collidable> col)
{
	// 登録解除
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(col);
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
	for (auto& item : m_collidables)
	{
		item->m_nextPos = (item->m_rigidbody.GetPos() + item->m_rigidbody.GetVelo());
	}
	for (auto& first : m_collidables)
	{
		for (auto& second : m_collidables)
		{
			//当たり判定チェック
			if (CheckCollide(first,second))
			{
				//当たっていれば配列に入れる

			}
		}
	}

	//位置修正
	FixPosition();
	printfDx("%d", m_collidables.size());
}

void Physics::DebugDraw()
{
	//当たり判定の描画を行う
	for (auto& item : m_collidables)
	{
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleData = std::dynamic_pointer_cast<CapsuleColliderData>(item->m_pColData);

			DrawCapsule3D(capsuleData->m_startPos.CastVECTOR(), item->m_rigidbody.GetPos().CastVECTOR(),
				capsuleData->m_radius, 5, GetColor(50, 50, 255), GetColor(255, 255, 255), false);
		}
		else if (item->m_pColData->GetKind() == ColliderData::Kind::kSphere)
		{
			auto sphereData = std::dynamic_pointer_cast<SphereColliderData>(item->m_pColData);

			DrawSphere3D(item->m_rigidbody.GetPos().CastVECTOR(), sphereData->m_radius, 5, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
		}
	}
}

void Physics::FixPosition()
{
	for (auto& item : m_collidables)
	{
		// Posを更新するので、velocityもそこに移動するvelocityに修正
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos();
		item->m_rigidbody.SetVelo(toFixedPos);

		// 位置確定
		item->m_rigidbody.SetPos(item->m_nextPos);

		//当たり判定がカプセルだったら
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleCol = std::dynamic_pointer_cast<CapsuleColliderData>(item->m_pColData);
			//伸びるカプセルかどうか取得する
			if (!capsuleCol->m_isMoveStartPos)
			{
				//伸びないカプセルだったら初期位置を一緒に動かす
				capsuleCol->m_startPos = item->m_nextPos;
			}
		}
	}
}

bool Physics::CheckCollide(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second)
{
	//第一の当たり判定と第二の当たり判定がおなじものでなければ
	if (first != second)
	{
		//当たり判定の種類を取得
		ColliderData::Kind firstKind = first->m_pColData->GetKind();
		ColliderData::Kind secondKind = second->m_pColData->GetKind();

		//球どうしの当たり判定
		if (firstKind == ColliderData::Kind::kSphere && secondKind == ColliderData::Kind::kSphere)
		{
			//当たり判定を球にダウンキャストする
			auto firstCol = std::dynamic_pointer_cast<SphereColliderData>(first->m_pColData);
			auto secondCol = std::dynamic_pointer_cast<SphereColliderData>(second->m_pColData);

			//当たり判定の距離を求める
			float length = (first->m_nextPos - second->m_nextPos).Length();

			//球の大きさを合わせたものよりも距離が短ければぶつかっている
			if (length < firstCol->m_radius + secondCol->m_radius)
			{
				return true;
			}
		}
		//カプセル同士の当たり判定
		else if (firstKind == ColliderData::Kind::kCapsule && secondKind == ColliderData::Kind::kCapsule)
		{

		}
		//球とカプセルの当たり判定
		else if (firstKind == ColliderData::Kind::kSphere && secondKind == ColliderData::Kind::kCapsule ||
			firstKind == ColliderData::Kind::kCapsule && secondKind == ColliderData::Kind::kSphere)
		{

		}
		//どれにも当てはまらなかったら
		else
		{
			assert(false);
			return false;
		}
	}
}

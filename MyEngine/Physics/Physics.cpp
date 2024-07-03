#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
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
	FixPosition();
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
	}
}

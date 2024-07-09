#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
void Physics::Entry(Collidable* col)
{
	// “o˜^
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (!found)
	{
		collidables.emplace_back(col);
	}
	// Šù‚É“o˜^‚³‚ê‚Ä‚½‚çƒGƒ‰[
	else
	{
		assert(0 && "Žw’è‚Ìcollidable‚Í“o˜^Ï‚Å‚·B");
	}
}

void Physics::Exit(Collidable* col)
{
	// “o˜^‰ðœ
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (found)
	{
		collidables.remove(col);
	}
	// “o˜^‚³‚ê‚Ä‚È‚©‚Á‚½‚çƒGƒ‰[
	else
	{
		assert(0 && "Žw’è‚Ìcollidable‚ª“o˜^‚³‚ê‚Ä‚¢‚Ü‚¹‚ñB");
	}
}

void Physics::Update()
{
	//‰¼ˆ—
	for (auto& item : collidables)
	{
		item->m_nextPos = (item->m_rigidbody.GetPos() + item->m_rigidbody.GetVelo());
	}
	//ƒlƒNƒXƒgƒ|ƒX‚ðŽg‚¢“–‚½‚è”»’è‚ðs‚¤

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
		// Pos‚ðXV‚·‚é‚Ì‚ÅAvelocity‚à‚»‚±‚ÉˆÚ“®‚·‚évelocity‚ÉC³
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos();
		item->m_rigidbody.SetVelo(toFixedPos);

		// ˆÊ’uŠm’è
		item->m_rigidbody.SetPos(item->m_nextPos);

		//“–‚½‚è”»’è‚ªƒJƒvƒZƒ‹‚¾‚Á‚½‚ç
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleCol = dynamic_cast<CapsuleColliderData*>(item->m_pColData);
			//L‚Ñ‚éƒJƒvƒZƒ‹‚©‚Ç‚¤‚©Žæ“¾‚·‚é
			if (!capsuleCol->m_isMoveStartPos)
			{
				//L‚Ñ‚È‚¢ƒJƒvƒZƒ‹‚¾‚Á‚½‚ç‰ŠúˆÊ’u‚ðˆê‚É“®‚©‚·
				capsuleCol->m_startPos = item->m_nextPos;
			}
		}
	}
}

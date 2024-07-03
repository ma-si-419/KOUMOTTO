#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
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
	FixPosition();
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
	}
}

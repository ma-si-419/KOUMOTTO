#include "CharacterBase.h"
#include <cassert>

CharacterBase::CharacterBase(const TCHAR* model,ObjectTag tag):
	Collidable(tag,ColliderData::Kind::kCapsule)
{
	m_modelHandle = MV1LoadModel(model);
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

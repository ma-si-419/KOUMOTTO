#include "ObjectBase.h"

ObjectBase::ObjectBase(const TCHAR* model):
	m_modelHandle(-1),
	m_pos()
{
	m_modelHandle = MV1LoadModel(model);
	MV1SetPosition(m_modelHandle, m_pos.CastVECTOR());
}

ObjectBase::~ObjectBase()
{
	MV1DeleteModel(m_modelHandle);
}

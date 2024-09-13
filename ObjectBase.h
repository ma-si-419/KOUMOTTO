#pragma once
#include "Vector3.h"
class ObjectBase
{
public:
	ObjectBase(const TCHAR* model);
	virtual ~ObjectBase();

	virtual void Draw() abstract;
protected:

	int m_modelHandle;
	MyEngine::Vector3 m_pos;
};


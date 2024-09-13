#pragma once
#include "ObjectBase.h"
class Stage : public ObjectBase
{
public:
	Stage();
	virtual ~Stage();

	void Draw() override;
};
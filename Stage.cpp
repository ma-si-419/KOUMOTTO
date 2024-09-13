#include "Stage.h"

Stage::Stage():
	ObjectBase("data/model/Stage.mv1")
{
}

Stage::~Stage()
{
}

void Stage::Draw()
{
	MV1DrawModel(m_modelHandle);
}

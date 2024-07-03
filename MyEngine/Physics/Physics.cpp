#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
void Physics::Entry(Collidable* col)
{
	// �o�^
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (!found)
	{
		collidables.emplace_back(col);
	}
	// ���ɓo�^����Ă���G���[
	else
	{
		assert(0 && "�w���collidable�͓o�^�ςł��B");
	}
}

void Physics::Exit(Collidable* col)
{
	// �o�^����
	bool found = (std::find(collidables.begin(), collidables.end(), col) != collidables.end());
	if (found)
	{
		collidables.remove(col);
	}
	// �o�^����ĂȂ�������G���[
	else
	{
		assert(0 && "�w���collidable���o�^����Ă��܂���B");
	}
}

void Physics::Update()
{
	//������
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
		// Pos���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos();
		item->m_rigidbody.SetVelo(toFixedPos);

		// �ʒu�m��
		item->m_rigidbody.SetPos(item->m_nextPos);
	}
}

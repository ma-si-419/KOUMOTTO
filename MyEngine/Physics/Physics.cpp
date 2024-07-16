#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
void Physics::Entry(std::shared_ptr<Collidable> col)
{
	// �o�^
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (!found)
	{
		m_collidables.emplace_back(col);
	}
	// ���ɓo�^����Ă���G���[
	else
	{
		assert(0 && "�w���collidable�͓o�^�ςł��B");
	}
}

void Physics::Exit(std::shared_ptr<Collidable> col)
{
	// �o�^����
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(col);
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
	for (auto& item : m_collidables)
	{
		item->m_nextPos = (item->m_rigidbody.GetPos() + item->m_rigidbody.GetVelo());
	}
	//�l�N�X�g�|�X���g�������蔻����s��

	FixPosition();
	printfDx("%d", m_collidables.size());
}

void Physics::DebugDraw()
{
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
		// Pos���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos();
		item->m_rigidbody.SetVelo(toFixedPos);

		// �ʒu�m��
		item->m_rigidbody.SetPos(item->m_nextPos);

		//�����蔻�肪�J�v�Z����������
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleCol = std::dynamic_pointer_cast<CapsuleColliderData>(item->m_pColData);
			//�L�т�J�v�Z�����ǂ����擾����
			if (!capsuleCol->m_isMoveStartPos)
			{
				//�L�тȂ��J�v�Z���������珉���ʒu���ꏏ�ɓ�����
				capsuleCol->m_startPos = item->m_nextPos;
			}
		}
	}
}

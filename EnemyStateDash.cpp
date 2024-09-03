#include "EnemyStateDash.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 240;
	//�㉺�ړ���D�悷��Y���W�̂���̑傫��
	constexpr float kYGapScale = 300;
	//�߂Â����Ƃ�D�悵�n�߂鋗��
	constexpr float kMoveFrontDistance = 2000;
	//����邱�Ƃ�D�悵�n�߂鋗��
	constexpr float kMoveBackDistance = 500;
	//����Ă��鋗���̂܂ܗD��x���グ�Ȃ��悤�ɁA�����Ɋ����������ėD��x�ɕϊ�����
	constexpr float kDistanceRate = 0.1;
	//��{�I�Ȉړ������̊���
	constexpr int kMoveDirRate[3] = { 35,10,30 };
	//�ړ����x
	constexpr float kMoveSpeed = 80.0f;
	//�����̕����̐�
	constexpr int kMoveDirNum = 8;
	//�����̕����̐��̔���
	constexpr int kMoveDirNumHalf = kMoveDirNum * 0.5;
}

void EnemyStateDash::Init(MyEngine::Vector3 playerPos)
{
	//player�̍��W�ɍ��킹�ē������������肷��

	//�ړ������̊���
	int moveRate[static_cast<int>(MoveKind::kMoveKindNum)];
	//����������������
	moveRate[static_cast<int>(MoveKind::kFront)] = kMoveDirRate[static_cast<int>(MoveKind::kFront)];
	moveRate[static_cast<int>(MoveKind::kBack)] = kMoveDirRate[static_cast<int>(MoveKind::kBack)];
	moveRate[static_cast<int>(MoveKind::kRandom)] = kMoveDirRate[static_cast<int>(MoveKind::kRandom)];


	//Y���W������Ă�����ł��邾�����킹��悤�ɓ���
	float gapPosY = abs(m_pEnemy->GetPos().y - playerPos.y);
	//Y���W�̂��ꂪ�傫��������㉺�ړ�����悤�ɂ���
	if (gapPosY > kYGapScale)
	{
		m_isMoveVertical = true;
	}
	//�v���C���[�Ƃ̋�����������Ή����قǃv���C���[�Ɍ������Ă����m�����グ��
	float distance = (m_pEnemy->GetPos() - playerPos).Length();
	//������������������΋߂Â��D��x���グ��
	if (distance > kMoveFrontDistance)
	{
		//�D��x�̑�����
		int addRate = static_cast<int>((kMoveFrontDistance - distance) * kDistanceRate);

		moveRate[static_cast<int>(MoveKind::kFront)] += addRate;
	}
	//�������������߂���Η����悤�ɂ���
	else if (distance < kMoveBackDistance)
	{
		//�ق��̑I����������
		moveRate[static_cast<int>(MoveKind::kFront)] = 0;
		moveRate[static_cast<int>(MoveKind::kRandom)] = 0;
	}

	//���������ׂđ������l
	int total = 0;

	for (auto item : moveRate)
	{
		total += item;
	}

	//���ׂđ������l�������_���͈̔͂ɂ���
	int ans = GetRand(total);
	int moveKind = 0;

	//�����������Ă����A0�ȉ��ɂȂ������̍s���Ɍ��肷��
	for (auto item : moveRate)
	{
		ans -= item;
		if (item <= 0)
		{
			break;
		}
		moveKind++;
	}

	//�ړ�����
	MyEngine::Vector3 moveDir;

	//�v���C���[�Ɍ������Ă���
	if (moveKind == static_cast<int>(MoveKind::kFront))
	{
		moveDir = (playerPos - m_pEnemy->GetPos()).Normalize();
	}
	//�v���C���[���痣���
	else if (moveKind == static_cast<int>(MoveKind::kBack))
	{
		moveDir = (m_pEnemy->GetPos() - playerPos).Normalize();
		//�㉺�ړ��t���O�������Ă�����
		if (m_isMoveVertical)
		{
			//�v���C���[��Y���W�����킹�ɍs��
			moveDir.y *= -1;
		}
	}
	//�����_���ȕ����Ɉړ�����
	else if (moveKind == static_cast<int>(MoveKind::kRandom))
	{
		moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf).Normalize();

		//�㉺�ړ��t���O�������Ă�����
		if (m_isMoveVertical)
		{
			//�O�㉡�ړ��Ȃ��ŏ㉺�ړ�����
			MyEngine::Vector3 targetPos = playerPos;
			//�O�㉡�������G�l�~�[�̌��ݒn�Ɠ����ɂ���
			targetPos.x = m_pEnemy->GetPos().x;
			targetPos.z = m_pEnemy->GetPos().z;

			//�㉺�ړ������݂̂̕����x�N�g�����쐬
			moveDir = (targetPos - m_pEnemy->GetPos()).Normalize();
		}

	}

	//�ړ��x�N�g��
	m_velo = moveDir * kMoveSpeed;

}

void EnemyStateDash::Update()
{
	//�o�ߎ��Ԃ��v��
	m_time++;

	//Init�Ō��肵���x�N�g���ňړ�����
	m_pEnemy->SetVelo(m_velo);

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateDash::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();

	return damage;
}
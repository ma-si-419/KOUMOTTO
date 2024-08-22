#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge : public PlayerStateBase
{
public:
	enum class MoveDir
	{
		kFront,
		kBack,
		kRight,
		kLeft
	};

	virtual void Update(std::shared_ptr<Player> player, MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kDodge; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

	void SetAnimDir(MoveDir dir) { m_animDir = dir; }

	void SetMoveDir(MyEngine::Vector3 dir) { m_moveDir = dir; }
private:


	//移動の方向
	MyEngine::Vector3 m_moveDir;

	//アニメの方向
	MoveDir m_animDir;

};


#pragma once
#include "PlayerStateBase.h"
class PlayerStateDodge : public PlayerStateBase
{
public:
	PlayerStateDodge(std::shared_ptr<Player> player, std::shared_ptr<SceneGame> scene) : PlayerStateBase(player, scene) {};

	void Init(MyEngine::Vector3 dir);

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kDodge; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

private:
	//移動の方向
	MyEngine::Vector3 m_moveDir;
	
	//経過時間
	int m_time = 0;

};


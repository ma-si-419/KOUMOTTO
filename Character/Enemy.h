#pragma once
#include "CharacterBase.h"
#include <memory>
class SceneGame;
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(std::shared_ptr<Physics> physics);
	void Update(std::shared_ptr<SceneGame> scene);
	void Draw();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
private:

};

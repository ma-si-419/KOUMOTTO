#pragma once
#include "CharacterBase.h"
#include "Input.h"
class Player : public CharacterBase
{
public:
	Player();
	~Player();

	void Init(Physics* physics);
	void Update(MyEngine::Input input);
	void Draw();
private:
	MyEngine::Vector3 m_cameraDir;
};


#pragma once
#include "Game.h"
#include <vector>
#include <string>

class LoadCsv
{
public:
	Game::AttackInfo GetAttackData();
private:
	//�����ŋ�؂�֐�
	std::vector<std::string> Split(const std::string& str, const char cutChar);
};


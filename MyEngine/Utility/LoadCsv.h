#pragma once
#include "Game.h"
#include <vector>
#include <string>
#include <map>

class LoadCsv
{
private:
	enum class AttackInfoSort
	{
		kId,
		kCost,
		kDamageRate,
		kSpeed,
		kRadius,
		kLifeTime,
		kAttackNum,
		kLaser,
		kTrackStartTime,
		kTrackEndTime,
		kAttackTime,
		kActionTime,
		kEnergy
	};
public:
	void LoadAttackData();
	std::map<std::string, Game::AttackInfo> GetAttackData() { return m_attackData; };
private:
	//攻撃のデータ
	std::map<std::string, Game::AttackInfo> m_attackData;

	//文字で区切る関数
	std::vector<std::string> Split(const std::string& str, const char cutChar);
};


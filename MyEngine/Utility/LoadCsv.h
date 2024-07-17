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
	//UŒ‚‚Ìƒf[ƒ^
	std::map<std::string, Game::AttackInfo> m_attackData;

	//•¶š‚Å‹æØ‚éŠÖ”
	std::vector<std::string> Split(const std::string& str, const char cutChar);
};


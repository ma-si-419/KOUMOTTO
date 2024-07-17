#pragma once
namespace Game
{
	namespace InputId
	{
		const char* const kOk = "OK";
		const char* const kPause = "PAUSE";
		const char* const kSelect = "SELECT";
		const char* const kCancel = "CANCEL";
		const char* const kUp = "UP";
		const char* const kDown = "DOWN";
		const char* const kLeft = "LEFT";
		const char* const kRight = "RIGHT";
		const char* const kLb = "LB";
		const char* const kRb = "RB";
		const char* const kA = "A";
		const char* const kB = "B";
		const char* const kX = "X";
		const char* const kY = "Y";

	}
	struct AttackInfo
	{
		int cost;
		float damageRate;
		float speed;
		float radius;
		int lifeTime;
		int attackNum;
		bool isLaser;
		int trackStartTime;
		int trackEndTime;
		int attackTime;
		int actionTime;
		bool isEnergy;
	};
}
#pragma once

// 当たったものの判別を行うためのタグ
enum class ObjectTag
{
	kPlayer,			//プレイヤー
	kEnemy,				//敵
	kEnergyAttack,		//気弾攻撃
	kPhysicalAttack,	//格闘攻撃
	kGround				//地面
};
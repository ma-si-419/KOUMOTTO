#pragma once

// 当たったものの判別を行うためのタグ
enum class ObjectTag
{
	kPlayer,			// プレイヤー
	kEnemy,			// 敵
	kGround
};
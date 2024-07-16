#pragma once
#include <list>
#include <vector>
#include <memory>

class Collidable;
class Physics final
{
public:
	//判定をする衝突物を登録・削除する
	void Entry(std::shared_ptr<Collidable> col);
	void Exit(std::shared_ptr<Collidable> col);
	//登録した衝突物の物理移動、衝突通知を行う
	void Update();
	//当たり判定の表示を行う
	void DebugDraw();
private:
	std::list<std::shared_ptr<Collidable>> m_collidables; //登録されたcollidableのリスト
	void FixPosition();
	struct OnCollideInfo
	{

	};
};


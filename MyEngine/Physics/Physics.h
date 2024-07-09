#pragma once
#include <list>
#include <vector>

class Collidable;
class Physics final
{
public:
	//���������Փ˕���o�^�E�폜����
	void Entry(Collidable* col);
	void Exit(Collidable* col);
	//�o�^�����Փ˕��̕����ړ��A�Փ˒ʒm���s��
	void Update();
	//�����蔻��̕\�����s��
	void DebugDraw();
private:
	std::list<Collidable*> collidables; //�o�^���ꂽcollidable�̃��X�g
	void FixPosition();
	struct OnCollideInfo
	{

	};
};


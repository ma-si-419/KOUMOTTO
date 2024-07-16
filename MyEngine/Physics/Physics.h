#pragma once
#include <list>
#include <vector>
#include <memory>

class Collidable;
class Physics final
{
public:
	//���������Փ˕���o�^�E�폜����
	void Entry(std::shared_ptr<Collidable> col);
	void Exit(std::shared_ptr<Collidable> col);
	//�o�^�����Փ˕��̕����ړ��A�Փ˒ʒm���s��
	void Update();
	//�����蔻��̕\�����s��
	void DebugDraw();
private:
	std::list<std::shared_ptr<Collidable>> m_collidables; //�o�^���ꂽcollidable�̃��X�g
	void FixPosition();
	struct OnCollideInfo
	{

	};
};


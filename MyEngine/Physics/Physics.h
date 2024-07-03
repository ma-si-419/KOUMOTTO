#pragma once
#include <list>
#include <vector>

class Collidable;
class Physics final
{
public:
	//”»’è‚ğ‚·‚éÕ“Ë•¨‚ğ“o˜^Eíœ‚·‚é
	void Entry(Collidable* col);
	void Exit(Collidable* col);
	//“o˜^‚µ‚½Õ“Ë•¨‚Ì•¨—ˆÚ“®AÕ“Ë’Ê’m‚ğs‚¤
	void Update();
private:
	std::list<Collidable*> collidables; //“o˜^‚³‚ê‚½collidable‚ÌƒŠƒXƒg
	void FixPosition();
};


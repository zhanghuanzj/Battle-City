#ifndef MAP_ELEMENT
#define MAP_ELEMENT
//#include "Tank.h"
//#include "LightTank.h"
//#include "Function.h"
class Tank;
class LightTank;
class Assassin;
extern int map[5][15][15];
extern int g_enemyNumber;
class Map_element  //地图元素基类
{
public:
	virtual void Move(Tank *pTank)=0;
};

class Grass :public Map_element //草坪类元素
{
public:
	static Grass* Instance();
	void Move(Tank *pTank);

};

class Brick1 :public Map_element //砖块1类元素
{
public:
	static Brick1* Instance();
	void Move(Tank *pTank);

};

class Brick2 :public Map_element //砖块2类元素
{
public:
	static Brick2* Instance();
	void Move(Tank *pTank);

};

class Floor :public Map_element //地板类元素
{
public:
	static Floor* Instance();
	void Move(Tank *pTank);

};
class Ice :public Map_element //冰地类元素
{
public:
	static Ice* Instance();
	void Move(Tank *pTank);

};

class Sand :public Map_element //沙漠类元素
{
public:
	static Sand* Instance();
	void Move(Tank *pTank);

};

class Forest :public Map_element //森林类元素
{
public:
	static Forest* Instance();
	void Move(Tank *pTank);

};

class Snow :public Map_element //雪地类元素
{
public:
	static Snow* Instance();
	void Move(Tank *pTank);

};
class Water :public Map_element //河流类元素
{
public:
	static Water* Instance();
	void Move(Tank *pTank);

};


bool WhichMapElement(Tank& tank);//定位所在地形,并判断周围是否有障碍


#endif // !MAP_ELEMENT

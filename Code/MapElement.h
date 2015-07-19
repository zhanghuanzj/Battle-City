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
class Map_element  //��ͼԪ�ػ���
{
public:
	virtual void Move(Tank *pTank)=0;
};

class Grass :public Map_element //��ƺ��Ԫ��
{
public:
	static Grass* Instance();
	void Move(Tank *pTank);

};

class Brick1 :public Map_element //ש��1��Ԫ��
{
public:
	static Brick1* Instance();
	void Move(Tank *pTank);

};

class Brick2 :public Map_element //ש��2��Ԫ��
{
public:
	static Brick2* Instance();
	void Move(Tank *pTank);

};

class Floor :public Map_element //�ذ���Ԫ��
{
public:
	static Floor* Instance();
	void Move(Tank *pTank);

};
class Ice :public Map_element //������Ԫ��
{
public:
	static Ice* Instance();
	void Move(Tank *pTank);

};

class Sand :public Map_element //ɳĮ��Ԫ��
{
public:
	static Sand* Instance();
	void Move(Tank *pTank);

};

class Forest :public Map_element //ɭ����Ԫ��
{
public:
	static Forest* Instance();
	void Move(Tank *pTank);

};

class Snow :public Map_element //ѩ����Ԫ��
{
public:
	static Snow* Instance();
	void Move(Tank *pTank);

};
class Water :public Map_element //������Ԫ��
{
public:
	static Water* Instance();
	void Move(Tank *pTank);

};


bool WhichMapElement(Tank& tank);//��λ���ڵ���,���ж���Χ�Ƿ����ϰ�


#endif // !MAP_ELEMENT

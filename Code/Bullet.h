#ifndef BULLET
#define BULLET
#include "SPoint.h"
#include "Direction.h"
#include <windows.h>
#include <vector>
#include "Function.h"
using namespace std;
class Tank;
extern vector<APoint> ShootedPlace;
extern HBITMAP g_hBullet[4];
extern HBITMAP g_hBulletEx[10];
extern int g_enemyNumber;
extern Tank *enemys[10];
extern Tank *newEnemy[20];
extern Tank *oldEnemy[20];
extern Tank *pLTank;
extern RECT g_rect;
extern HDC g_hbufdc;
extern int map[5][15][15];
extern int g_score;
class Bullet
{
private:
	SPoint m_pLocation;
	Tank *m_tOwner;
	Direction m_dDirection;
	int m_iMoveSpeed;
	int bulletExplode;
	int fireX,fireY;
	bool m_bIsFly;

public:
	Bullet(){}
	Bullet(int x,int y,Tank *owner,Direction dir,int speed=6,bool flag=false);
	SPoint Location() {return m_pLocation;}
	void setX(int x){m_pLocation.setX(x);}
	void setY(int y){m_pLocation.setY(y);}        //����


	Tank* getOwner(){return m_tOwner;}         //�ڵ�������

	void setDirection(Direction dir){m_dDirection=dir;}//���÷���
	Direction getDirection(){return m_dDirection;}

	int getMoveSpeed(){return m_iMoveSpeed;}//�Ż��ٶ�

	bool getIsFly(){return m_bIsFly;}         //�Ƿ��ڷ���
	void setIsFly(bool flag){m_bIsFly=flag;bulletExplode=0;}  //����״̬

	void Move();                             //�ƶ�
	void Paint(HDC& hmdc);

};
#endif
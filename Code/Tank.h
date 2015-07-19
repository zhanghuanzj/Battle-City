#ifndef TANK
#define TANK
#include "SPoint.h"
#include "Direction.h"
#include <windows.h>
#include "resource.h"
#include "Bullet.h"
class Bullet;
class Map_element;
extern HDC g_hbufdc;
extern HDC g_hdc;
extern RECT g_rect;
extern HBITMAP g_hBomb[8];
class Tank
{
	friend class Bullet;
private:
	int m_iID;
	int m_iMoveSpeed;
	int m_iHP;
	bool m_bIsAlive;
	Direction m_dDirection;
	Map_element * m_pMapElement;

protected:
	SPoint m_pLocation;
	SPoint m_pDrawPoint;
	SPoint m_pMapCoordinate;
	Bullet m_pBullet;
	int m_iBomb;
	int m_iAstarInterval;
	int StarLevel;
	bool isInvincible;
	int InvincibleTime;
	int MaxHP;
public:
	Tank(int id,Direction dir,int hp,int loc_x,int loc_y,int mvSpeed);
	int getID() {return m_iID;}                          //��ȡID
	Direction getDirection() {return m_dDirection;}      //��ȡ����
	void setDirection(Direction dir) {m_dDirection=dir;} //���÷���
	

	int getHP(){return m_iHP;}                        //����HP
	void increaseHP();                                //��������ֵ
	void decreaseHP(int n=1);                         //��������ֵ
	void increaseMaxHP(){MaxHP++;}

	bool getIsInvincible() {return isInvincible;}

	void setIsAlive(bool flag){m_bIsAlive = flag;}
	bool getIsAlive() {return m_bIsAlive;}

	Map_element* getPMapElement(){return m_pMapElement;} //���ص�ͼԪ��ָ��
	void setPMapElement(Map_element* pMap){m_pMapElement=pMap;}//���õ�ͼԪ��

	int MoveSpeed() {return m_iMoveSpeed;} //�����ƶ��ٶ�
	void increaseSpeed(){m_iMoveSpeed++;}
	void setSpeed(int sp){m_iMoveSpeed=sp;}

	SPoint Location() {return m_pLocation;}				//����λ��
	SPoint MapCoordinate(){return m_pMapCoordinate;}   //���ص�ͼ������
	virtual void setLocationX(int x)=0;              //���õص�����X
	virtual void setLocationY(int y)=0;               //���õص�����Y
	SPoint DrawPoint() {return m_pDrawPoint;}			//���ػ�ͼ��

	virtual HINSTANCE getHinstance()=0;
	Bullet getBullet(){return m_pBullet;}

	void virtual Update()=0;							//��������
	void virtual Paint(HDC &hdc)=0;								//����tank


	virtual int getScenario(int i){return 0;}
	virtual void setScenario(int i,int val){}
	virtual int getFitness(){return 0;}
	virtual void setFitness(int i){}
	virtual void increaseShoot(){}
	virtual int getShoot(){return 0;}
	virtual void setHBMP(){};
};
#endif
#include "MapElement.h"
#include "LightTank.h"
#include "Function.h"
#include "Assassin.h"
#include "Tank.h"
#include <ctime>

extern Tank* pLTank;
extern Tank* enemys[10];
extern Tank* newEnemy[20];
bool isTouched(Tank*,Tank*);
const int excursionA = 3;
//定位所在地形
bool WhichMapElement(Tank& tank)      //头部探测
{
	SPoint point = tank.DrawPoint();
	SPoint left_Side_Point = point;
	SPoint right_Side_Point = point;   //定义地形探测点
	int x = point.getX();
	int y = point.getY();
	bool isFrontBarrier = false;
	if (tank.getID()==0||tank.getID()==2)
	{
		switch (tank.getDirection())  //确定探测点
		{
		case FRONT:
			point.setY(y+22);
			point.setX(x+25);
			left_Side_Point.setY(y+22);
			left_Side_Point.setX(x+7);
			right_Side_Point.setY(y+22);
			right_Side_Point.setX(x+43);
			if (tank.Location().getY()<=23)
			{
				isFrontBarrier = true;
			}
			break;
		case RETREAT:
			point.setY(y+70);
			point.setX(x+25);
			left_Side_Point.setY(y+70);
			left_Side_Point.setX(x+43);
			right_Side_Point.setY(y+70);
			right_Side_Point.setX(x+7);
			if (tank.Location().getY()>=g_rect.bottom-28)
			{
				isFrontBarrier = true;
			}
			break;
		case LEFT:
			point.setY(y+42);
			point.setX(x-2);
			left_Side_Point.setY(y+61);
			left_Side_Point.setX(x-2);
			right_Side_Point.setY(y+32);
			right_Side_Point.setX(x-2);
			if (tank.Location().getX()<=25)
			{
				isFrontBarrier = true;
			}
			break;
		case RIGHT:
			point.setX(x+52);
			point.setY(y+42);
			left_Side_Point.setX(x+52);
			left_Side_Point.setY(y+32);
			right_Side_Point.setX(x+52);
			right_Side_Point.setY(y+61);
			if (tank.Location().getX()>=g_rect.right-25)
			{
				isFrontBarrier = true;
			}
			break;
		}
	}
	else
	{
		switch (tank.getDirection())  //确定探测点
		{
		case FRONT:
			point.setY(y+excursionA);
			point.setX(x+25);
			left_Side_Point.setY(y+excursionA);
			left_Side_Point.setX(x+excursionA);
			right_Side_Point.setY(y+excursionA);
			right_Side_Point.setX(x+50-excursionA);
			if (tank.Location().getY()<=23)
			{
				isFrontBarrier = true;
			}
			break;
		case RETREAT:
			point.setY(y+50-excursionA);
			point.setX(x+25);
			left_Side_Point.setY(y+50-excursionA);
			left_Side_Point.setX(x+50-excursionA);
			right_Side_Point.setY(y+50-excursionA);
			right_Side_Point.setX(x+excursionA);
			if (tank.Location().getY()>=g_rect.bottom-28)
			{
				isFrontBarrier = true;
			}
			break;
		case LEFT:
			point.setY(y+25);
			point.setX(x-excursionA);
			left_Side_Point.setY(y+50-excursionA);
			left_Side_Point.setX(x-excursionA);
			right_Side_Point.setY(y+excursionA);
			right_Side_Point.setX(x-excursionA);
			if (tank.Location().getX()<=25)
			{
				isFrontBarrier = true;
			}
			break;
		case RIGHT:
			point.setX(x+50-excursionA);
			point.setY(y+25);
			left_Side_Point.setX(x+50-excursionA);
			left_Side_Point.setY(y+excursionA);
			right_Side_Point.setX(x+50-excursionA);
			right_Side_Point.setY(y+50-excursionA);
			if (tank.Location().getX()>=g_rect.right-25)
			{
				isFrontBarrier = true;
			}
			break;
		}
	}
	
	switch (map[(g_roundNumber-1)%5][point.getY()/64][point.getX()/64])   //根据地图信息得出所在地形
	{
	case 101:case 109: case 110:  case 113: case 114: case 115: case 116:
		tank.setPMapElement(Brick1::Instance());
		break;
	case 102:
		tank.setPMapElement(Brick2::Instance());
		break;
	case 103:
		tank.setPMapElement(Floor::Instance());
		break;
	case 104:
		tank.setPMapElement(Grass::Instance());
		break;
	case 105:
		tank.setPMapElement(Ice::Instance());
		break;
	case 106:
		tank.setPMapElement(Sand::Instance());
		break;
	case 107:
		tank.setPMapElement(Snow::Instance());
		break;
	case 108:
		tank.setPMapElement(Water::Instance());
		break;
	case 111: case 112:
		tank.setPMapElement(Forest::Instance());
		break;
	default:
		break;
	}
    //敌我双方相遇情况
	if (tank.getID()==0)                //检测
	{
		if (!enemys[0]->getIsInvincible()&&enemys[0]->getIsAlive()&&enemys[0]->getHP()>0&&isTouched(&tank,enemys[0]))  //我方坦克 遇 碰撞车
		{
			tank.decreaseHP();
			enemys[0]->decreaseHP();
			isFrontBarrier = true;
		}
		for (int i=0;i<g_enemyNumber;i++)                     //我方坦克 遇 敌方攻击坦克
		{
			if (newEnemy[i]->getIsAlive()&&newEnemy[i]->getHP()>0&&isTouched(&tank,newEnemy[i]))
			{
				isFrontBarrier = true;
			}
		}
	}
	else if (tank.getID()==1)   //Assassin
	{
		if (enemys[0]->getHP()&&isTouched(&tank,pLTank)&&!tank.getIsInvincible())                //碰撞车 遇 我方坦克
		{
			tank.decreaseHP();
			pLTank->decreaseHP();
			isFrontBarrier = true;
		}
		for (int i=0;i<g_enemyNumber;i++)          //碰撞车 遇 敌方坦克
		{
			if (newEnemy[i]->getIsAlive()&&newEnemy[i]->getHP()>0&&isTouched(&tank,newEnemy[i]))
			{
				isFrontBarrier = true;
			}
		}

		
	}
	else if (tank.getID()==2)  //敌方攻击坦克
	{

		if (isTouched(&tank,pLTank))  //敌方攻击坦克 遇 我方坦克
		{
			isFrontBarrier = true;
		}
		else if (enemys[0]->getHP()>0&&enemys[0]->getIsAlive()&&isTouched(&tank,enemys[0]))  //敌方攻击坦克 遇 碰撞车
		{
			isFrontBarrier = true;
		}
		for (int i=0;i<g_enemyNumber;i++)
		{
			if (newEnemy[i]!=&tank&&newEnemy[i]->getIsAlive()&&newEnemy[i]->getHP()>0&&isTouched(&tank,newEnemy[i]))
			{
				isFrontBarrier = true;
			}			
		}
	}
	//if (isTouched(&tank,enemys[0])&&tank.getID()!=enemys[0]->getID())      
	//{
	//	tank.decreaseHP(3);
	//	isFrontBarrier = true;
	//}
	//if (isTouched(&tank,pLTank)&&tank.getID()!=pLTank->getID())
	//{
	//	tank.decreaseHP(3);
	//	isFrontBarrier = true;
	//}
	

	int left = map[(g_roundNumber-1)%5][left_Side_Point.getY()/64][left_Side_Point.getX()/64];
	int right = map[(g_roundNumber-1)%5][right_Side_Point.getY()/64][right_Side_Point.getX()/64];
	if (left==101||left==102||left==108||right==101||right==102||right==108||(left>=109&&left!=111&&left!=112&&left<=116)||(right>=109&&right!=111&&right!=112&&right<=116))//前方有障碍
	{
		isFrontBarrier = true;
	}
	return isFrontBarrier;
}

//草坪类相关成员函数
Grass* Grass::Instance()
{
	static Grass instance;
	return &instance;
}

void Grass::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(locationY-pTank->MoveSpeed());
		break;
	case RETREAT:
		pTank->setLocationY(locationY+pTank->MoveSpeed());
		break;
	case LEFT:
		pTank->setLocationX(locationX-pTank->MoveSpeed());
		break;
	case RIGHT:
		pTank->setLocationX(locationX+pTank->MoveSpeed());
		break;
	default:
		break;
	}
}

//砖块1类相关成员函数
Brick1* Brick1::Instance()
{
	static Brick1 instance;
	return &instance;
}

void Brick1::Move(Tank *pTank)
{
}

//砖块2类相关成员函数
Brick2* Brick2::Instance()
{
	static Brick2 instance;
	return &instance;
}

void Brick2::Move(Tank *pTank)
{
}

//地板类相关成员函数
Floor* Floor::Instance()
{
	static Floor instance;
	return &instance;
}

void Floor::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(static_cast<int>(locationY-pTank->MoveSpeed()*0.8));
		break;
	case RETREAT:
		pTank->setLocationY(static_cast<int>(locationY+pTank->MoveSpeed()*0.8));
		break;
	case LEFT:
		pTank->setLocationX(static_cast<int>(locationX-pTank->MoveSpeed()*0.8));
		break;
	case RIGHT:
		pTank->setLocationX(static_cast<int>(locationX+pTank->MoveSpeed()*0.8));
		break;
	default:
		break;
	}
}

//冰面类相关成员函数
Ice* Ice::Instance()
{
	static Ice instance;
	return &instance;
}

void Ice::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(static_cast<int>(locationY-pTank->MoveSpeed()*1.5));
		break;
	case RETREAT:
		pTank->setLocationY(static_cast<int>(locationY+pTank->MoveSpeed()*1.5));
		break;
	case LEFT:
		pTank->setLocationX(static_cast<int>(locationX-pTank->MoveSpeed()*1.5));
		break;
	case RIGHT:
		pTank->setLocationX(static_cast<int>(locationX+pTank->MoveSpeed()*1.5));
		break;
	default:
		break;
	}
}

//沙漠类相关成员函数
Sand* Sand::Instance()
{
	static Sand instance;
	return &instance;
}

void Sand::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(static_cast<int>(locationY-pTank->MoveSpeed()*0.5));
		break;
	case RETREAT:
		pTank->setLocationY(static_cast<int>(locationY+pTank->MoveSpeed()*0.5));
		break;
	case LEFT:
		pTank->setLocationX(static_cast<int>(locationX-pTank->MoveSpeed()*0.5));
		break;
	case RIGHT:
		pTank->setLocationX(static_cast<int>(locationX+pTank->MoveSpeed()*0.5));
		break;
	default:
		break;
	}
}

//雪地类相关成员函数
Snow* Snow::Instance()
{
	static Snow instance;
	return &instance;
}

void Snow::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	srand((unsigned)time(nullptr));
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(static_cast<int>(locationY-(rand()%pTank->MoveSpeed()+1)));
		break;
	case RETREAT:
		pTank->setLocationY(static_cast<int>(locationY+(rand()%pTank->MoveSpeed()+1)));
		break;
	case LEFT:
		pTank->setLocationX(static_cast<int>(locationX-(rand()%pTank->MoveSpeed()+1)));
		break;
	case RIGHT:
		pTank->setLocationX(static_cast<int>(locationX+(rand()%pTank->MoveSpeed()+1)));
		break;
	default:
		break;
	}
}

//河流类相关成员函数
Water* Water::Instance()
{
	static Water instance;
	return &instance;
}

void Water::Move(Tank *pTank)
{
}

//森林类相关成员函数
Forest* Forest::Instance()
{
	static Forest instance;
	return &instance;
}

void Forest::Move(Tank *pTank)
{
	SPoint point = pTank->Location();
	int locationX = point.getX();
	int locationY = point.getY();
	/*srand((unsigned)time(nullptr));*/
	//改变步伐
	switch (pTank->getDirection())
	{
	case FRONT:
		pTank->setLocationY(static_cast<int>(locationY-(rand()%pTank->MoveSpeed())));
		break;
	case RETREAT:
		pTank->setLocationY(static_cast<int>(locationY+(rand()%pTank->MoveSpeed())));
		break;
	case LEFT:
		pTank->setLocationX(static_cast<int>(locationX-(rand()%pTank->MoveSpeed())));
		break;
	case RIGHT:
		pTank->setLocationX(static_cast<int>(locationX+(rand()%pTank->MoveSpeed())));
		break;
	default:
		break;
	}
}
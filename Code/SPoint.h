#ifndef SPOINT
#define SPOINT
#include "Direction.h"
class SPoint
{
private:
	int x;
	int y;

public:
	SPoint(int ix=0,int iy=0):x(ix),y(iy){}
	int getX()const{return x;}                //定义成const类型，使得普通对象和常量对象都可以使用该成员函数，否则const SPoint& sp ,sp.getX()会发生错误
	void setX(int x){this->x=x;}
	int getY()const{return y;}
	void setY(int y){this->y=y;}

};

class APoint
{
private:
	int x;
	int y;
	int cost;
	int toHereCost;
	int ShootTimes;
	Direction ToSourceDirection;
public:
	APoint(int ix,int iy,int icost=0,int tcost=0,Direction dir=FRONT ):x(ix),y(iy),cost(icost),toHereCost(tcost),ToSourceDirection(dir),ShootTimes(0){}
	APoint(SPoint sp,int icost=0,int tcost=0,Direction dir=FRONT):x(sp.getX()),y(sp.getY()),cost(icost),toHereCost(tcost),ToSourceDirection(dir),ShootTimes(0){}

	int getShootTimes(){return ShootTimes;}
	void increaseShootTimes(){ShootTimes++;}
	bool operator==(const APoint& Ap)
	{
		return (x==Ap.x&&y==Ap.y);
	}
	int getX()const{return x;}
	void setX(int x){this->x=x;}
	int getY()const{return y;}
	void setY(int y){this->y=y;}
	int getCost()const{return cost;}
	void setCost(int cost){this->cost=cost;}
	int getToHereCost()const{return toHereCost;}
	void setToHereCost(int tcost){toHereCost=tcost;}
	Direction getToSourceDirection()const{return ToSourceDirection;}
	void setDirection(Direction dir){ToSourceDirection=dir;}
};

#endif
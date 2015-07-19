#ifndef ASSASSIN
#define ASSASSIN
#include <windows.h>
#include "MapElement.h"
#include "Function.h"
#include "Direction.h"
#include "Tank.h"

extern HBITMAP g_hAssassin[4];
extern int g_roundNumber;
extern HBITMAP g_hInitialize[21];
class Tank;
void AStar(Tank*assassin,Tank*ltank);
class Assassin :public Tank
{
private:
	HINSTANCE hinstance;
	static int m_framNumber;
	HBITMAP m_hAssassin;
public:
	Assassin(int id=1,Direction dir=FRONT,int hp=1,int loc_x=480,int loc_y=38,HINSTANCE hinst=NULL,int mvSpeed=7);//
	void setLocationX(int x);
	void setLocationY(int y);
	void Update();
	void Adjust(Direction dir);
	void setHBMP();
	void Paint(HDC &hmdc);
	HINSTANCE getHinstance(){return hinstance;}
	static int getFramNumber() {return m_framNumber;}	//获取帧数
	static void setFramNumber(int num){m_framNumber=num;}//设置帧数
	static void increaseFramNumber();					//更新帧数
	static void intializeFrame() {m_framNumber=0; }      //帧数初始化
	
};
#endif
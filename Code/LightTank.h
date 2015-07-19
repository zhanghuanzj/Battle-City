#ifndef LIGHT_TANK
#define LIGHT_TANK
#include <windows.h>
#include <fstream>
#include "Tank.h"
using namespace std;
class MapElement;
const int TankNumber = 1;
extern int g_score;
extern int g_score_pre;
extern int g_HistoryBestScore;
extern HBITMAP g_hBloodBar;
extern HBITMAP g_hLevelUp[26];
extern bool IsLevelUP;
class LightTank : public Tank
{
private:
	HINSTANCE hinstance;
	HBITMAP m_hBMP;
	static int m_framNumber;
	int BulletRest;
public:
	LightTank(int id=0,Direction dir=FRONT,int hp=3,int loc_x=400,int loc_y=400,HINSTANCE hinst=NULL,int mvSpeed=6);//
	HINSTANCE getHinstance(){return hinstance;}
	void setLocationX(int x);
	void setLocationY(int y);
	void Update();
	void MoveBack();
	void Paint(HDC &hmdc);
	void BulletShoot();
	void LevelUP(HDC &hmdc);

	static int getFramNumber() {return m_framNumber;}	//获取帧数
	static void setFramNumber(int num){m_framNumber=num;}//设置帧数
	static void increaseFramNumber();					//更新帧数
	static void intializeFrame() {m_framNumber=0; }      //帧数初始化
};
#endif // !LIGHT_TANK

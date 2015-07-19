#ifndef DEFINE
#define DEFINE
#include <windows.h>
#include <vector>
#include "resource.h"
#include "LightTank.h"
#include "Assassin.h"
#include "EnemyTank.h"
#include "Bullet.h"
#include "MapElement.h"
#include "SPoint.h"
using namespace std;

#define WINDOW_WIDTH  980
#define WINDOW_HEIGHT 1000
#define MAPS_NUMBER  16
#define TANK_NUMBER  1
#define BRICK1  IDB_BITMAP1
#define BRICK2  IDB_BITMAP2  
#define FLOOR   IDB_BITMAP3 
#define GRASS	IDB_BITMAP4
#define ICE  	IDB_BITMAP5
#define SAND	IDB_BITMAP6
#define SNOW	IDB_BITMAP7
#define WATER	IDB_BITMAP8
#define WATERN  IDB_BITMAP29
#define SNOWN   IDB_BITMAP30
#define SANDW   IDB_BITMAP31
#define ICEN    IDB_BITMAP32
#define GRASSN  IDB_BITMAP33
#define FLOORN  IDB_BITMAP34
#define BRICKN  IDB_BITMAP35

#define BROKEN_BUILDING    IDB_BITMAP117                 
#define FIRE_BUILDING      IDB_BITMAP118                  
#define FOREST             IDB_BITMAP119                   
#define FOREST_SNOW        IDB_BITMAP120                   
#define HILL               IDB_BITMAP121                  
#define MOUNTAIN           IDB_BITMAP122                  
#define MOUNTAIN_SONW      IDB_BITMAP123                   
#define MOUNTAIN_ZHANG     IDB_BITMAP124                   

#define Missile_RIGHT IDB_BITMAP36       //Missile RIGHT
#define Missile_DOWN  IDB_BITMAP37       //Missile DOWN
#define Missile_LEFT  IDB_BITMAP38       //Missile LEFT
#define Missile_UP    IDB_BITMAP39       //Missile UP

#define Missile_EX1   IDB_BITMAP40      
#define Missile_EX2   IDB_BITMAP45 
#define Missile_EX3   IDB_BITMAP46      
#define Missile_EX4   IDB_BITMAP47 
#define Missile_EX5   IDB_BITMAP48      
#define Missile_EX6   IDB_BITMAP49 
#define Missile_EX7   IDB_BITMAP50      
#define Missile_EX8   IDB_BITMAP51 

#define Assassin_1    IDB_BITMAP41 
#define Assassin_2    IDB_BITMAP42 
#define Assassin_3    IDB_BITMAP43 
#define Assassin_4    IDB_BITMAP44 

#define TANK_INITIALIZE1 IDB_BITMAP52
#define TANK_INITIALIZE2 IDB_BITMAP53
#define TANK_INITIALIZE3 IDB_BITMAP54
#define TANK_INITIALIZE4 IDB_BITMAP55
#define TANK_INITIALIZE5 IDB_BITMAP56
#define TANK_INITIALIZE6 IDB_BITMAP57
#define TANK_INITIALIZE7 IDB_BITMAP58
#define TANK_INITIALIZE8 IDB_BITMAP59
#define TANK_INITIALIZE9 IDB_BITMAP60
#define TANK_INITIALIZE10 IDB_BITMAP61
#define TANK_INITIALIZE11 IDB_BITMAP62
#define TANK_INITIALIZE12 IDB_BITMAP63
#define TANK_INITIALIZE13 IDB_BITMAP64
#define TANK_INITIALIZE14 IDB_BITMAP65
#define TANK_INITIALIZE15 IDB_BITMAP66
#define TANK_INITIALIZE16 IDB_BITMAP67
#define TANK_INITIALIZE17 IDB_BITMAP68
#define TANK_INITIALIZE18 IDB_BITMAP69
#define TANK_INITIALIZE19 IDB_BITMAP70
#define TANK_INITIALIZE20 IDB_BITMAP71
#define TANK_INITIALIZE21 IDB_BITMAP72

#define GAMEOVER_BG  IDB_BITMAP73    
#define REPLAYBUTTON IDB_BITMAP74 
#define GAMEBEGIN    IDB_BITMAP75

#define ROUNDBG_01    IDB_BITMAP76                    
#define ROUNDBG_02    IDB_BITMAP77                    
#define ROUNDBG_03    IDB_BITMAP78                  
#define ROUNDBG_04    IDB_BITMAP79                    
#define ROUNDBG_05    IDB_BITMAP80                  
#define ROUNDBG_06    IDB_BITMAP81                 
#define ROUNDBG_07    IDB_BITMAP82                    
#define ROUNDBG_08    IDB_BITMAP83                    
#define ROUNDBG_09    IDB_BITMAP84                 
#define ROUNDBG_10    IDB_BITMAP85                   
#define ROUNDBG_11    IDB_BITMAP86                   
#define ROUNDBG_12    IDB_BITMAP87                   
#define ROUNDBG_13    IDB_BITMAP88                    
#define ROUNDBG_14    IDB_BITMAP89                    

#define BloodBar      IDB_BITMAP90

#define LEVELUP01     IDB_BITMAP91                    
#define LEVELUP02     IDB_BITMAP92                    
#define LEVELUP03     IDB_BITMAP93                    
#define LEVELUP04     IDB_BITMAP94                    
#define LEVELUP05     IDB_BITMAP95                    
#define LEVELUP06     IDB_BITMAP96                    
#define LEVELUP07     IDB_BITMAP97                    
#define LEVELUP08     IDB_BITMAP98                    
#define LEVELUP09     IDB_BITMAP99                   
#define LEVELUP10     IDB_BITMAP100                   
#define LEVELUP11     IDB_BITMAP101                   
#define LEVELUP12     IDB_BITMAP102                  
#define LEVELUP13     IDB_BITMAP103                   
#define LEVELUP14     IDB_BITMAP104                   
#define LEVELUP15     IDB_BITMAP105                   
#define LEVELUP16     IDB_BITMAP106                  
#define LEVELUP17     IDB_BITMAP107                  
#define LEVELUP18     IDB_BITMAP108                  
#define LEVELUP19     IDB_BITMAP109                  
#define LEVELUP20     IDB_BITMAP110                   
#define LEVELUP21     IDB_BITMAP111                   
#define LEVELUP22     IDB_BITMAP112                  
#define LEVELUP23     IDB_BITMAP113                  
#define LEVELUP24     IDB_BITMAP114                  
#define LEVELUP25     IDB_BITMAP115                   
#define LEVELUP26     IDB_BITMAP116                   

#define LIGHTTANK IDB_BITMAP9

HBITMAP g_hBG[MAPS_NUMBER]; //地图句柄
HBITMAP g_hGameOver ;       //游戏结束画面
HBITMAP g_hBullet[4];
HBITMAP g_hBulletEx[10];
HBITMAP g_hInitialize[21];
HBITMAP g_hRoundBG[14];
HBITMAP g_hLevelUp[26];
HBITMAP g_hBomb[8];
HBITMAP g_hEnemys[3];
HBITMAP g_hLTank ;
HBITMAP g_hELife;
HBITMAP g_hAssassin[4];
HBITMAP g_hGameOverBG;
HBITMAP g_hRepalyButton;
HBITMAP g_hGameBegin;
HBITMAP g_hBloodBar;
HINSTANCE g_hinstance;
HDC g_hdc,g_hbufdc,g_hmdc;  //环境上下文句柄
DWORD g_tpre,g_tnow;        //刷新时间记录      
DWORD startTime,endTime;
bool AssassinFlag;
bool GameOver;
bool GameBegin;
bool RoundLoading;
bool IsLevelUP;
int g_enemyNumber =20;
int g_turnNumber;
int g_score;
int g_score_pre;
int g_HistoryBestScore;
int g_roundNumber;
int g_maps[MAPS_NUMBER]={BRICKN,//地图资源文件
	BRICKN,
	FLOORN,
	GRASSN,
	ICEN,
	SANDW,
	SNOWN,
	WATERN,
	BROKEN_BUILDING,
	FIRE_BUILDING,
	FOREST,
	FOREST_SNOW,
	HILL,
	MOUNTAIN,
	MOUNTAIN_SONW,
	MOUNTAIN_ZHANG,
};    
 
#define mapNumber 5
                      /* 1   2   3   4   5   6   7   8   9  10   11  12  13  14  15*/
int map[5][15][15]={

	                {                    //开始
	                  {104,104,104,104,104,104,104,104,104,104,104,101,101,104,104},//1
	                  {104,101,104,105,105,105,104,104,104,104,103,103,103,103,103},//2
	                  {104,101,104,108,108,108,103,108,108,108,103,103,103,103,103},//3
	                  {107,107,107,108,102,102,104,104,104,108,106,101,101,101,101},//4
	                  {107,107,107,103,104,104,104,104,104,108,106,106,106,106,106},//5
	                  {107,107,105,108,104,104,104,101,104,108,106,106,106,106,106},//6
	                  {107,107,105,108,104,104,104,102,104,103,106,106,106,106,106},//7
	                  {101,101,105,108,104,104,104,104,104,108,106,106,104,104,104},//8
	                  {105,105,105,108,108,108,108,108,103,108,104,104,104,104,104},//9
	                  {105,105,105,105,101,101,102,102,103,103,103,104,104,104,104},//10
	                  {104,103,103,103,102,102,101,101,103,103,103,108,108,102,104},//11
	                  {104,105,104,104,104,104,104,104,104,103,103,102,108,102,104},//12
	                  {105,105,105,104,104,106,106,106,103,103,103,102,106,108,104},//13
	                  {104,105,102,104,106,103,103,103,103,103,106,106,106,106,106},//14
	                  {104,105,102,104,104,104,104,104,104,106,106,104,104,104,104} //15
					},
				  {                 //雪地
	              {107,107,105,107,107,107,107,103,107,107,107,107,107,107,107},//1
				  {105,105,105,105,107,107,107,103,107,107,107,112,115,107,107},//2
				  {105,105,105,105,107,107,107,103,107,107,112,115,115,107,107},//3
				  {105,105,105,115,115,107,107,103,107,112,115,115,112,112,112},//4
				  {107,107,105,115,107,107,107,103,112,115,115,112,112,112,112},//5
				  {107,107,105,107,107,107,107,103,115,115,112,112,112,112,107},//6
				  {103,103,115,103,103,103,103,103,103,103,103,103,103,115,107},//7
				  {107,115,115,107,107,115,115,103,112,112,112,112,112,115,115},//8
				  {107,107,107,107,115,115,112,103,112,112,112,112,112,105,105},//9
				  {107,107,107,115,115,112,107,103,112,112,112,115,105,105,105},//10
				  {107,107,115,115,112,107,107,103,112,112,115,115,115,105,107},//11
				  {107,115,115,112,107,107,107,103,112,115,115,107,107,105,107},//12
			   	  {107,115,112,107,107,107,107,103,112,112,107,107,107,105,107},//13
				  {107,107,107,107,112,112,112,103,112,107,107,107,107,105,107},//14
				  {107,107,107,107,107,107,107,103,107,107,107,107,107,105,107} //15
                  },
					{  /* 1   2   3   4   5   6   7   8   9  10   11  12  13  14  15*/         //草地
						{104,104,104,116,116,116,104,104,104,104,104,104,104,104,104},//1
						{104,104,104,104,104,104,104,104,104,104,116,104,104,104,104},//2
						{104,104,104,116,116,104,104,104,116,116,116,116,104,104,104},//3
						{104,104,104,116,116,116,116,104,116,116,116,104,104,104,113},//4
						{104,104,104,104,116,116,116,104,116,104,104,104,104,113,113},//5
						{104,113,113,104,116,116,116,104,104,104,108,108,104,104,104},//6
						{104,113,113,104,104,104,104,104,104,104,104,108,108,104,104},//7
						{104,113,113,113,113,104,104,104,104,116,104,104,104,104,104},//8
						{104,111,111,111,113,113,113,104,104,116,116,116,104,104,116},//9
						{104,104,111,111,111,113,113,104,104,104,116,116,116,104,116},//10
						{104,104,111,111,111,113,113,104,104,104,116,116,116,104,116},//11
						{104,104,111,111,111,113,113,113,104,104,104,116,104,104,104},//12
						{104,104,104,104,104,104,104,104,104,108,104,104,104,104,104},//13
						{104,111,111,111,111,113,104,104,104,108,108,108,108,104,104},//14
						{104,111,111,111,111,113,113,104,104,104,108,108,108,108,104} //15
				  },
				  {  /* 1   2   3   4   5   6   7   8   9  10   11  12  13  14  15*/         //城镇
					  {103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},//1
					  {103,103,109,109,103,110,109,103,109,109,109,110,110,103,103},//2
					  {103,103,103,109,109,109,109,103,103,103,103,109,109,110,109},//3
					  {110,103,103,103,103,103,103,103,103,109,103,103,103,109,109},//4
					  {109,109,103,109,103,103,103,103,103,109,109,109,103,103,103},//5
					  {103,109,103,109,103,103,103,101,101,110,110,109,103,109,103},//6
					  {103,109,103,109,103,103,103,103,103,103,103,103,103,103,103},//7
					  {103,109,109,109,103,103,101,103,103,101,103,109,103,109,103},//8
					  {103,103,103,103,103,103,103,109,103,103,103,109,103,109,103},//9
					  {103,109,103,109,109,109,103,109,103,109,103,103,103,103,103},//10
					  {103,109,103,109,103,103,103,109,103,109,109,103,103,103,103},//11
					  {103,109,103,103,103,109,109,110,103,103,109,109,110,109,103},//12
					  {103,109,103,109,109,110,110,103,103,103,103,110,110,103,103},//13
					  {103,109,109,103,103,103,103,103,103,103,103,103,103,103,103},//14
					  {103,103,103,103,109,109,110,103,103,103,109,109,110,103,103} //15
				  },
				  {  /* 1   2   3   4   5   6   7   8   9  10   11  12  13  14  15*/         //沙漠
					  {106,106,114,114,114,106,106,106,106,106,106,114,114,114,106},//1
					  {106,106,114,114,106,106,114,114,114,114,106,106,114,114,106},//2
					  {106,106,106,114,106,114,114,114,114,114,114,106,114,106,106},//3
					  {114,114,106,104,104,114,106,106,106,106,114,104,104,106,106},//4
					  {114,114,106,104,104,104,106,106,106,106,104,104,114,106,106},//5
					  {106,106,106,114,114,104,106,106,106,106,106,106,114,114,106},//6
					  {106,106,114,114,106,106,106,106,106,106,106,106,114,114,106},//7
					  {106,106,114,114,106,106,106,106,106,106,106,106,114,114,106},//8
					  {114,106,114,114,106,106,106,106,106,106,106,106,114,114,106},//9
					  {114,106,114,114,106,106,106,106,106,106,106,106,114,114,106},//10
					  {114,106,114,114,106,106,106,106,106,106,106,104,114,106,106},//11
					  {114,106,114,114,104,104,106,106,106,106,106,104,104,104,106},//12
					  {106,106,106,114,104,114,114,114,114,114,114,114,104,114,114},//13
					  {106,114,106,104,104,104,114,114,114,114,114,104,104,106,114},//14
					  {106,114,114,114,106,106,106,106,106,106,106,106,106,106,106} //15
				  }

}; //屏幕地图块数组

                  /* 1   2   3   4   5   6   7   8   9  10   11  12  13  14  15*/
//int map[15][15]={
//}; //屏幕地图块数组

RECT g_rect;     //记录屏幕矩形信息

Tank *pLTank;  //操控坦克指针
Tank *enemys[10]; //敌人坦克部队
Tank *newEnemy[20];
Tank *oldEnemy[20];

vector<APoint> ShootedPlace;


//函数声明
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool Game_init(HWND hwnd,HINSTANCE hinstance);
void Game_paint(HWND hwnd);
void Game_prePaint(HWND hwnd);
bool Game_clean(HWND hwnd);

#endif // !DEFINE

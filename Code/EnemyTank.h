#ifndef ENEMYTANK
#define ENEMYTANK
#include "GAlgorithm.h"
#include <cmath>
#include <time.h>
#include <algorithm>

#include "Tank.h"
using namespace std;
extern Tank *newEnemy[20];
extern Tank *oldEnemy[20];
extern HBITMAP g_hInitialize[21];
extern Tank *pLTank;
extern HBITMAP g_hEnemys[3];
extern HDC g_hmdc;

class MapElement;


class EnemyTank : public Tank
{
private:
	HINSTANCE hinstance;
	HBITMAP m_hBMP;
	static int m_framNumber;
	int BulletRest;
	int RandomRest;
	/*GA*/
	int Scenario[10];
	int Shoot;
	int m_fitness;


public:
	/*GA*/
	int getScenario(int i){return Scenario[i];}
	void setScenario(int i,int val){Scenario[i]=val;}

	int getFitness(){return m_fitness;}
	void setFitness(int fit){m_fitness = fit;}

	void increaseShoot(){Shoot++;}
	int getShoot(){return Shoot;}

    static void Fitness(int TID);
	static bool compareEnemy(Tank *T1,Tank *T2)
	{
		return T1->getFitness()>T2->getFitness();
	}
	static void CrossOver(int newT,int oldT1,int oldT2);
	static void Mutation(int TID);
	static void Creature(int TID);
	static void SortEnemy();

	void OtherHandle();
	void AStarHandle();
	void Smart(int sIndex);
	void Attack(int sIndex);
	void Random();
	void BulletShoot();
	

	bool IsFaceBullet();
	bool IsNearBullet();
	bool IsWillShot();
	bool IsWillBeShot();
	bool IsSneakAttack();
	bool IsFaceAttack();
	bool IsBeSneakAttack();
	bool IsWander();

	int InWhichScenario();
	/*GA*/


	EnemyTank(int id=0,Direction dir=FRONT,int hp=3,int loc_x=480,int loc_y=32,HINSTANCE hinst=NULL,int mvSpeed=6);//
	HINSTANCE getHinstance(){return hinstance;}
	void setLocationX(int x);
	void setLocationY(int y);
	void Update();
	void Paint(HDC &hmdc);

	 void setHBMP();
	static int getFramNumber() {return m_framNumber;}	//获取帧数
	static void setFramNumber(int num){m_framNumber=num;}//设置帧数
	static void increaseFramNumber();					//更新帧数
	static void intializeFrame() {m_framNumber=0; }      //帧数初始化
};
#endif
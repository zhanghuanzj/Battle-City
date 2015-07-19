#ifndef FUNCTION
#define FUNCTION
#include <cmath>
#include <vector>
using namespace std;
class Bullet;
class Tank;
class LightTank;
class APoint;

extern int map[5][15][15];
extern Tank* newEnemy[20];
bool isTouched(Tank*T1,Tank*T2);
bool isTouched(Bullet *B,Tank *T);
bool isTouched(Bullet *B1,Bullet *B2);
void AStarWeightAdjust(vector<APoint> &vec,Tank*ltank);
void AStarWithBulletWeightAdjust(vector<APoint>& vec,Tank*ltank);
void SNBAStar(Tank*assassin,Tank*ltank);
void NBAStar(Tank*assassin,Tank*ltank);
void NAStar(Tank*assassin,Tank*ltank);
void AStar(Tank*enemy,Tank*ltank);
int MapValue(int x,int y);
int Heuristic(const APoint& ap,const APoint& sp);
bool compareAP(const APoint& ap1,const APoint& ap2);
#endif
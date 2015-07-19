#include "Function.h"
#include "Tank.h"
#include "Assassin.h"
#include "LightTank.h"
#include "Bullet.h"
#include <cmath>
#include <list>
#include <algorithm>
using namespace std;
extern int map[5][15][15];
extern vector<APoint> ShootedPlace;
const int CANTMOVE = 0x0fffffff;
const int CANTACROSS = 0x0ffffff0;
const int DANGEROUS = 15;
const int BULLETDANGEROUS = 20;

bool isTouched(Tank*T1,Tank*T2)
{
	int T1X=T1->Location().getX();
	int T1Y=T1->Location().getY();
	int T2X=T2->Location().getX();
	int T2Y=T2->Location().getY();
	if (T1->getID()==1)
	{
		return (pow(T2X-T1X,2)+pow(T2Y-T1Y,2)<=2500);
	}
	else if (pow(T2X-T1X,2)+pow(T2Y-T1Y,2)<=2500)
	{
		if (T1X<T2X&&T1->getDirection()==RIGHT)
		{
			return true;
		}
		else if(T1X>T2X&&T1->getDirection()==LEFT)
		{
			return true;
		}
		else if(T1Y<T2Y&&T1->getDirection()==RETREAT)
		{
			return true;
		}
		else if (T1Y>T2Y&&T1->getDirection()==FRONT)
		{
			return true;
		}
	}
	return false;
}

bool isTouched(Bullet *B,Tank *T)
{
	int BX=B->Location().getX();
	int BY=B->Location().getY();
	int TX=T->Location().getX();
	int TY=T->Location().getY();
	return (pow(BX-TX,2)+pow(BY-TY,2)<=900);
}

bool isTouched(Bullet *B1,Bullet *B2)
{
	int B1X=B1->Location().getX();
	int B1Y=B1->Location().getY();
	int B2X=B2->Location().getX();
	int B2Y=B2->Location().getY();
	return (pow(B1X-B2X,2)+pow(B1Y-B2Y,2)<=225);
}

int MapValue(int x,int y)   //����Ȩֵ
{
	int val=0;
	if (x<1||x>15||y<1||y>15)  // Խ��ֱ�Ӵ���
	{
		return CANTMOVE;
	}
	switch (map[(g_roundNumber-1)%5][y-1][x-1])
	{
	case 105:              //����
		val=1;
		break;
	case 103:             //�ݵأ���ש
	case 104: 
		val=2;
		break;
	case 107:             //ѩ�أ�ɳ��
	case 106:
		val=3;
		break;
	case 101:            //ǽ��
	case 102:
	case 109: 
	case 110: 
	case 113: 
	case 114: 
	case 115: 
	case 116:
		val=CANTACROSS;
		break;
	case 108:
		val=CANTMOVE;   //����
		break;
	case 111: 
	case 112: 
		val=4;
		break;
	default:
		val=CANTMOVE;
		break;
	}
	for (int i=0;i<g_enemyNumber;i++)   //��̹�˿������޷������ģ��ƶ�Ȩֵ���ں�����
	{
		if (newEnemy[i]->getIsAlive()&&newEnemy[i]->getHP()>0&&newEnemy[i]->Location().getX()==x&&newEnemy[i]->Location().getY()==y)
		{
			val = CANTMOVE;
		}
	}
	return val;
} 

int Heuristic( const APoint& ap, const APoint& sp)    //��ȡ����ֵ
{
	return (abs(ap.getX()-sp.getX())+abs(ap.getY()-sp.getY()));
}

bool compareAP(const APoint& ap1,const APoint& ap2)  //�Ƚ�Ȩֵ
{
	return ap1.getCost()<ap2.getCost();
}

void AStarWeightAdjust(vector<APoint>& vec,Tank*ltank)
{
	int destX = ltank->MapCoordinate().getX();
	int destY = ltank->MapCoordinate().getY();
	switch (ltank->getDirection())
	{
	case FRONT:
		while (destY>1&&MapValue(destX,destY-1)!=CANTACROSS)
		{
			vec.push_back(APoint(destX,--destY));
		}
		break;
	case RETREAT:
		while (destY<15&&MapValue(destX,destY+1)!=CANTACROSS)
		{
			vec.push_back(APoint(destX,++destY));
		}
		break;
	case LEFT:
		while (destX>1&&MapValue(destX-1,destY)!=CANTACROSS)
		{
			vec.push_back(APoint(--destX,destY));
		}
		break;
	case RIGHT:
		while (destX<15&&MapValue(destX+1,destY)!=CANTACROSS)
		{
			vec.push_back(APoint(++destX,destY));
		}
		break;
	default:
		break;
	}
}

void AStarWithBulletWeightAdjust(vector<APoint>& vec,Tank*ltank)
{
	int bulletX = ltank->getBullet().Location().getX()/64+1;
	int bulletY = ltank->getBullet().Location().getY()/64+1;
	switch (ltank->getBullet().getDirection())
	{
	case FRONT:
		while (bulletY>0&&MapValue(bulletX,bulletY)!=CANTACROSS)
		{
			vec.push_back(APoint(bulletX,bulletY--));
		}
		break;
	case RETREAT:
		while (bulletY<16&&MapValue(bulletX,bulletY)!=CANTACROSS)
		{
			vec.push_back(APoint(bulletX,bulletY++));
		}
		break;
	case LEFT:
		while (bulletX>0&&MapValue(bulletX,bulletY)!=CANTACROSS)
		{
			vec.push_back(APoint(bulletX--,bulletY));
		}
		break;
	case RIGHT:
		while (bulletX<16&&MapValue(bulletX,bulletY)!=CANTACROSS)
		{
			vec.push_back(APoint(bulletX++,bulletY));
		}
		break;
	default:
		break;
	}
}
void SNBAStar(Tank*assassin,Tank*ltank)
{
	vector<APoint> vec_weight;
	vector<APoint> vec_bullet;
	AStarWeightAdjust(vec_weight,ltank);      //������·��Ȩֵ�ı�
	if (ltank->getBullet().getIsFly())
	{
		AStarWithBulletWeightAdjust(vec_bullet,ltank);  //�ӵ�·����Ȩֵ�ı�
	}
	
	APoint vfirst(assassin->MapCoordinate()); //���
	APoint vfinal(ltank->MapCoordinate());    //�յ�
	list<APoint> openlist;
	list<APoint> closelist;
	openlist.push_back(vfirst);        //���뵽openlist����ʾ�Ѿ�����
	while (!openlist.empty())
	{
		APoint vetex = openlist.front();
		openlist.pop_front();         // ��openlist��ɾ��
		if (vetex==vfinal)            //�ҵ�Ŀ�꣬ȷ����һ������
		{
			Direction first=FRONT;
			while (!(vetex==vfirst))
			{
				switch (vetex.getToSourceDirection())   //����
				{
				case FRONT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()-1));
					first = RETREAT;
					break;
				case RETREAT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()+1));
					first = FRONT;
					break;
				case LEFT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()-1,vetex.getY()));
					first = RIGHT;
					break;
				case RIGHT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()+1,vetex.getY()));
					first = LEFT;
					break;
				default:
					break;
				}
			}
			assassin->setDirection(first);
			return;
		}
		else                         //�����ĸ�����ڵ�
		{
			closelist.push_back(vetex);  //���뵽closelist�У���ʾ�Ѿ�����

			APoint up(vetex.getX(),vetex.getY()-1);    //up ����
			if (find(openlist.begin(),openlist.end(),up)==openlist.end()&&find(closelist.begin(),closelist.end(),up)==closelist.end()&&MapValue(up.getX(),up.getY())!=CANTMOVE&&MapValue(up.getX(),up.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(up.getX(),up.getY());
				if (find(vec_weight.begin(),vec_weight.end(),up)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),up)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				if (find(ShootedPlace.begin(),ShootedPlace.end(),up)!=ShootedPlace.end())  //�������ٵ�+��Ӧ���д���
				{
					vector<APoint>::iterator sp = find(ShootedPlace.begin(),ShootedPlace.end(),up);
					toCost += sp->getShootTimes();
				}
				int cost =toCost +Heuristic(up,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				up.setToHereCost(toCost);
				up.setCost(cost);
				up.setDirection(RETREAT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(up);        //���뵽openlist
			}

			APoint down(vetex.getX(),vetex.getY()+1);    //down ����
			if (find(openlist.begin(),openlist.end(),down)==openlist.end()&&find(closelist.begin(),closelist.end(),down)==closelist.end()&&MapValue(down.getX(),down.getY())!=CANTMOVE&&MapValue(down.getX(),down.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(down.getX(),down.getY());
				if (find(vec_weight.begin(),vec_weight.end(),down)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),down)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				if (find(ShootedPlace.begin(),ShootedPlace.end(),down)!=ShootedPlace.end())  //�������ٵ�+��Ӧ���д���
				{
					vector<APoint>::iterator sp = find(ShootedPlace.begin(),ShootedPlace.end(),down);
					toCost += sp->getShootTimes();
				}
				int cost = toCost+Heuristic(down,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				down.setToHereCost(toCost);
				down.setCost(cost);
				down.setDirection(FRONT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(down);        //���뵽openlist
			}

			APoint left(vetex.getX()-1,vetex.getY());    //left ����
			if (find(openlist.begin(),openlist.end(),left)==openlist.end()&&find(closelist.begin(),closelist.end(),left)==closelist.end()&&MapValue(left.getX(),left.getY())!=CANTMOVE&&MapValue(left.getX(),left.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(left.getX(),left.getY());
				if (find(vec_weight.begin(),vec_weight.end(),left)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),left)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				if (find(ShootedPlace.begin(),ShootedPlace.end(),left)!=ShootedPlace.end())  //�������ٵ�+��Ӧ���д���
				{
					vector<APoint>::iterator sp = find(ShootedPlace.begin(),ShootedPlace.end(),left);
					toCost += sp->getShootTimes();
				}
				int cost = toCost+Heuristic(left,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				left.setToHereCost(toCost);
				left.setCost(cost);
				left.setDirection(RIGHT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(left);        //���뵽openlist
			}

			APoint right(vetex.getX()+1,vetex.getY());    //right ����
			if (find(openlist.begin(),openlist.end(),right)==openlist.end()&&find(closelist.begin(),closelist.end(),right)==closelist.end()&&MapValue(right.getX(),right.getY())!=CANTMOVE&&MapValue(right.getX(),right.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(right.getX(),right.getY());
				if (find(vec_weight.begin(),vec_weight.end(),right)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),right)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				if (find(ShootedPlace.begin(),ShootedPlace.end(),right)!=ShootedPlace.end())  //�������ٵ�+��Ӧ���д���
				{
					vector<APoint>::iterator sp = find(ShootedPlace.begin(),ShootedPlace.end(),right);
					toCost += sp->getShootTimes();
				}
				int cost = toCost+Heuristic(right,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				right.setToHereCost(toCost);
				right.setCost(cost);
				right.setDirection(LEFT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(right);        //���뵽openlist
			}
			openlist.sort(compareAP);
		}
	}
}
void NBAStar(Tank*assassin,Tank*ltank)
{
	vector<APoint> vec_weight;
	vector<APoint> vec_bullet;
	AStarWeightAdjust(vec_weight,ltank);      //������·��Ȩֵ�ı�
	if (ltank->getBullet().getIsFly())
	{
		AStarWithBulletWeightAdjust(vec_bullet,ltank);  //�ӵ�·����Ȩֵ�ı�
	}

	APoint vfirst(assassin->MapCoordinate()); //���
	APoint vfinal(ltank->MapCoordinate());    //�յ�
	list<APoint> openlist;
	list<APoint> closelist;
	openlist.push_back(vfirst);        //���뵽openlist����ʾ�Ѿ�����
	while (!openlist.empty())
	{
		APoint vetex = openlist.front();
		openlist.pop_front();         // ��openlist��ɾ��
		if (vetex==vfinal)            //�ҵ�Ŀ�꣬ȷ����һ������
		{
			Direction first=FRONT;
			while (!(vetex==vfirst))
			{
				switch (vetex.getToSourceDirection())   //����
				{
				case FRONT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()-1));
					first = RETREAT;
					break;
				case RETREAT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()+1));
					first = FRONT;
					break;
				case LEFT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()-1,vetex.getY()));
					first = RIGHT;
					break;
				case RIGHT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()+1,vetex.getY()));
					first = LEFT;
					break;
				default:
					break;
				}
			}
			assassin->setDirection(first);
			return;
		}
		else                         //�����ĸ�����ڵ�
		{
			closelist.push_back(vetex);  //���뵽closelist�У���ʾ�Ѿ�����

			APoint up(vetex.getX(),vetex.getY()-1);    //up ����
			if (find(openlist.begin(),openlist.end(),up)==openlist.end()&&find(closelist.begin(),closelist.end(),up)==closelist.end()&&MapValue(up.getX(),up.getY())!=CANTMOVE&&MapValue(up.getX(),up.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(up.getX(),up.getY());
				if (find(vec_weight.begin(),vec_weight.end(),up)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),up)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				int cost =toCost +Heuristic(up,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				up.setToHereCost(toCost);
				up.setCost(cost);
				up.setDirection(RETREAT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(up);        //���뵽openlist
			}

			APoint down(vetex.getX(),vetex.getY()+1);    //down ����
			if (find(openlist.begin(),openlist.end(),down)==openlist.end()&&find(closelist.begin(),closelist.end(),down)==closelist.end()&&MapValue(down.getX(),down.getY())!=CANTMOVE&&MapValue(down.getX(),down.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(down.getX(),down.getY());
				if (find(vec_weight.begin(),vec_weight.end(),down)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),down)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				int cost = toCost+Heuristic(down,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				down.setToHereCost(toCost);
				down.setCost(cost);
				down.setDirection(FRONT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(down);        //���뵽openlist
			}

			APoint left(vetex.getX()-1,vetex.getY());    //left ����
			if (find(openlist.begin(),openlist.end(),left)==openlist.end()&&find(closelist.begin(),closelist.end(),left)==closelist.end()&&MapValue(left.getX(),left.getY())!=CANTMOVE&&MapValue(left.getX(),left.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(left.getX(),left.getY());
				if (find(vec_weight.begin(),vec_weight.end(),left)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),left)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				int cost = toCost+Heuristic(left,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				left.setToHereCost(toCost);
				left.setCost(cost);
				left.setDirection(RIGHT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(left);        //���뵽openlist
			}

			APoint right(vetex.getX()+1,vetex.getY());    //right ����
			if (find(openlist.begin(),openlist.end(),right)==openlist.end()&&find(closelist.begin(),closelist.end(),right)==closelist.end()&&MapValue(right.getX(),right.getY())!=CANTMOVE&&MapValue(right.getX(),right.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(right.getX(),right.getY());
				if (find(vec_weight.begin(),vec_weight.end(),right)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				if (find(vec_bullet.begin(),vec_bullet.end(),right)!=vec_bullet.end())  //·�����ڵ�·����+BULLETDANGEROUS
				{
					toCost += BULLETDANGEROUS;
				}
				int cost = toCost+Heuristic(right,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				right.setToHereCost(toCost);
				right.setCost(cost);
				right.setDirection(LEFT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(right);        //���뵽openlist
			}
			openlist.sort(compareAP);
		}
	}
}
void NAStar(Tank*assassin,Tank*ltank)
{
	vector<APoint> vec_weight;
	AStarWeightAdjust(vec_weight,ltank);      //������·��Ȩֵ�ı�

	APoint vfirst(assassin->MapCoordinate()); //���
	APoint vfinal(ltank->MapCoordinate());    //�յ�
	list<APoint> openlist;
	list<APoint> closelist;
	openlist.push_back(vfirst);        //���뵽openlist����ʾ�Ѿ�����
	while (!openlist.empty())
	{
		APoint vetex = openlist.front();
		openlist.pop_front();         // ��openlist��ɾ��
		if (vetex==vfinal)            //�ҵ�Ŀ�꣬ȷ����һ������
		{
			Direction first=FRONT;
			while (!(vetex==vfirst))
			{
				switch (vetex.getToSourceDirection())   //����
				{
				case FRONT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()-1));
					first = RETREAT;
					break;
				case RETREAT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()+1));
					first = FRONT;
					break;
				case LEFT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()-1,vetex.getY()));
					first = RIGHT;
					break;
				case RIGHT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()+1,vetex.getY()));
					first = LEFT;
					break;
				default:
					break;
				}
			}
			assassin->setDirection(first);
			return;
		}
		else                         //�����ĸ�����ڵ�
		{
			closelist.push_back(vetex);  //���뵽closelist�У���ʾ�Ѿ�����

			APoint up(vetex.getX(),vetex.getY()-1);    //up ����
			if (find(openlist.begin(),openlist.end(),up)==openlist.end()&&find(closelist.begin(),closelist.end(),up)==closelist.end()&&MapValue(up.getX(),up.getY())!=CANTMOVE&&MapValue(up.getX(),up.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(up.getX(),up.getY());
				if (find(vec_weight.begin(),vec_weight.end(),up)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
				int cost =toCost +Heuristic(up,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				up.setToHereCost(toCost);
				up.setCost(cost);
				up.setDirection(RETREAT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(up);        //���뵽openlist
			}

			APoint down(vetex.getX(),vetex.getY()+1);    //down ����
			if (find(openlist.begin(),openlist.end(),down)==openlist.end()&&find(closelist.begin(),closelist.end(),down)==closelist.end()&&MapValue(down.getX(),down.getY())!=CANTMOVE&&MapValue(down.getX(),down.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(down.getX(),down.getY());
				if (find(vec_weight.begin(),vec_weight.end(),down)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
		
				int cost = toCost+Heuristic(down,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				down.setToHereCost(toCost);
				down.setCost(cost);
				down.setDirection(FRONT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(down);        //���뵽openlist
			}

			APoint left(vetex.getX()-1,vetex.getY());    //left ����
			if (find(openlist.begin(),openlist.end(),left)==openlist.end()&&find(closelist.begin(),closelist.end(),left)==closelist.end()&&MapValue(left.getX(),left.getY())!=CANTMOVE&&MapValue(left.getX(),left.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(left.getX(),left.getY());
				if (find(vec_weight.begin(),vec_weight.end(),left)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
	
				int cost = toCost+Heuristic(left,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				left.setToHereCost(toCost);
				left.setCost(cost);
				left.setDirection(RIGHT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(left);        //���뵽openlist
			}

			APoint right(vetex.getX()+1,vetex.getY());    //right ����
			if (find(openlist.begin(),openlist.end(),right)==openlist.end()&&find(closelist.begin(),closelist.end(),right)==closelist.end()&&MapValue(right.getX(),right.getY())!=CANTMOVE&&MapValue(right.getX(),right.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(right.getX(),right.getY());
				if (find(vec_weight.begin(),vec_weight.end(),right)!=vec_weight.end())  //·����̹���ڿ�ǰ+DANGEROUS
				{
					toCost += DANGEROUS;
				}
		
				int cost = toCost+Heuristic(right,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				right.setToHereCost(toCost);
				right.setCost(cost);
				right.setDirection(LEFT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(right);        //���뵽openlist
			}
			openlist.sort(compareAP);
		}
	}
}
void AStar(Tank*enemy,Tank*ltank)
{
	APoint vfirst(enemy->MapCoordinate());
	APoint vfinal(ltank->MapCoordinate());
	list<APoint> openlist;
	list<APoint> closelist;
	openlist.push_back(vfirst);        //���뵽openlist����ʾ�Ѿ�����
	while (!openlist.empty())
	{
		APoint vetex = openlist.front();
		openlist.pop_front();         // ��openlist��ɾ��
		if (vetex==vfinal)            //�ҵ�Ŀ�꣬ȷ����һ������
		{
			Direction first=FRONT;
			while (!(vetex==vfirst))
			{
				switch (vetex.getToSourceDirection())   //��ԭ
				{
				case FRONT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()-1));
					first = RETREAT;
					break;
				case RETREAT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX(),vetex.getY()+1));
					first = FRONT;
					break;
				case LEFT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()-1,vetex.getY()));
					first = RIGHT;
					break;
				case RIGHT:
					vetex = *find(closelist.begin(),closelist.end(),APoint(vetex.getX()+1,vetex.getY()));
					first = LEFT;
					break;
				default:
					break;
				}
			}
			enemy->setDirection(first);
			return;
		}
		else                         //�����ĸ�����ڵ�
		{
			closelist.push_back(vetex);  //���뵽closelist�У���ʾ�Ѿ�����

			APoint up(vetex.getX(),vetex.getY()-1);    //up ����
			if (find(openlist.begin(),openlist.end(),up)==openlist.end()&&find(closelist.begin(),closelist.end(),up)==closelist.end()&&MapValue(up.getX(),up.getY())!=CANTMOVE&&MapValue(up.getX(),up.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(up.getX(),up.getY());
				int cost =toCost +Heuristic(up,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				up.setToHereCost(toCost);
				up.setCost(cost);
				up.setDirection(RETREAT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(up);        //���뵽openlist
			}

			APoint down(vetex.getX(),vetex.getY()+1);    //down ����
			if (find(openlist.begin(),openlist.end(),down)==openlist.end()&&find(closelist.begin(),closelist.end(),down)==closelist.end()&&MapValue(down.getX(),down.getY())!=CANTMOVE&&MapValue(down.getX(),down.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(down.getX(),down.getY());
				int cost = toCost+Heuristic(down,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				down.setToHereCost(toCost);
				down.setCost(cost);
				down.setDirection(FRONT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(down);        //���뵽openlist
			}

			APoint left(vetex.getX()-1,vetex.getY());    //left ����
			if (find(openlist.begin(),openlist.end(),left)==openlist.end()&&find(closelist.begin(),closelist.end(),left)==closelist.end()&&MapValue(left.getX(),left.getY())!=CANTMOVE&&MapValue(left.getX(),left.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(left.getX(),left.getY());
				int cost = toCost+Heuristic(left,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				left.setToHereCost(toCost);
				left.setCost(cost);
				left.setDirection(RIGHT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(left);        //���뵽openlist
			}

			APoint right(vetex.getX()+1,vetex.getY());    //right ����
			if (find(openlist.begin(),openlist.end(),right)==openlist.end()&&find(closelist.begin(),closelist.end(),right)==closelist.end()&&MapValue(right.getX(),right.getY())!=CANTMOVE&&MapValue(right.getX(),right.getY())!=CANTACROSS)//�������б����ҿ���ͨ��
			{
				int toCost = vetex.getToHereCost()+MapValue(right.getX(),right.getY());
				int cost = toCost+Heuristic(right,vfinal);     //���� �ܼ�ֵ=��ǰһ�ڵ��ֵ+�˽ڵ��ֵ+����ֵ
				right.setToHereCost(toCost);
				right.setCost(cost);
				right.setDirection(LEFT);      //���õ�ǰһ�ڵ�ķ���
				openlist.push_back(right);        //���뵽openlist
			}
			openlist.sort(compareAP);
		}
	}
}


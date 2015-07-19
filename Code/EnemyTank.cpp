#include "EnemyTank.h"
#include <winuser.h>
#include <typeinfo>
#include "Direction.h"
#include "MapElement.h"
#pragma comment(lib,"winmm.lib")
int EnemyTank::m_framNumber=0;
const int TurnDistance = 9;
const int GeneticFactor = 2;
EnemyTank::EnemyTank(int id,Direction dir,int hp,int loc_x,int loc_y,HINSTANCE hinst,int mvSpeed):Tank(id,dir,hp,loc_x,loc_y,mvSpeed),																								
	hinstance(hinst),BulletRest(10),RandomRest(10),Shoot(0)
{	
	setPMapElement(Grass::Instance());         //调用函数还要写返回值干嘛 汗！
}

void EnemyTank::setHBMP() //!!!!!!!!!!!!!!!!!!!
{
	if (m_fitness<50)
	{
		m_hBMP = g_hEnemys[0];
		StarLevel = 1;
	}
	else if (m_fitness<75)


	{
		m_hBMP = g_hEnemys[1];
		StarLevel = 2;
	}
	else
	{
		m_hBMP = g_hEnemys[2];
		StarLevel = 3;
	}
}

void EnemyTank::setLocationX(int x)
{
	if (x<25)
	{
		x=25;
	}
	else if (x>g_rect.right-25)
	{
		x=g_rect.right-25;
	}
	m_pLocation.setX(x);
	m_pDrawPoint.setX(x-25);
	m_pMapCoordinate.setX(x/64+1);
}

void EnemyTank::setLocationY(int y)
{
	if (y<23)
	{
		y=23;
	}
	else if (y>g_rect.bottom-28)
	{
		y=g_rect.bottom-28;
	}
	m_pLocation.setY(y);
	m_pDrawPoint.setY(y-38);
	m_pMapCoordinate.setY(y/64+1);
}

void EnemyTank::increaseFramNumber()
{
	m_framNumber=(m_framNumber+1)%4;
}

void EnemyTank::Fitness(int TID)
{
	newEnemy[TID]->setFitness(int(pow(newEnemy[TID]->getScenario(0),2)+pow(newEnemy[TID]->getScenario(2),2)+pow(newEnemy[TID]->getScenario(4),2)+pow(newEnemy[TID]->getScenario(8),2)+pow(newEnemy[TID]->getScenario(7),2)+pow(newEnemy[TID]->getScenario(9)-2,2)+16*newEnemy[TID]->getShoot()));
	newEnemy[TID]->setHBMP();
}

void EnemyTank::Random()
{
	if (RandomRest>10&&(abs(m_pLocation.getX()%64-31)<TurnDistance&&abs(m_pLocation.getY()%64-31)<TurnDistance)||WhichMapElement(*this))
	{
		RandomRest=0;
		/*srand((unsigned)time(nullptr));*/
		switch (rand()%4)
		{
		case 0:
			if (getDirection()!=FRONT)
			{
				setDirection(FRONT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case 1:
			if (getDirection()!=RETREAT)
			{
				setDirection(RETREAT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case 2:
			if (getDirection()!=LEFT)
			{
				setDirection(LEFT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case 3:
			if (getDirection()!=RIGHT)
			{
				setDirection(RIGHT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
		default:
			break;
		}
	}
	
	if(!WhichMapElement(*this) )            //获取所朝向地形,四周是否有障碍物
	{
		getPMapElement()->Move(this);        //进行相应移动
	}
	if (rand()%30==1)
	{
		BulletShoot();
	}
}
void EnemyTank::Smart(int sIndex)
{
	if (!getBullet().getIsFly())
	{
		Attack(sIndex);
	}
	else
	{
		AStarHandle();
	}
}
void EnemyTank::AStarHandle()
{
	if (m_iAstarInterval>10&&!WhichMapElement(*this)&&(abs(m_pLocation.getX()%64-31)<TurnDistance&&abs(m_pLocation.getY()%64-32)<TurnDistance))
	{
		m_iAstarInterval=0;
		switch (Scenario[8])
		{
		case RANDOM:
			Random();
			return;
		case ASTAR:
			AStar(this,pLTank);
			break;
		case N_ASTAR:
			NAStar(this,pLTank);
			break;
		case NB_ASTAR:
			NBAStar(this,pLTank);
			break;
		case SNB_ASTAR:
			SNBAStar(this,pLTank);
			break;
		default:
			break;
		}
		switch (getDirection())
		{
		case FRONT:
			if (getDirection()!=FRONT)
			{
				setDirection(FRONT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case RETREAT:
			if (getDirection()!=RETREAT)
			{
				setDirection(RETREAT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case LEFT:
			if (getDirection()!=LEFT)
			{
				setDirection(LEFT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
			break;
		case RIGHT:
			if (getDirection()!=RIGHT)
			{
				setDirection(RIGHT);
				intializeFrame();
			}
			else
			{
				increaseFramNumber();
			}
		default:
			break;
		}
	}
	else if (WhichMapElement(*this))       //遇到阻碍 随机行走
	{
		Random();
		return;
	}
	

	if(!WhichMapElement(*this) )            //获取所朝向地形,四周是否有障碍物
	{
		getPMapElement()->Move(this);        //进行相应移动
	}
}
//void EnemyTank::OtherHandle()
//{
//	switch (rand()%5)
//	{
//	case 3:case 4:
//		AStarHandle();
//	default:
//		Random();
//		break;
//	}
//}
void EnemyTank::Attack(int sIndex)
{
	if ((abs(m_pLocation.getX()%64-31)<TurnDistance&&abs(m_pLocation.getY()%64-32)<TurnDistance)||WhichMapElement(*this))
	{
	if (BulletRest>10&&!getBullet().getIsFly())
	{
		BulletRest=0;
		Direction dir = pLTank->getBullet().getDirection();
		Direction dirT = pLTank->getDirection();
		switch (sIndex)                                                       //选取不同情况下的进攻方向
		{
		case FORCAST_SHOT:                           //ok
			if (abs(MapCoordinate().getX()-pLTank->MapCoordinate().getX())==1)
			{
				if (MapCoordinate().getY()>pLTank->MapCoordinate().getY())
				{
					setDirection(FRONT);
				}
				else if (MapCoordinate().getY()<pLTank->MapCoordinate().getY())
				{
					setDirection(RETREAT);
				}
			}
			else if(abs(MapCoordinate().getY()-pLTank->MapCoordinate().getY())==1)
			{
				if (MapCoordinate().getX()>pLTank->MapCoordinate().getX())
				{
					setDirection(LEFT);
				}
				else if(MapCoordinate().getX()<pLTank->MapCoordinate().getX())
				{
					setDirection(RIGHT);
				}
			}
			break;
		case FORCAST_BULLET_AGAINST:
			if (dir==FRONT)
			{
				setDirection(RETREAT);
			}
			else if (dir==RETREAT)
			{
				setDirection(FRONT);
			}
			else if (dir==LEFT)
			{
				setDirection(RIGHT);
			}
			else if (dir==RIGHT)
			{
				setDirection(LEFT);
			}
			break;
		case FACE_ATTACK:
		case BE_SNEAK_ATTACK:
			if (dirT==FRONT)
			{
				setDirection(RETREAT);
			}
			else if (dirT==RETREAT)
			{
				setDirection(FRONT);
			}
			else if (dirT==LEFT)
			{
				setDirection(RIGHT);
			}
			else if (dirT==RIGHT)
			{
				setDirection(LEFT);
			}
			break;
		case WANDER:
			if (MapCoordinate().getX()==pLTank->MapCoordinate().getX())
			{
				if (MapCoordinate().getY()<pLTank->MapCoordinate().getY())
				{
					setDirection(RETREAT);
				}
				else
				{
					setDirection(FRONT);
				}
			}
			else if (MapCoordinate().getY()==pLTank->MapCoordinate().getY())
			{
				if (MapCoordinate().getX()<pLTank->MapCoordinate().getX())
				{
					setDirection(RIGHT);
				}
				else
				{
					setDirection(LEFT);
				}
			}
		default:
			break;
		}

		PlaySound(MAKEINTRESOURCE(IDR_WAVE3),NULL,SND_ASYNC|SND_RESOURCE);
		m_pBullet.setIsFly(true);
		int x = Location().getX();
		int y = Location().getY();
		switch (getDirection())
		{
		case RETREAT:
			m_pBullet.setDirection(RETREAT);
			m_pBullet.setX(x);
			m_pBullet.setY(y);
			break;
		case LEFT:
			m_pBullet.setDirection(LEFT);
			m_pBullet.setX(x-20);
			m_pBullet.setY(y-7);
			break;
		case RIGHT:
			m_pBullet.setDirection(RIGHT);
			m_pBullet.setX(x+20);
			m_pBullet.setY(y-7);
			break;
		case FRONT:
			m_pBullet.setDirection(FRONT);
			m_pBullet.setX(x);
			m_pBullet.setY(y-15);
			break;
		default:
			break;
		}
	}
	}
	else
	{
		if(!WhichMapElement(*this) )            //获取所朝向地形,四周是否有障碍物
		{
			getPMapElement()->Move(this);        //进行相应移动
		}
	}
}
void EnemyTank::BulletShoot()
{
	/*TextOut(g_hmdc,200,220,TEXT("SHOOT!FUNCTION"),wcslen(TEXT("SHOOT!FUNCTION")));*/
	if (!m_pBullet.getIsFly()&&BulletRest>10)
	{
		BulletRest=0;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3),NULL,SND_ASYNC|SND_RESOURCE);
		/*TextOut(g_hmdc,200,260,TEXT("SHOOT!FUNCTIONSOUND"),wcslen(TEXT("SHOOT!FUNCTIONSOUND")));*/
		m_pBullet.setIsFly(true);
		int x = Location().getX();
		int y = Location().getY();
		switch (getDirection())
		{
		case RETREAT:
			m_pBullet.setDirection(RETREAT);
			m_pBullet.setX(x);
			m_pBullet.setY(y);
			break;
		case LEFT:
			m_pBullet.setDirection(LEFT);
			m_pBullet.setX(x-20);
			m_pBullet.setY(y-7);
			break;
		case RIGHT:
			m_pBullet.setDirection(RIGHT);
			m_pBullet.setX(x+20);
			m_pBullet.setY(y-7);
			break;
		case FRONT:
			m_pBullet.setDirection(FRONT);
			m_pBullet.setX(x);
			m_pBullet.setY(y-15);
			break;
		default:
			break;
		}
	}
}
void EnemyTank::Update()
{
	m_iAstarInterval++;
	BulletRest++;
	RandomRest++;
	if (typeid(*getPMapElement())==typeid(Ice)&&!WhichMapElement(*this))//在冰面上&&没有障碍做特殊处理，不受控制
	{	
		if (Scenario[InWhichScenario()]==ATTACK)
		{
			BulletShoot();
		}
		getPMapElement()->Move(this);
		
	}   
	else  //如果遇到障碍物则可以掉头
	{

		int scenarioIndex = InWhichScenario();//其它地形，没障碍物
		switch (Scenario[scenarioIndex])
		{
		case RANDOM:
			Random();
			break;
		case ASTAR:
			AStarHandle();
			break;
		case ATTACK:
			Attack(scenarioIndex);   //炮弹发射处理
			break;
		case SMART:
			Smart(scenarioIndex);
			break;
		default:
			AStarHandle();
			break;
		}
	}


}

void EnemyTank::Paint(HDC &hmdc)
{
	if (getHP()>0)
	{
		if (isInvincible==true)
		{
			SelectObject(g_hbufdc,g_hInitialize[(63-(InvincibleTime--))/3]);
			if (InvincibleTime==0)
			{
				isInvincible=false;
			}
			TransparentBlt(hmdc,DrawPoint().getX()-13,DrawPoint().getY()+13,75,50,g_hbufdc,0,0,75,50,RGB(255,255,255));
		}
		else
		{
			Update();     //坦克处理移动动作，并检测炮弹发出情况

			SelectObject(g_hbufdc,m_hBMP);  //绘制坦克
			if (StarLevel==1)
			{

				TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,75,g_hbufdc,getFramNumber()*50,getDirection()*75,50,75,RGB(255,255,255));
			}
			else if(StarLevel==2)
			{
				TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,75,g_hbufdc,getFramNumber()*50,getDirection()*75,50,75,RGB(252,0,252));
			}
			else
			{
				TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,75,g_hbufdc,getFramNumber()*50,getDirection()*75,50,75,RGB(255,0,255));
			}


			if (m_pBullet.getIsFly())    //检测炮弹状态并采取相应绘制
			{
				m_pBullet.Paint(hmdc);
			}
		}
		
	}
	else
	{
		switch (m_iBomb)
		{
		case 1:
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1),hinstance,SND_RESOURCE|SND_ASYNC);
			SelectObject(g_hbufdc,g_hBomb[0]);
			break;
		case 2:case 3:case 4:case 5:
			SelectObject(g_hbufdc,g_hBomb[0]);
			break;
		case 6:case 7:case 8:case 9:
			SelectObject(g_hbufdc,g_hBomb[1]);
			break;
		case 10:case 11:case 12:case 13:
			SelectObject(g_hbufdc,g_hBomb[2]);
			break;
		case 14:case 15:case 16:case 17:
			SelectObject(g_hbufdc,g_hBomb[3]);
			break;
		case 18:case 19:case 20:case 21:
			SelectObject(g_hbufdc,g_hBomb[4]);
			break;
		case 22:case 23:case 24:case 25:
			SelectObject(g_hbufdc,g_hBomb[5]);
			break;
		case 26:case 27:case 28:case 29:
			SelectObject(g_hbufdc,g_hBomb[6]);
			break;
		case 30:case 31:case 32:case 33:
			SelectObject(g_hbufdc,g_hBomb[7]);
			break;
		case 34:
			setIsAlive(false);
			m_iBomb=1;
			return;
		default:
			break;
		}

		TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),64,64,g_hbufdc,0,0,64,64,RGB(0,0,0));
		m_iBomb++;
		if (m_pBullet.getIsFly())    //检测炮弹状态并采取相应绘制
		{
			m_pBullet.Paint(hmdc);
		}
	}

}

//GA
void EnemyTank::CrossOver(int newT,int oldT1,int oldT2)
{

	
	for (int i=0;i<10;i++)
	{
		switch(rand()%2)
		{
		case 0:
			newEnemy[newT]->setScenario(i,oldEnemy[oldT1]->getScenario(i));
			break;
		case 1:
			newEnemy[newT]->setScenario(i,oldEnemy[oldT2]->getScenario(i));
			break;
		default:
			break;
		}

	}
}    //杂交
void EnemyTank::Mutation(int TID)
{
	for (int i=0;i<8;i++)
	{
		if (rand()%20==1)
		{
			if (i==0)
			{
				switch (rand()%(3+GeneticFactor))                   //预测射击
				{
				case 0:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				case 1:
					newEnemy[TID]->setScenario(i,ASTAR);
					break;
				case 2:
					newEnemy[TID]->setScenario(i,ATTACK);
					break;
				default:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				}
			}
			else if(i==1||i==2)                    //预测被射击或，炮弹临近
			{
				switch (rand()%(2+GeneticFactor))
				{
				case 0:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				case 1:
					newEnemy[TID]->setScenario(i,ASTAR);
					break;
				default:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				}
			}
			else                                   //其它情况
			{
				switch (rand()%(4+GeneticFactor))
				{
				case 0:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				case 1:
					newEnemy[TID]->setScenario(i,ASTAR);
					break;
				case 2:
					newEnemy[TID]->setScenario(i,ATTACK);
					break;
				case 3:
					newEnemy[TID]->setScenario(i,SMART);
					break;
				default:
					newEnemy[TID]->setScenario(i,RANDOM);
					break;
				}
			}
		}
	}
	if (rand()%20==1)
		switch (rand()%(5+GeneticFactor))
	{
		case 0:
			newEnemy[TID]->setScenario(8,RANDOM);
			break;
		case 1:
			newEnemy[TID]->setScenario(8,ASTAR);
			break;
		case 2:
			newEnemy[TID]->setScenario(8,N_ASTAR);
			break;
		case 3:
			newEnemy[TID]->setScenario(8,NB_ASTAR);
			break;
		case 4:
			newEnemy[TID]->setScenario(8,SNB_ASTAR);
			break;
		default:
			newEnemy[TID]->setScenario(8,RANDOM);
			break;
	}
	if (rand()%20==1)
		switch (rand()%(4+GeneticFactor))
	{
		case 0:
			newEnemy[TID]->setScenario(9,SPEED3);
			break;
		case 1:
			newEnemy[TID]->setScenario(9,SPEED4);
			break;
		case 2:
			newEnemy[TID]->setScenario(9,SPEED5);
			break;
		case 3:
			newEnemy[TID]->setScenario(9,SPEED6);
			break;
		default:
			newEnemy[TID]->setScenario(9,SPEED3);
			break;
	}
}     //基因突变
void EnemyTank::Creature(int TID)
{
	for (int i=0;i<8;i++)
	{
		if (i==0)
		{
			switch (rand()%(6+GeneticFactor))                   //预测射击
			{
			case 0:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			case 1:
				newEnemy[TID]->setScenario(i,ASTAR);
				break;
			case 2:
				newEnemy[TID]->setScenario(i,ATTACK);
				break;
			default:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			}
		}
		else if(i==1||i==2)                    //预测被射击或，炮弹临近
		{
			switch (rand()%(4+GeneticFactor))
			{
			case 0:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			case 1:
				newEnemy[TID]->setScenario(i,ASTAR);
				break;
			default:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			}
		}
		else                                   //其它情况
		{
			switch (rand()%(8+GeneticFactor))
			{
			case 0:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			case 1:
				newEnemy[TID]->setScenario(i,ASTAR);
				break;
			case 2:
				newEnemy[TID]->setScenario(i,ATTACK);
				break;
			case 3:
				newEnemy[TID]->setScenario(i,SMART);
				break;
			default:
				newEnemy[TID]->setScenario(i,RANDOM);
				break;
			}
		}
	}

	switch (rand()%(10+GeneticFactor))
	{
	case 0:
		newEnemy[TID]->setScenario(8,RANDOM);
		break;
	case 1:
		newEnemy[TID]->setScenario(8,ASTAR);
		break;
	case 2:
		newEnemy[TID]->setScenario(8,N_ASTAR);
		break;
	case 3:
		newEnemy[TID]->setScenario(8,NB_ASTAR);
		break;
	case 4:
		newEnemy[TID]->setScenario(8,SNB_ASTAR);
		break;
	default:
		newEnemy[TID]->setScenario(8,RANDOM);
		break;
	}

	switch (rand()%(6+GeneticFactor))
	{
	case 0:
		newEnemy[TID]->setScenario(9,SPEED3);
		break;
	case 1:
		newEnemy[TID]->setScenario(9,SPEED4);
		break;
	case 2:
		newEnemy[TID]->setScenario(9,SPEED5);
		break;
	case 3:
		newEnemy[TID]->setScenario(9,SPEED6);
		break;
	default:
		newEnemy[TID]->setScenario(9,SPEED3);
		break;
	}

}
void EnemyTank::SortEnemy()
{
	sort(newEnemy,newEnemy+10,compareEnemy);
}

bool EnemyTank::IsFaceBullet()
{
	if (pLTank->getBullet().getIsFly())
	{
		int bulletX = pLTank->getBullet().Location().getX()/64+1;
		int bulletY = pLTank->getBullet().Location().getY()/64+1;
		if (bulletX==MapCoordinate().getX())
		{
			if (MapCoordinate().getY()>bulletY&&pLTank->getBullet().getDirection()==RETREAT)
			{
				return true;
			}
			else if (MapCoordinate().getY()<bulletY&&pLTank->getBullet().getDirection()==FRONT)
			{
				return true;
			}
		}
		else if (bulletY==MapCoordinate().getY())
		{
			if (MapCoordinate().getX()>bulletX&&pLTank->getBullet().getDirection()==RIGHT)
			{
				return true;
			}
			else if (MapCoordinate().getX()<bulletX&&pLTank->getBullet().getDirection()==LEFT)
			{
				return true;
			}
		}
	}
	return false;
}                    //判断处于哪个状态

bool EnemyTank::IsNearBullet()
{
	if (pLTank->getBullet().getIsFly())
	{
		int bulletX = pLTank->getBullet().Location().getX()/64+1;
		int bulletY = pLTank->getBullet().Location().getY()/64+1;
		if (abs(bulletX-MapCoordinate().getX())==1)
		{
			if (MapCoordinate().getY()>bulletY&&pLTank->getBullet().getDirection()==RETREAT)
			{
				return true;
			}
			else if (MapCoordinate().getY()<bulletY&&pLTank->getBullet().getDirection()==FRONT)
			{
				return true;
			}
		}
		else if (abs(bulletY==MapCoordinate().getY())==1)
		{
			if (MapCoordinate().getX()>bulletX&&pLTank->getBullet().getDirection()==RIGHT)
			{
				return true;
			}
			else if (MapCoordinate().getX()<bulletX&&pLTank->getBullet().getDirection()==LEFT)
			{
				return true;
			}
		}
	}
	return false;
}

bool EnemyTank::IsWillShot()
{
	if (MapCoordinate().getX()-pLTank->MapCoordinate().getX()==-1&&pLTank->getDirection()==LEFT)
	{
			return true;
	}
	else if (MapCoordinate().getX()-pLTank->MapCoordinate().getX()==1&&pLTank->getDirection()==RIGHT)
	{
		return true;
	}
	else if (MapCoordinate().getY()-pLTank->MapCoordinate().getY()==-1&&pLTank->getDirection()==FRONT)
	{
			return true;
	}
	else if (MapCoordinate().getY()-pLTank->MapCoordinate().getY()==1&&pLTank->getDirection()==RETREAT)
	{
		return true;
	}
	return false;
}

bool EnemyTank::IsWillBeShot()
{
	if (abs(MapCoordinate().getX()-pLTank->MapCoordinate().getX())==1)
	{
		if (MapCoordinate().getX()>pLTank->MapCoordinate().getX()&&getDirection()==LEFT)
		{
			return true;
		}
		else if(MapCoordinate().getX()<pLTank->MapCoordinate().getX()&&getDirection()==RIGHT)
		{
			return true;
		}
	}
	else if (abs(MapCoordinate().getY()-pLTank->MapCoordinate().getY())==1)
	{
		if (MapCoordinate().getY()>pLTank->MapCoordinate().getY()&&getDirection()==FRONT)
		{
			return true;
		}
		else if (MapCoordinate().getY()<pLTank->MapCoordinate().getY()&&getDirection()==RETREAT)
		{
			return true;
		}
	}
	return false;
}

bool EnemyTank::IsSneakAttack()
{
	if (MapCoordinate().getX()==pLTank->MapCoordinate().getX())
	{
		if (MapCoordinate().getY()>pLTank->MapCoordinate().getY()&&getDirection()==FRONT&&pLTank->getDirection()!=RETREAT)
		{
			return true;
		}
		else if(MapCoordinate().getY()<pLTank->MapCoordinate().getY()&&getDirection()==RETREAT&&pLTank->getDirection()!=FRONT)
		{
			return true;
		}
	}
	else if (MapCoordinate().getY()==pLTank->MapCoordinate().getY())
	{
		if (MapCoordinate().getX()>pLTank->MapCoordinate().getX()&&getDirection()==LEFT&&pLTank->getDirection()!=RIGHT)
		{
			return true;
		}
		else if (MapCoordinate().getX()<pLTank->MapCoordinate().getX()&&getDirection()==RIGHT&&pLTank->getDirection()!=LEFT)
		{
			return true;
		}
	}
	return false;
}

bool EnemyTank::IsFaceAttack()
{
	if (MapCoordinate().getX()==pLTank->MapCoordinate().getX())
	{
		if (MapCoordinate().getY()>pLTank->MapCoordinate().getY()&&getDirection()==FRONT&&pLTank->getDirection()==RETREAT)
		{
			return true;
		}
		else if(MapCoordinate().getY()<pLTank->MapCoordinate().getY()&&getDirection()==RETREAT&&pLTank->getDirection()==FRONT)
		{
			return true;
		}
	}
	else if (MapCoordinate().getY()==pLTank->MapCoordinate().getY())
	{
		if (MapCoordinate().getX()>pLTank->MapCoordinate().getX()&&getDirection()==LEFT&&pLTank->getDirection()==RIGHT)
		{
			return true;
		}
		else if (MapCoordinate().getX()<pLTank->MapCoordinate().getX()&&getDirection()==RIGHT&&pLTank->getDirection()==LEFT)
		{
			return true;
		}
	}
	return false;
}

bool EnemyTank::IsBeSneakAttack()
{
	if (MapCoordinate().getX()==pLTank->MapCoordinate().getX())
	{
		if (MapCoordinate().getY()>pLTank->MapCoordinate().getY()&&getDirection()!=FRONT&&pLTank->getDirection()==RETREAT)
		{
			return true;
		}
		else if(MapCoordinate().getY()<pLTank->MapCoordinate().getY()&&getDirection()!=RETREAT&&pLTank->getDirection()==FRONT)
		{
			return true;
		}
	}
	else if (MapCoordinate().getY()==pLTank->MapCoordinate().getY())
	{
		if (MapCoordinate().getX()>pLTank->MapCoordinate().getX()&&getDirection()!=LEFT&&pLTank->getDirection()==RIGHT)
		{
			return true;
		}
		else if (MapCoordinate().getX()<pLTank->MapCoordinate().getX()&&getDirection()!=RIGHT&&pLTank->getDirection()==LEFT)
		{
			return true;
		}
	}
	return false;
}

bool EnemyTank::IsWander()
{
	if (MapCoordinate().getX()==pLTank->MapCoordinate().getX())
	{
		if (MapCoordinate().getY()>pLTank->MapCoordinate().getY()&&getDirection()!=FRONT&&pLTank->getDirection()!=RETREAT)
		{
			return true;
		}
		else if(MapCoordinate().getY()<pLTank->MapCoordinate().getY()&&getDirection()!=RETREAT&&pLTank->getDirection()!=FRONT)
		{
			return true;
		}
	}
	else if (MapCoordinate().getY()==pLTank->MapCoordinate().getY())
	{
		if (MapCoordinate().getX()>pLTank->MapCoordinate().getX()&&getDirection()!=LEFT&&pLTank->getDirection()!=RIGHT)
		{
			return true;
		}
		else if (MapCoordinate().getX()<pLTank->MapCoordinate().getX()&&getDirection()!=RIGHT&&pLTank->getDirection()!=LEFT)
		{
			return true;
		}
	}
	return false;
}


int EnemyTank::InWhichScenario()                                             //获取Scenario
{

	if (IsFaceBullet())
	{
		return FORCAST_BULLET_AGAINST;
	}
	else if (IsNearBullet())
	{
		return FORCAST_BULLET_NEAR;
	}
	else if (IsWillShot())
	{
		return FORCAST_SHOT;
	}
	else if (IsWillBeShot())
	{
		return FORCAST_BESHOT;
	}
	else if (IsSneakAttack())
	{
		return SNEAK_ATTACK;
	}
	else if (IsFaceAttack())
	{
		return FACE_ATTACK;
	}
	else if (IsBeSneakAttack())
	{
		return BE_SNEAK_ATTACK;
	}
	else if (IsWander())
	{
		return WANDER;
	}
	return OTHER;
}
#include "Bullet.h"
#include "Tank.h"
#include <typeinfo>
#include "LightTank.h"
#include "Assassin.h"

const int  excursion = 12;
int MapValue(int x,int y);
class Assassin;
Bullet::Bullet(int x,int y,Tank *owner,Direction dir,int speed,bool flag):m_pLocation(x,y),																					  
																						  m_tOwner(owner),
																						  m_dDirection(dir),
																						  m_iMoveSpeed(speed),
																						  m_bIsFly(flag)
{
}
void Bullet::Move()
{
	int bulletX = m_pLocation.getX();
	int bulletY = m_pLocation.getY();
	int collideX;
	int collideY;
	int bulletFlySpeed = m_iMoveSpeed+m_tOwner->MoveSpeed();
	switch (m_dDirection)                     //炮弹的飞行
	{
	case RETREAT:
		m_pLocation.setY(bulletY+bulletFlySpeed);
		collideX = m_pLocation.getX();
		collideY = m_pLocation.getY()+7;
		break;
	case LEFT:
		m_pLocation.setX(bulletX-bulletFlySpeed);
		collideX = m_pLocation.getX()-7;
		collideY = m_pLocation.getY();
		break;
	case RIGHT:
		m_pLocation.setX(bulletX+bulletFlySpeed);
		collideX = m_pLocation.getX()+7;
		collideY = m_pLocation.getY();
		break;
	case FRONT:
		m_pLocation.setY(bulletY-bulletFlySpeed);
		collideX = m_pLocation.getX();
		collideY = m_pLocation.getY()-7;
		break;
	default:
		break;
	}
	int map_value = map[(g_roundNumber-1)%5][collideY/64][collideX/64];// 获取地形
	if (map_value==101||map_value==102||(map_value>=109&&map_value!=111&&map_value!=112&&map_value<=116))     //遇到墙壁
	{
		m_bIsFly = false;
	}
	else if (bulletX<0||bulletX>g_rect.right||bulletY<0||bulletY>g_rect.bottom)  //出界
	{
		m_bIsFly = false;
	}
	else if (m_tOwner->getID()==0)       //操控坦克   炮弹处理
	{
		if (isTouched(this,enemys[0])&&enemys[0]->getIsAlive()&&enemys[0]->getHP()>0&&!enemys[0]->getIsInvincible())   //击中Assassin
		{
			ShootedPlace.push_back(APoint(this->Location().getX()/64+1,this->Location().getY()/64+1));
			enemys[0]->decreaseHP();
			m_bIsFly = false;
			g_score+=30;
		}
		else
		{
			for (int i=0;i<g_enemyNumber;i++)
			{
				if (isTouched(this,newEnemy[i])&&newEnemy[i]->getIsAlive()&&newEnemy[i]->getHP()>0&&!newEnemy[i]->getIsInvincible())            //操控炮弹击中敌方坦克
				{
					ShootedPlace.push_back(APoint(this->Location().getX()/64+1,this->Location().getY()/64+1));
					newEnemy[i]->decreaseHP();
					m_bIsFly = false;
			        if (newEnemy[i]->getFitness()<30)
			        {
						g_score+=20;   ////////////////////////////////////////////////////////////////////////////////////////////////////
			        }
					else if (newEnemy[i]->getFitness()<50)
					{
						g_score+=35;
					}
					else
					{
						g_score+=50;
					}
					break;
				}
				else if (newEnemy[i]->getBullet().getIsFly()&&isTouched(&(newEnemy[i]->getBullet()),this))//操控炮弹与敌方炮弹相碰
				{
					newEnemy[i]->m_pBullet.setIsFly(false);
					m_bIsFly = false;
					break;
				}
			}
		}
		
	}
	else if (m_tOwner->getID()==2)   //以后修改！！！！！！！！！！
	{
		if (pLTank->getIsAlive()&&pLTank->getHP()&&isTouched(this,pLTank))
		{
			pLTank->decreaseHP();
			this->m_tOwner->increaseShoot();
			this->m_tOwner->setFitness(this->m_tOwner->getFitness()+25);
			m_bIsFly = false;
		}
	}

}

void Bullet::Paint(HDC& hmdc)
{
	
	
	switch (getDirection())
	{
	case FRONT:
		SelectObject(g_hbufdc,g_hBullet[0]);
        TransparentBlt(hmdc,m_pLocation.getX()-40,m_pLocation.getY()-25,80,50,g_hbufdc,0,0,80,50,RGB(0,0,0));
		break;
	case RETREAT:
		SelectObject(g_hbufdc,g_hBullet[1]);
		TransparentBlt(hmdc,m_pLocation.getX()-40,m_pLocation.getY()-25,80,50,g_hbufdc,0,0,80,50,RGB(0,0,0));
		break;
	case LEFT:
		SelectObject(g_hbufdc,g_hBullet[2]);
		TransparentBlt(hmdc,m_pLocation.getX()-25,m_pLocation.getY()-40,50,80,g_hbufdc,0,0,50,80,RGB(0,0,0));
		break;
	case RIGHT:
		SelectObject(g_hbufdc,g_hBullet[3]);
		TransparentBlt(hmdc,m_pLocation.getX()-25,m_pLocation.getY()-40,50,80,g_hbufdc,0,0,50,80,RGB(0,0,0));
		break;
	default:
		break;
	}
	
	if (bulletExplode<8)
	{
			fireX=m_tOwner->Location().getX();
			fireY=m_tOwner->Location().getY();
			switch (getDirection())
			{
			case RETREAT:
				fireY+=excursion;
				break;
			case LEFT:
				fireX-=(20+excursion);
				fireY-=7;
				break;
			case RIGHT:
				fireX+=(20+excursion);
				fireY-=7;
				break;
			case FRONT:
				fireY-=(15+excursion);
				break;
			default:
				break;
			}
		SelectObject(g_hbufdc,g_hBulletEx[bulletExplode++]);
		TransparentBlt(hmdc,fireX-20,fireY-20,40,40,g_hbufdc,0,0,40,40,RGB(0,0,0));
	}
	Move();
	/*BitBlt(hmdc,m_pLocation.getX()-7,m_pLocation.getY()-7,15,15,g_hbufdc,0,0,SRCCOPY);*/
}

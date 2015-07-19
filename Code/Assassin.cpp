#include "Assassin.h"

#include "Function.h"
#include "MapElement.h"
#include <ctime>
#include <cmath>
#include <typeinfo>
extern Tank *pLTank;
int Assassin::m_framNumber=0;
const int circleDistance = 7;
Assassin::Assassin(int id,Direction dir,int hp,int loc_x,int loc_y,HINSTANCE hinst,int mvSpeed):Tank(id,dir,hp,loc_x,loc_y,mvSpeed),
                                                                                        	hinstance(hinst)
{
	setPMapElement(Grass::Instance());         //调用函数还要写返回值干嘛 汗！
}

void Assassin::setLocationX(int x)
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
	//switch (getDirection())
	//{
	//case LEFT:
	//	x+=25;
	//	break;
	//case RIGHT:
	//	x-=25;
	//	break;
	//default:
	//	break;
	//}
	m_pMapCoordinate.setX(x/64+1);
}

void Assassin::setLocationY(int y)
{
	if (y<25)
	{
		y=23;
	}
	else if (y>g_rect.bottom-25)
	{
		y=g_rect.bottom-25;
	}
	m_pLocation.setY(y);
	m_pDrawPoint.setY(y-25);
	//switch (getDirection())
	//{
	//case RETREAT:
	//	y-=25;
	//	break;
	//case FRONT:
	//	y+=25;
	//	break;
	//default:
	//	break;
	//}
	m_pMapCoordinate.setY(y/64+1);
}

void Assassin::increaseFramNumber()
{
	m_framNumber=(m_framNumber+1)%3;
}

void Assassin::Adjust(Direction dir)
{
	switch (dir)    
	{
	case FRONT:
		if (getDirection()!=FRONT)
		{
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
			intializeFrame();
		}
		else
		{
			increaseFramNumber();
		}
		break;
	default:
		break;
	}
}
void Assassin::Update()
{

	bool isLock = WhichMapElement(*this);
		if (!isLock&&abs(m_pLocation.getX()%64-31)<circleDistance&&abs(m_pLocation.getY()%64-32)<circleDistance)   //没有卡住且在中心  重新定向  间距通过速度而定
		{
			switch (g_roundNumber)
			{
			case 1:
				AStar(this,pLTank);
				break;
			case 2:
				NAStar(this,pLTank);
				break;
			case 3:
				NBAStar(this,pLTank);
				break;
			default:
				SNBAStar(this,pLTank);
				break;
			}
		  //A*得出走向 
			Adjust(getDirection());
		}
		else if (isLock&&abs(m_pLocation.getX()%64-31)<circleDistance&&abs(m_pLocation.getY()%64-32)<circleDistance)    //卡住
		{
			Direction dir = (Direction)unsigned(rand()%4);
			Adjust(dir);
		}
		    if(!WhichMapElement(*this) )            //获取所朝向地形,四周是否有障碍物
			{
				getPMapElement()->Move(this);        //进行相应移动
			}	
	
}

void Assassin::setHBMP()
{
	switch (g_roundNumber)
	{
	case 1:
		m_hAssassin = g_hAssassin[0];
		break;
	case 2:
		m_hAssassin = g_hAssassin[1];
		break;
	case 3:
		m_hAssassin = g_hAssassin[2];
		break;
	default:
		m_hAssassin = g_hAssassin[3];
		break;
	}
	setLocationX(m_pLocation.getX());
	setLocationY(m_pLocation.getY());
}
void Assassin::Paint(HDC &hmdc)
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
			Update();	
			int framNumber = getFramNumber();
			SelectObject(g_hbufdc,m_hAssassin);
			TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,50,g_hbufdc,framNumber*50,getDirection()*50,50,50,RGB(0,0,0));
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

	}

}
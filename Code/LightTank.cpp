#include "LightTank.h"
#include <winuser.h>
#include <typeinfo>
#include "MapElement.h"
#pragma comment(lib,"winmm.lib")
int LightTank::m_framNumber=0;
LightTank::LightTank(int id,Direction dir,int hp,int loc_x,int loc_y,HINSTANCE hinst,int mvSpeed):Tank(id,dir,hp,loc_x,loc_y,mvSpeed),	
																									BulletRest(10),
																									hinstance(hinst)
{
	m_hBMP = LoadBitmap(hinst,MAKEINTRESOURCE(IDB_BITMAP9));
	setPMapElement(Grass::Instance());         //调用函数还要写返回值干嘛 汗！
}

void LightTank::setLocationX(int x)
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

void LightTank::setLocationY(int y)
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

void LightTank::increaseFramNumber()
{
	m_framNumber=(m_framNumber+1)%4;
}

void LightTank::BulletShoot()
{
	if (GetAsyncKeyState('A')&0x8000&&!m_pBullet.getIsFly()&&BulletRest>=10)
	{
		BulletRest=0;
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
void LightTank::MoveBack()
{
	int stepLenght = 2;
	Direction dir = getDirection();
		switch (dir)
		{
		case RETREAT:
			do 
			{
				setLocationY(Location().getY()-stepLenght);
			}
			while (WhichMapElement(*this));
			setLocationY(Location().getY()+stepLenght);
			break;
		case LEFT:
			do 
			{
				setLocationX(Location().getX()+stepLenght);
			}
			while (WhichMapElement(*this));
			setLocationX(Location().getX()-stepLenght);
			break;
		case RIGHT:
			do 
			{
				setLocationX(Location().getX()-stepLenght);
			}
			while (WhichMapElement(*this));
			setLocationX(Location().getX()+stepLenght);
			break;
		case FRONT:
			do 
			{
				setLocationY(Location().getY()+stepLenght);
			}
			while (WhichMapElement(*this));
			setLocationY(Location().getY()-stepLenght);	
			break;
		default:
			break;
		}
}
void LightTank::Update()
{
	bool press = false;
	BulletRest++;
	if (typeid(*getPMapElement())==typeid(Ice)&&!WhichMapElement(*this))//在冰面上&&没有障碍做特殊处理，不受控制
	{
		BulletShoot();
		getPMapElement()->Move(this);
		if (WhichMapElement(*this))
		{
			MoveBack();
		}
		 //如果遇到障碍物则可以掉头
	
	}
	else
	{
	 if (GetAsyncKeyState(VK_UP)&0x8000)     //确准坦克朝向并更新相应帧数
	{
		if (getDirection()!=FRONT)
		{
			setDirection(FRONT);
			intializeFrame();
		}
		else
		{
			increaseFramNumber();
		}
		press = true;
	}
	else if (GetAsyncKeyState(VK_DOWN)&0x8000)
	{
		if (getDirection()!=RETREAT)
		{
			setDirection(RETREAT);
			intializeFrame();
		}
		else
		{
			increaseFramNumber();
		}
		press = true;
	}
	else if (GetAsyncKeyState(VK_LEFT)&0x8000)
	{
		if (getDirection()!=LEFT)
		{
			setDirection(LEFT);
			intializeFrame();
		}
		else
		{
			increaseFramNumber();
		}
		press = true;
	}
	else if (GetAsyncKeyState(VK_RIGHT)&0x8000)
	{
		if (getDirection()!=RIGHT)
		{
			setDirection(RIGHT);
			intializeFrame();
		}
		else
		{
			increaseFramNumber();
		}
		press = true;
	}

	BulletShoot();   //炮弹发射处理

	if (press)  //是否进行按键操作
	{
			if(!WhichMapElement(*this) )            //获取所朝向地形,四周是否有障碍物
			{
		    getPMapElement()->Move(this);        //进行相应移动
			if (WhichMapElement(*this))
			{
				MoveBack();
			}
			}
	}
	else
	{
		WhichMapElement(*this);     //虽然不动但是得检测是否碰撞
	}
	}
}
void LightTank::LevelUP(HDC &hmdc)
{
	static int levelAnimationTime = 0;
	if ((g_score>=500&&g_score_pre<500)||(g_score>=1500&&g_score_pre<1500)||(g_score>=3000&&g_score_pre<3000)||(g_score>=5000&&g_score_pre<5000))
	{	
		increaseMaxHP();
		increaseHP();
		increaseSpeed();
		IsLevelUP = true;
	}
	if (IsLevelUP&&levelAnimationTime<78)
	{
		SelectObject(g_hbufdc,g_hLevelUp[levelAnimationTime/3]);
		TransparentBlt(hmdc,Location().getX()-110,Location().getY()-67,220,135,g_hbufdc,0,0,220,135,RGB(128,128,128));
		levelAnimationTime++;
		if (levelAnimationTime==78)
		{
			IsLevelUP = false;
			levelAnimationTime=0;
		}
	}
}
void LightTank::Paint(HDC &hmdc)
{
	/*******活着*****/
	if (getHP()>0)
	{
		/**************************坦克移动等操作******************************/
		Update();                      //坦克处理移动动作，并检测炮弹发出情况

		/**************************坦克绘制******************************/
		SelectObject(g_hbufdc,m_hBMP);  //绘制坦克
		TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,75,g_hbufdc,getFramNumber()*50,getDirection()*75,50,75,RGB(252,252,252));


		/**************************炮弹******************************/
		if (m_pBullet.getIsFly())      //检测炮弹状态并采取相应绘制
		{
			m_pBullet.Paint(hmdc);
		}


		 /**************************血条******************************/
		SelectObject(g_hbufdc,g_hBloodBar);
		int bloodX = DrawPoint().getX()-8;
		int bloodY = DrawPoint().getY()-15;
		TransparentBlt(hmdc,bloodX,bloodY,64,18,g_hbufdc,0,0,64,18,RGB(0,1,2));  //血条背景绘制
		HBRUSH hbrush;
		if (getHP()==2)
		{
			hbrush = CreateSolidBrush(RGB(255,102,0));  //橘黄
		}
		else if (getHP()==1)
		{
			hbrush = CreateSolidBrush(RGB(255,0,0));    //红色
		}
		else
		{
			hbrush = CreateSolidBrush(RGB(9,247,56));   //绿色
		}
		int bloodRectLUX = bloodX+3;
		int bloodRectLUY = bloodY+4;
		int bloodRectRDX = bloodX+60*getHP()/MaxHP;
		int bloodRectRDY = bloodY+14;
		SelectObject(hmdc,hbrush);
		Rectangle(hmdc,bloodRectLUX,bloodRectLUY,bloodRectRDX,bloodRectRDY);  //血条绘制
		DeleteObject(hbrush);

		/**************************升级******************************/
		LevelUP(hmdc);    //检查是否升级
		g_score_pre = g_score;
	}
	/******挂了******/
	else
	{
		/**************************如果分数比最高纪录高则写入纪录******************************/
		if (g_score>g_HistoryBestScore)//分数写入文件
		{
			g_HistoryBestScore = g_score;
			fstream wfile("HistoryBestScore.txt");
			wfile<<g_HistoryBestScore;
		}
		/**************************爆炸动画******************************/
		switch (m_iBomb)      //爆炸更新
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
		TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),64,64,g_hbufdc,0,0,64,64,RGB(0,0,0));  //坦克爆炸绘制
		m_iBomb++;
		if (m_pBullet.getIsFly())    //检测炮弹状态并采取相应绘制
		{
			m_pBullet.Paint(hmdc);
		}
	}

}
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
	setPMapElement(Grass::Instance());         //���ú�����Ҫд����ֵ���� ����
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
	if (typeid(*getPMapElement())==typeid(Ice)&&!WhichMapElement(*this))//�ڱ�����&&û���ϰ������⴦�����ܿ���
	{
		BulletShoot();
		getPMapElement()->Move(this);
		if (WhichMapElement(*this))
		{
			MoveBack();
		}
		 //��������ϰ�������Ե�ͷ
	
	}
	else
	{
	 if (GetAsyncKeyState(VK_UP)&0x8000)     //ȷ׼̹�˳��򲢸�����Ӧ֡��
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

	BulletShoot();   //�ڵ����䴦��

	if (press)  //�Ƿ���а�������
	{
			if(!WhichMapElement(*this) )            //��ȡ���������,�����Ƿ����ϰ���
			{
		    getPMapElement()->Move(this);        //������Ӧ�ƶ�
			if (WhichMapElement(*this))
			{
				MoveBack();
			}
			}
	}
	else
	{
		WhichMapElement(*this);     //��Ȼ�������ǵü���Ƿ���ײ
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
	/*******����*****/
	if (getHP()>0)
	{
		/**************************̹���ƶ��Ȳ���******************************/
		Update();                      //̹�˴����ƶ�������������ڵ��������

		/**************************̹�˻���******************************/
		SelectObject(g_hbufdc,m_hBMP);  //����̹��
		TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),50,75,g_hbufdc,getFramNumber()*50,getDirection()*75,50,75,RGB(252,252,252));


		/**************************�ڵ�******************************/
		if (m_pBullet.getIsFly())      //����ڵ�״̬����ȡ��Ӧ����
		{
			m_pBullet.Paint(hmdc);
		}


		 /**************************Ѫ��******************************/
		SelectObject(g_hbufdc,g_hBloodBar);
		int bloodX = DrawPoint().getX()-8;
		int bloodY = DrawPoint().getY()-15;
		TransparentBlt(hmdc,bloodX,bloodY,64,18,g_hbufdc,0,0,64,18,RGB(0,1,2));  //Ѫ����������
		HBRUSH hbrush;
		if (getHP()==2)
		{
			hbrush = CreateSolidBrush(RGB(255,102,0));  //�ٻ�
		}
		else if (getHP()==1)
		{
			hbrush = CreateSolidBrush(RGB(255,0,0));    //��ɫ
		}
		else
		{
			hbrush = CreateSolidBrush(RGB(9,247,56));   //��ɫ
		}
		int bloodRectLUX = bloodX+3;
		int bloodRectLUY = bloodY+4;
		int bloodRectRDX = bloodX+60*getHP()/MaxHP;
		int bloodRectRDY = bloodY+14;
		SelectObject(hmdc,hbrush);
		Rectangle(hmdc,bloodRectLUX,bloodRectLUY,bloodRectRDX,bloodRectRDY);  //Ѫ������
		DeleteObject(hbrush);

		/**************************����******************************/
		LevelUP(hmdc);    //����Ƿ�����
		g_score_pre = g_score;
	}
	/******����******/
	else
	{
		/**************************�����������߼�¼����д���¼******************************/
		if (g_score>g_HistoryBestScore)//����д���ļ�
		{
			g_HistoryBestScore = g_score;
			fstream wfile("HistoryBestScore.txt");
			wfile<<g_HistoryBestScore;
		}
		/**************************��ը����******************************/
		switch (m_iBomb)      //��ը����
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
		TransparentBlt(hmdc,DrawPoint().getX(),DrawPoint().getY(),64,64,g_hbufdc,0,0,64,64,RGB(0,0,0));  //̹�˱�ը����
		m_iBomb++;
		if (m_pBullet.getIsFly())    //����ڵ�״̬����ȡ��Ӧ����
		{
			m_pBullet.Paint(hmdc);
		}
	}

}
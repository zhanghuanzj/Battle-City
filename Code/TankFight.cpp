#include <windows.h>
#include <string>
#include <stdlib.h>
#include <tchar.h>            //swprintf_s()
#include <ctime>
#include <winuser.h>
#include <fstream>
#include "Define.h"
#include "SPoint.h"
#pragma comment(lib,"msimg32.lib")  //TransparentBlt()
using namespace std;



int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT ("TankFight");
    HWND         hwnd;
	MSG          msg={0};
    WNDCLASSEX   wndclassex = {0};
    wndclassex.cbSize        = sizeof(WNDCLASSEX);
    wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
    wndclassex.lpfnWndProc   = WndProc;
    wndclassex.cbClsExtra    = 0;
    wndclassex.cbWndExtra    = 0;
    wndclassex.hInstance     = hInstance;
    wndclassex.hIcon         = LoadIcon (hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
    wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
    wndclassex.lpszMenuName  = NULL;
    wndclassex.lpszClassName = szAppName;
    wndclassex.hIconSm       = wndclassex.hIcon;
	
    if (!RegisterClassEx (&wndclassex))
    {
        MessageBox (NULL, TEXT ("RegisterClassEx failed!"), szAppName, MB_ICONERROR);
        return 0;
    }
    hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		                  szAppName, 
        		          TEXT ("̹�˴�ս"),
                		  WS_OVERLAPPEDWINDOW,
		                  CW_USEDEFAULT, 
        		          CW_USEDEFAULT, 
                		  WINDOW_WIDTH, 
		                  WINDOW_HEIGHT, 
        		          NULL, 
                		  NULL, 
		                  hInstance,
        		          NULL); 
						  
    ShowWindow (hwnd, iCmdShow);
	MoveWindow(hwnd,400,0,WINDOW_WIDTH,WINDOW_HEIGHT,true);
    UpdateWindow (hwnd);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE2),NULL,SND_ASYNC|SND_LOOP|SND_RESOURCE);
	if (!Game_init(hwnd,hInstance))
	{
		MessageBox (NULL, TEXT ("Game initialize failed!"), szAppName, MB_ICONERROR);
	}
   while (msg.message!=WM_QUIT)
    {
		if (PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);

		}
		else
		{
			g_tnow = GetTickCount();
			if (g_tnow-g_tpre>30)
			{
				//static int SndNum = 0;
				//if (SndNum==100)
				//{
					/*PlaySound(MAKEINTRESOURCE(IDR_WAVE2),NULL,SND_ASYNC|SND_LOOP|SND_RESOURCE|SND_NOSTOP);*/
			/*	}
				SndNum++;
				SndNum%=101;*/
				Game_prePaint(hwnd);
			}
		}

    }
    return msg.wParam;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    switch (message)
    {
    case WM_CREATE:
        return (0);
	case WM_LBUTTONDOWN:
		if (GameOver)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if(x>=410&&x<=570&&y>=800&&y<=930)
				Game_init(hwnd,g_hinstance);
		}
		else if (!GameBegin)
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if(x>=387&&x<=611&&y>=882&&y<=950)
				GameBegin = true;
		}
		break;
    case WM_PAINT:
		{
        hdc = BeginPaint (hwnd, &ps);
		wchar_t s[20] ;
		swprintf_s(s,TEXT("A Window"));
        TextOut (hdc, 0, 0,s,wcslen(s));
        EndPaint (hwnd, &ps);
        return (0);
		}
		
    case WM_DESTROY:
        PostQuitMessage (0);
        return (0);
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}
void loadHistoryBestScore()
{
	fstream rfile("HistoryBestScore.txt");  
	rfile>>g_HistoryBestScore; 
}
bool Game_init(HWND hwnd,HINSTANCE hinstance) 
{
	loadHistoryBestScore();
	HBITMAP bmp;
	g_hdc = GetDC(hwnd);                     /*���������ĳ�ʼ��*/
	g_hmdc = CreateCompatibleDC(g_hdc);
	g_hbufdc = CreateCompatibleDC(g_hdc);
	bmp = CreateCompatibleBitmap(g_hdc,WINDOW_WIDTH,WINDOW_HEIGHT);
    
	SelectObject(g_hmdc,bmp);
	g_hinstance = hinstance;
	g_turnNumber = 0;
	g_score = 0;
	g_score_pre = 0;
	g_roundNumber = 1;
	AssassinFlag = true;
	GameOver = false;
	GameBegin = false;
	RoundLoading = true;
	IsLevelUP = false;

	g_hEnemys[0]=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP20));
	g_hEnemys[1]=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP19));
	g_hEnemys[2]=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP21));
	g_hLTank = LoadBitmap(g_hinstance,MAKEINTRESOURCE(IDB_BITMAP22));
    g_hELife = LoadBitmap(g_hinstance,MAKEINTRESOURCE(IDB_BITMAP24));
	for (int i=0;i<MAPS_NUMBER;++i)             //���ص�ͼ
	{
		g_hBG[i] = LoadBitmap(hinstance,MAKEINTRESOURCE(g_maps[i]));
	}

	g_hBomb[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP15));  //���ƾ��
	g_hBomb[1] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP16));
	g_hBomb[2] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP17));
	g_hBomb[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP23));
	g_hBomb[4] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP25));
	g_hBomb[5] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP26));
	g_hBomb[6] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP27));
	g_hBomb[7] = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP28));

	g_hBullet[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_UP)); //�������
	g_hBullet[1] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_DOWN)); 
	g_hBullet[2] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_LEFT));
	g_hBullet[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_RIGHT)); 

	g_hBulletEx[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX1));//�������ƾ��
	g_hBulletEx[1] = g_hBulletEx[0];
	g_hBulletEx[2] = g_hBulletEx[0];
	g_hBulletEx[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX2));
	g_hBulletEx[4] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX3));
	g_hBulletEx[5] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX4));
	g_hBulletEx[6] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX5));
	g_hBulletEx[7] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX6));
	g_hBulletEx[8] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX7));
	g_hBulletEx[9] = LoadBitmap(hinstance,MAKEINTRESOURCE(Missile_EX8));

	g_hInitialize[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE1));
	g_hInitialize[1] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE2));
	g_hInitialize[2] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE3));
	g_hInitialize[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE4));
	g_hInitialize[4] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE5));
	g_hInitialize[5] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE6));
	g_hInitialize[6] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE7));
	g_hInitialize[7] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE8));
	g_hInitialize[8] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE9));
	g_hInitialize[9] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE10));
	g_hInitialize[10] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE11));
	g_hInitialize[11] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE12));
	g_hInitialize[12] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE13));
	g_hInitialize[13] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE14));
	g_hInitialize[14] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE15));
	g_hInitialize[15] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE16));
	g_hInitialize[16] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE17));
	g_hInitialize[17] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE18));
	g_hInitialize[18] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE19));
	g_hInitialize[19] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE20));
	g_hInitialize[20] = LoadBitmap(hinstance,MAKEINTRESOURCE(TANK_INITIALIZE21));


	g_hAssassin[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(Assassin_1));
	g_hAssassin[1] = LoadBitmap(hinstance,MAKEINTRESOURCE(Assassin_2));
	g_hAssassin[2] = LoadBitmap(hinstance,MAKEINTRESOURCE(Assassin_3));
	g_hAssassin[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(Assassin_4));

	g_hGameOverBG = LoadBitmap(hinstance,MAKEINTRESOURCE(GAMEOVER_BG));//��Ϸ����
	g_hRepalyButton = LoadBitmap(hinstance,MAKEINTRESOURCE(REPLAYBUTTON));
	g_hGameBegin = LoadBitmap(hinstance,MAKEINTRESOURCE(GAMEBEGIN));

	g_hRoundBG[0] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_01));
	g_hRoundBG[1] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_02));
	g_hRoundBG[2] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_03));
	g_hRoundBG[3] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_04));
	g_hRoundBG[4] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_05));
	g_hRoundBG[5] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_06));
	g_hRoundBG[6] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_07));
	g_hRoundBG[7] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_08));
	g_hRoundBG[8] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_09));
	g_hRoundBG[9] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_10));
	g_hRoundBG[10] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_11));
	g_hRoundBG[11] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_12));
	g_hRoundBG[12] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_13));
	g_hRoundBG[13] = LoadBitmap(hinstance,MAKEINTRESOURCE(ROUNDBG_14));

	g_hBloodBar = LoadBitmap(hinstance,MAKEINTRESOURCE(BloodBar));

	for (int i=0;i<26;i++)
	{
		g_hLevelUp[i] = LoadBitmap(hinstance,MAKEINTRESOURCE(LEVELUP01+i));   //�����������
	}

	GetClientRect(hwnd,&g_rect);//ȡ���ڲ����ھ���
	srand((unsigned)time(NULL));
	g_hGameOver = LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP14));//������Ϸ��������
	pLTank=new LightTank(0,FRONT,3,400,400,hinstance);/*����̹��*/


	int assassinPlaceX ;
	switch (rand()%2)
	{
	case 0:
		assassinPlaceX = 32;
		break;
	case 1:
		assassinPlaceX = g_rect.right-32;
		break;
	default:
		break;
	}
	enemys[0]=new Assassin(1,FRONT,1,assassinPlaceX,g_rect.bottom/2-64,hinstance);//����Assassin
	enemys[0]->setHBMP();
	enemys[0]->setIsAlive(false);

	

	
	for (int i=0;i<g_enemyNumber;i++)
	{
		int Ex,Ey;
		switch (rand()%2)
		{
		case 0:
			Ey = 32;
			break;
		case 1:
			Ey = g_rect.bottom-32;
			break;
		default:
			break;
		}

		switch (rand()%3)
		{
		case 0:
			Ex = 32;
			break;
		case 1:
			Ex = 480;
			break;
		case 2:
			Ex = g_rect.right-32;
			break;
		default:
			break;
		}
		newEnemy[i] = new EnemyTank(2,FRONT,1,Ex,Ey,hinstance);
		newEnemy[i]->setIsAlive(false);
		EnemyTank::Creature(i);
		EnemyTank::Fitness(i);
		newEnemy[i]->setSpeed(newEnemy[i]->getScenario(9));
	}


   Game_prePaint(hwnd);
   return true;
}

void Game_prePaint(HWND hwnd)
{
	if (!GameBegin)
	{
		SelectObject(g_hbufdc,g_hGameBegin);
		BitBlt(g_hmdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_hbufdc,0,0,SRCCOPY);
		BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_hmdc,0,0,SRCCOPY);
	}
	else if(RoundLoading&&GameBegin)
	{
		SelectObject(g_hbufdc,g_hRoundBG[(g_roundNumber-1)%14]);
		BitBlt(g_hmdc,0,0,980,1000,g_hbufdc,0,0,SRCCOPY);             //chapter ����ͼ
		

		wchar_t chapter[30];
		swprintf_s(chapter,TEXT("Chapter %d"),g_roundNumber);   //��߼�¼

		HFONT hFont = CreateFont(40,40,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,TEXT("΢���ź�"));  //�½�����
		SelectObject(g_hmdc,hFont);
		SetBkMode(g_hmdc,TRANSPARENT);
		SetTextColor(g_hmdc,RGB(255,0,0));
		TextOut(g_hmdc,300,100,chapter,wcslen(chapter));
		DeleteObject(hFont);
		BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_hmdc,0,0,SRCCOPY);   

		Sleep(3000);
		RoundLoading = false;
	}
	else
	{
		Game_paint(hwnd);
	}
	g_tpre = GetTickCount();   /*ʱ�����*/
}
void Game_turn(HWND hwnd)
{
	/*srand((unsigned)time(nullptr));*/
	RoundLoading = true;
	pLTank->setLocationX(400);
	pLTank->setLocationY(400);
	EnemyTank::SortEnemy();           //��ǰһ��̹�˵÷�����
	for (int i=0;i<g_enemyNumber;i++)
	{
		delete oldEnemy[i];
		oldEnemy[i] = newEnemy[i];    //��ǰһ��̹�˷���ɵ�̹������
		
	}
	for (int i=0;i<g_enemyNumber;i++)
	{
		int Ex,Ey;
		switch (rand()%2)
		{
		case 0:
			Ey = 32;
			break;
		case 1:
			Ey = g_rect.bottom-32;
			break;
		default:
			break;
		}

		switch (rand()%3)
		{
		case 0:
			Ex = 32;
			break;
		case 1:
			Ex = 480;
			break;
		case 2:
			Ex = g_rect.right-32;
			break;
		default:
			break;
		}

		newEnemy[i] = new EnemyTank(2,FRONT,1,Ex,Ey,g_hinstance);
		newEnemy[i]->setIsAlive(false);
		int chooseNum1 = rand()%4;
		int chooseNum2 = rand()%4;
		EnemyTank::CrossOver(i,chooseNum1,chooseNum2);  //������һ��̹�ˣ������л���ͻ��
		EnemyTank::Mutation(i);
		EnemyTank::Fitness(i);
		newEnemy[i]->setSpeed(newEnemy[i]->getScenario(9));
	}
	g_turnNumber = 0;
	AssassinFlag = true;
	g_roundNumber++;
	pLTank->increaseHP();         //��̹�˽��������ָ�
}

void Game_paint(HWND hwnd)
{
	/*��ͼ�����*/
	for (int i=0;i<15;i++)
	{
		for (int j=0;j<15;j++)
		{
			SelectObject(g_hbufdc,g_hBG[map[(g_roundNumber-1)%5][i][j]-101]);    //map[i][j]��¼���Ƕ���ĵ�ͼԪ�ر��-101�ŵõ���Ӧ�ĵ�ͼԪ�ؾ��
			BitBlt(g_hmdc,j*64,i*64,64,64,g_hbufdc,0,0,SRCCOPY);
		}
	}

	if (pLTank->getIsAlive())      //����̹��û��
	{
		
		/*̹�������Ϣ*/
		wchar_t Tlife[30],EnemyTankNumber[30],score[20],roundNum[10];

		/*swprintf_s(E0c,TEXT("E0X fitness is %d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d ,%d"),newEnemy[0]->getFitness(),newEnemy[1]->getFitness(),newEnemy[2]->getFitness(),newEnemy[3]->getFitness(),newEnemy[4]->getFitness(),newEnemy[5]->getFitness(),newEnemy[6]->getFitness(),newEnemy[7]->getFitness(),newEnemy[8]->getFitness(),newEnemy[9]->getFitness());
		TextOut (g_hmdc, 0, 260,E0c,wcslen(E0c));*/
		


		/***************����ֵ����**********/
		HFONT hfont= CreateFont(20,8,0,0,3,0,0,0,GB2312_CHARSET,0,0,0,0,TEXT("΢���ź�"));
		SetBkMode(g_hmdc,TRANSPARENT);
		SetTextColor(g_hmdc,RGB(0,0,0));
		SelectObject(g_hmdc,hfont);   //����

		SelectObject(g_hbufdc,g_hLTank);
		TransparentBlt(g_hmdc,0,320,32,32,g_hbufdc,0,0,32,32,RGB(0,0,0));
		swprintf_s(Tlife,TEXT("%d"),pLTank->getHP());
		TextOut(g_hmdc,34,330,Tlife,wcslen(Tlife));
		/*************�ҷ�̹�˻���***************/
		pLTank->Paint(g_hmdc);

		/*************Assassin����****************/
		if (enemys[0]->getIsAlive())
		{
		     enemys[0]->Paint(g_hmdc);
		}

		/**********�з�̹�˻���**************/
		int aliveNumber=0;
		startTime = GetTickCount();
		for (int i=0;i<g_enemyNumber;i++)
		{
			if (newEnemy[i]->getIsAlive())
			{
				newEnemy[i]->Paint(g_hmdc);
				aliveNumber++;
			}			
		}
		/************************������3��̹��*************************************/
		while (aliveNumber<3&&g_turnNumber<20) 
		{
			newEnemy[g_turnNumber++]->setIsAlive(true);
			newEnemy[g_turnNumber-1]->decreaseHP(); /////////////////////////////////////////////////////////////
		
			aliveNumber++;
			//if (g_turnNumber==10) ///////////////////////////////////////////////////////
			//{
			//	break;
			//}
		}
        /************************��ʮ��̹�˳���ʱ �����̿ͳ�*************************************/	
		if (g_turnNumber==10&&AssassinFlag)              
		{
			if (enemys[0]->getIsAlive()==false)
			{
				delete enemys[0];
				int assassinPlaceX ;
				switch (rand()%2)
				{
				case 0:
					assassinPlaceX = 32;
					break;
				case 1:
					assassinPlaceX = g_rect.right-32;
					break;
				default:
					break;
				}
				enemys[0]=new Assassin(1,FRONT,1,assassinPlaceX,g_rect.bottom/2-64,g_hinstance);//����Assassin
				enemys[0]->setHBMP();
				enemys[0]->setIsAlive(true);
				AssassinFlag = false;
				///////////////////////////*enemys[0]->decreaseHP();*/////////////////////////////////////  
			}
		}

		/************************�з�̹����������*************************************/	
		SelectObject(g_hbufdc,g_hELife);
		TransparentBlt(g_hmdc,-10,285,52,32,g_hbufdc,0,0,52,32,RGB(0,0,0));
		swprintf_s(EnemyTankNumber,TEXT("%d"),20-g_turnNumber+aliveNumber);
		TextOut(g_hmdc,35,295,EnemyTankNumber,wcslen(EnemyTankNumber));


		/************************����,�غϻ���****************************************/
		swprintf_s(score,TEXT("SCORE:%d"),g_score);
		TextOut(g_hmdc,0,355,score,wcslen(score));
		swprintf_s(roundNum,TEXT("ROUND:%d"),g_roundNumber);
		TextOut(g_hmdc,0,376,roundNum,wcslen(roundNum));

		/************************ȫ���ݻ�ʱ������һ��****************************/
		if (aliveNumber==0&&g_turnNumber==20&&!enemys[0]->getIsAlive())  
		{
			Game_turn(hwnd);
		}



		DeleteObject(hfont);
	}
	else     /**************Game Over***********************/
	{
		if (pLTank->getIsAlive())         //��ըͼ
		{
			pLTank->Paint(g_hmdc);
		}
		else                //����ͼ
		{
			GameOver = true;
			SelectObject(g_hbufdc,g_hGameOverBG);
			BitBlt(g_hmdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_hbufdc,0,0,SRCCOPY);     //��Ϸ��������

			SelectObject(g_hbufdc,g_hGameOver);
			TransparentBlt(g_hmdc,0,372,980,591,g_hbufdc,0,0,980,591,RGB(0,0,0));   //GAMEOVER
			
			SelectObject(g_hbufdc,g_hRepalyButton);
			TransparentBlt(g_hmdc,410,800,160,130,g_hbufdc,0,0,160,130,RGB(0,0,0)); //REPLAY

			wchar_t score[30];
			swprintf_s(score,TEXT("BEST SCORE %d !"),g_HistoryBestScore);   //��߼�¼


		    
			HFONT hFont = CreateFont(35,25,0,0,0,0,0,0,GB2312_CHARSET,0,0,0,0,TEXT("΢���ź�"));
			SelectObject(g_hmdc,hFont);
			SetBkMode(g_hmdc,TRANSPARENT);
			SetTextColor(g_hmdc,RGB(255,0,0));
			TextOut(g_hmdc,275,100,score,wcslen(score));
			DeleteObject(hFont);
		}

	}
	/*ȫ�����Ƶ�����������*/
	BitBlt(g_hdc,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,g_hmdc,0,0,SRCCOPY);

	g_tpre = GetTickCount();   /*ʱ�����*/
}

//��Ϸ�˳�����
bool Game_clean(HWND hwnd)
{
	for (int i=0;i<MAPS_NUMBER;i++)
	{
		DeleteObject(g_hBG[i]);
	}
	for (int i=0;i<3;i++)
	{
		 DeleteObject(g_hEnemys[i]);
	}
	DeleteObject(g_hLTank);
	DeleteObject(g_hELife);
	for (int i=0;i<8;i++)
	{
		DeleteObject(g_hBomb[i]);
	}
	for (int i=0;i<4;i++)
	{
		DeleteObject(g_hBullet[i]);//�������ƾ��
		DeleteObject(g_hAssassin[i]);
	}
	for (int i=0;i<10;i++)
	{
		DeleteObject(g_hBulletEx[i]);
	}
	for ( int i=0;i<21;i++)
	{
		DeleteObject(g_hInitialize[i]);
	}

	DeleteObject(g_hGameBegin);
	DeleteObject(g_hGameOver);
	DeleteObject(g_hRepalyButton);

	for (int i=0;i<14;i++)
	{
		DeleteObject(g_hRoundBG[i]);
	}

	for (int i=0;i<26;i++)
	{
		DeleteObject(g_hLevelUp[i]);  //�����������
	}
    DeleteObject(g_hBloodBar);
	DeleteDC(g_hbufdc);
	DeleteDC(g_hmdc);
	ReleaseDC(hwnd,g_hdc);
	return true;
}

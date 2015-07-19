#include "Tank.h"
#include <tchar.h>


Tank::Tank(int id,Direction dir,int hp,int loc_x,int loc_y,int mvSpeed):m_pBullet(loc_x,loc_y,this,dir,8,false),
													MaxHP(3),
												    isInvincible(true),
													InvincibleTime(63),
													m_iBomb(1),
													m_iAstarInterval(0),
													m_iID(id),
													m_dDirection(dir),
													m_iHP(hp),
													m_bIsAlive(true),
													m_iMoveSpeed(mvSpeed),
													 m_pLocation(loc_x,loc_y),
													 m_pDrawPoint(loc_x-25,loc_y-38),
													 m_pMapCoordinate(loc_x/64+1,loc_y/64+1)
{
}

void Tank::increaseHP()
{
	if (m_iHP<MaxHP)
	{
		m_iHP++;
	}
}

void Tank::decreaseHP(int n)
{
	m_iHP-=n;
}





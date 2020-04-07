#include "stdafx.h"
#include "tank.h"
tank::tank()
{
}
tank::~tank()
{
}

HRESULT tank::init(const char * imageName)
{

	//탱크 방향설정
	_direction = TANKDIRECTION_UP;

	_image = IMAGEMANAGER->findImage(imageName);

	//속도
	_speed = 100.0f;

	_bullet = new bullet;

	_bullet->init("bullet", 10, 640);

	return S_OK;
}

void tank::release()
{
	SAFE_DELETE(_bullet);
}

void tank::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LEFT) && _rc.left > 0)
	{
		tankMove();
		_direction = TANKDIRECTION_LEFT;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT) && _rc.right < WINSIZEX)
	{
		tankMove();
		_direction = TANKDIRECTION_RIGHT;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP) && _rc.top > 0)
	{
		tankMove();
		_direction = TANKDIRECTION_UP;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN) && _rc.bottom < WINSIZEY)
	{
		tankMove();
		_direction = TANKDIRECTION_DOWN;
	}
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
	{
		fire();
	}
	_bullet->update();
	bulletCollision();

}

void tank::render()
{
	_image->frameRender(getMemDC(), _rc.left, _rc.top, 0, _frameY);
	_bullet->render();

	char strBlock[128];
	sprintf_s(strBlock, "현재 위치 :  %d번 타일, Y : %d 타일", tileX, tileY);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	TextOut(getMemDC(), 300, 10, strBlock, strlen(strBlock));
}

void tank::setTankPosition(RECT rc)
{
	_rc = rc;
	_x = _rc.left + (_rc.right - _rc.left) / 2;
	_y = _rc.top + (_rc.bottom - _rc.top) / 2;
}

void tank::tankMove()
{
	RECT rcCollision;
	int tileIndex[2];//검사용 타일

	rcCollision = _rc;//가상의 렉트

	//타임매니저를 이용한 방법
	float elpasedTime = TIMEMANAGER->getElapsedTime();
	float moveSpeed = elpasedTime * _speed;

	//일단 무조건 이동ㄱㄱ
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		_frameY = 2;
		_x -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(),
			_image->getFrameHeight());
		break;
	case TANKDIRECTION_RIGHT:
		_frameY = 1;
		_x += moveSpeed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(),
			_image->getFrameHeight());
		break;
	case TANKDIRECTION_UP:
		_frameY = 3;
		_y -= moveSpeed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(),
			_image->getFrameHeight());
		break;
	case TANKDIRECTION_DOWN:

		_frameY = 0;
		_y += moveSpeed;
		rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(),
			_image->getFrameHeight());
		break;
	}//end of switch(_direction)

	//땅크가 지금 밟고 있는 타일 번호를 알아오자.
	tileX = rcCollision.left / TILESIZE;
	tileY = rcCollision.top / TILESIZE;

	//땅크가 타일 어디에 있는지 확인이 가능하므로
	//땅크 방향에 따라서 그 앞의 타일 번호를 계산한다.
	//인덱스 번호가 2개인 이유는 렉트의 레프트, 탑 위치로 땅크의 위치를 구하고
	//렉트의 레프트, 탑이 타일번호에 걸쳐져 있을수도, 딱 맞게 있을수도 있기 때문에
	//걸쳐져 있을경우 걸친  그 밑의 타일의 값(ex : 오른쪽일 경우 땅크 위치 밑에 있는 값 앞에 있는 타일)을 가져온다.
	//그래서 정확하게 걸치지 않았을때 통과 할수 있도록.....


	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		tileIndex[0] = tileX + tileY * TILEX;		//땅크가 밟고 있는 녀석
		tileIndex[1] = tileX + (tileY + 1)* TILEX;	//땅크가 밟고 있는 왼쪽
		break;
	case TANKDIRECTION_RIGHT:
		tileIndex[0] = (tileX + tileY * TILEX) + 1;
		tileIndex[1] = (tileX + (tileY + 1)*TILEX) + 1;	//땅크가 밟고 있는 오른쪽
		break;
	case TANKDIRECTION_UP:
		tileIndex[0] = tileX + tileY * TILEX;
		tileIndex[1] = tileX + 1 + tileY * TILEX;
		break;
	case TANKDIRECTION_DOWN:
		tileIndex[0] = (tileX + tileY * TILEX) + TILEX;
		tileIndex[1] = (tileX + 1 + tileY * TILEX) + TILEX;
		break;
	}//end of switch(_direction)

	for (int i = 0; i < 2; i++)
	{
		
		RECT rc;
		//해당타일의 속성이 움직이지 못하는 곳이면....
		if (((_tankMap->getAttribute()[tileIndex[i]] & ATTR_UNMOVABLE) == ATTR_UNMOVABLE) &&
			IntersectRect(&rc, &_tankMap->getMap()[tileIndex[i]].rcTile, &rcCollision))
		{
			//움직이려 할때 갈수 없는 지역이면 탱크의 움직임을 고정하자
			//ex)땅크가 왼쪽으로갈때 왼쪽지역이 갈수 없으면
			switch (_direction)
			{
			case TANKDIRECTION_LEFT:
				_rc.left = _tankMap->getMap()[tileIndex[i]].rcTile.right;	//왼쪽 타일의 라이트의 위치를 탱크의 래프트의 위치로 고정
				_rc.right = _rc.left + _image->getFrameWidth();				//탱크의 크기만큼
				_x = _rc.left + (_rc.right - _rc.left) / 2;					//탱크 x의 위치를 계산한 탱크 랙트의 위치값으로 설정(중앙으로)
				break;
			case TANKDIRECTION_RIGHT:
				_rc.right = _tankMap->getMap()[tileIndex[i]].rcTile.left;
				_rc.left = _rc.right - _image->getFrameWidth();
				_x = _rc.left + (_rc.right - _rc.left) / 2;
				break;
			case TANKDIRECTION_UP:
				_rc.top = _tankMap->getMap()[tileIndex[i]].rcTile.bottom;
				_rc.bottom = _rc.top + _image->getFrameHeight();
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			case TANKDIRECTION_DOWN:
				_rc.bottom = _tankMap->getMap()[tileIndex[i]].rcTile.top;
				_rc.top = _rc.bottom - _image->getFrameHeight();
				_y = _rc.top + (_rc.bottom - _rc.top) / 2;
				break;
			}
			return;
		}
	}//end of for
	//움직이자
	rcCollision = RectMakeCenter(_x, _y, _image->getFrameWidth(), _image->getFrameHeight());
	_rc = rcCollision;
}

void tank::fire()
{
	switch (_direction)
	{
	case TANKDIRECTION_LEFT:
		_bullet->fire(_x, _y, PI, 200.0f);
		break;
	case TANKDIRECTION_RIGHT:
		_bullet->fire(_x, _y, 0, 200.0f);
		break;
	case TANKDIRECTION_UP:
		_bullet->fire(_x, _y, PI / 2, 200.0f);
		break;
	case TANKDIRECTION_DOWN:
		_bullet->fire(_x, _y, PI / 2 * 3, 200.0f);
		break;
	}
}

void tank::bulletCollision()
{
	RECT rc;
	for (int i = 0; i < _bullet->getVBullet().size(); i++)
	{
		//총알이 화면 밖으로 나가면
		if (_bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2 < 0
			|| _bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2 > WINSIZEX
			|| _bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2 < 0
			|| _bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2 > WINSIZEY)
		{
			_bullet->removeMissile(i);
			return;
		}

		int tileIndex;	//검사할 타일
		int tileX, tileY;//총알 날아가고 있는 타일 x,y

		if (_bullet->getVBullet()[i].angle == PI)//왼쪽으로 쏠때
		{
			tileX = (_bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2) / TILESIZE;
			tileY = (_bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2) / TILESIZE;
			tileIndex = tileX + tileY * TILEX;

		}
		else if (_bullet->getVBullet()[i].angle == 0)//오른쪽으로 쏠때
		{
			tileX = (_bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2) / TILESIZE;
			tileY = (_bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2) / TILESIZE;
			tileIndex = tileX + tileY * TILEX;
		}
		else if (_bullet->getVBullet()[i].angle == PI / 2)//위로쏠때
		{
			tileX = (_bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2) / TILESIZE;
			tileY = (_bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2) / TILESIZE;
			tileIndex = tileX + tileY * TILEX;
		}
		else if (_bullet->getVBullet()[i].angle == PI / 2 * 3)//아래로 쏠때
		{
			tileX = (_bullet->getVBullet()[i].rc.left + IMAGEMANAGER->findImage("bullet")->getWidth() / 2) / TILESIZE;
			tileY = (_bullet->getVBullet()[i].rc.top + IMAGEMANAGER->findImage("bullet")->getHeight() / 2) / TILESIZE;
			tileIndex = tileX + tileY * TILEX;
		}
	
	}
}

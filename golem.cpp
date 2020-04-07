#include "stdafx.h"
#include "golem.h"

HRESULT golem::init()
{
	_hp = 100;
	_atk = 18;
	_speed = 1;

	_frameX = 0;
	_frameY = 0;
	_frameCount = 0;

	_isSummonComplete = false;

	_moveCoolTime = 0;

	_atkCoolTime = 3;

	_isFindMoster = false;
	return S_OK;
}

void golem::update()
{ 
	if (!m_player->getIsSummonAlive())
	{  
		_hp = 100;
		_vAStar.clear();
		_isMove = false;
		_isAtk = false;
		_isHit = false;
		_isDeath = false;
		_deathCheck = false;
		// 재 소환 시 기존에 이동 하던 값 초기화
		_moveYCount = _moveYMaxCount = _moveXCount = _moveXMaxCount = 0;
		_isMoveEnd = false;
		_isEndPointCheck = false;
		_isFindRoad = false;
		_deathIndex = 0;
		_golem = IMAGEMANAGER->findImage("fire_golem_summon");
		if (m_player->getSummonDir() == 4) _golemRc = (RectMake(m_player->getPlayerRc().left - 120, m_player->getPlayerRc().top + 15, 25, 25));
		if (m_player->getSummonDir() == 5) _golemRc = (RectMake(m_player->getPlayerRc().left - 220, m_player->getPlayerRc().top - 35, 25, 25));
		if (m_player->getSummonDir() == 6) _golemRc = (RectMake(m_player->getPlayerRc().left - 320, m_player->getPlayerRc().top - 85, 25, 25));
		if (m_player->getSummonDir() == 7) _golemRc = (RectMake(m_player->getPlayerRc().left - 220, m_player->getPlayerRc().top - 135, 25, 25));
		if (m_player->getSummonDir() == 0) _golemRc = (RectMake(m_player->getPlayerRc().left - 120, m_player->getPlayerRc().top - 185, 25, 25));
		if (m_player->getSummonDir() == 1) _golemRc = (RectMake(m_player->getPlayerRc().left - 20, m_player->getPlayerRc().top - 135, 25, 25));
		if (m_player->getSummonDir() == 2) _golemRc = (RectMake(m_player->getPlayerRc().left + 80, m_player->getPlayerRc().top - 85, 25, 25));
		if (m_player->getSummonDir() == 3) _golemRc = (RectMake(m_player->getPlayerRc().left - 20, m_player->getPlayerRc().top - 35, 25, 25));
	} 

	if ( _isFindRoad && !_isMoveEnd && _vAStar.size() != 0 &&  !_deathCheck)
	{
		_golem = IMAGEMANAGER->findImage("fire_golem_move");
		_golemShadow = IMAGEMANAGER->findImage("fire_golem_move_shadow");
		rndMove();
	}  

	if (_isMoveEnd && _isMove && !_deathCheck) waitMove();
	if (!_deathCheck)findMonster();
	animation();
	_golemColRc = RectMake(_golemRc.left + m_camera->getCameraX() + 10, _golemRc.top + m_camera->getCameraY() + 20, 55, 75);

	//if (_isFindMoster && _isMoveEnd )atkGolem();
	if ((_isFindMoster|| _isFindOgre || _isFindAnda) 
		&& _vAStar.size() == 0 
		&& m_player->getIsSummonAlive() 
		&& (!_vBalog.at(_findMonsterIndex)->getIsDeath() || !_vOgre.at(_findOgreIndex)->getIsDeath() || !_anda->getIsDeath()) 
		&& m_player->getIsSummonAlive() && !_deathCheck ) atkGolem();
	else if (_vAStar.size() == 0 && 
		(_vBalog.at(_findMonsterIndex)->getIsDeath() || _vOgre.at(_findOgreIndex)->getIsDeath() || _anda->getIsDeath() ) 
		&& m_player->getIsSummonAlive() && !_isDeath) setIdle();

	if (_hp < 0 && !_deathCheck) golemDeath();

	_currentX = (_golemRc.left + 20 + (_golemRc.top + 70) * 2) / 100;
	_currentY = (((_golemRc.top + 70 + 40) / 25 - (_golemRc.left + 14 + 25) / 50 + 1) / 2);
}

void golem::render()
{  
	if (_isMove ) _golem->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX(), _golemRc.top + m_camera->getCameraY(), _frameX, _golemDirection);
	else if (_isAtk) _golem->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX()-50, _golemRc.top + m_camera->getCameraY(), _frameX, _golemDirection);
	else if (_isHit) _golem->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 50, _golemRc.top + m_camera->getCameraY(), _hitIndex, _golemDirection);
	else if (_isDeath) _golem->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 50, _golemRc.top + m_camera->getCameraY(), _deathIndex, 0);
	else _golem->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX(), _golemRc.top + m_camera->getCameraY(), _frameX,0);
		
	
	//Rectangle(getMemDC(), _golemRc.left + m_camera->getCameraX(), _golemRc.top + m_camera->getCameraY(), _golemRc.right + m_camera->getCameraX(), _golemRc.bottom + m_camera->getCameraY());
}

void golem::shadowRender()
{
	if (_isMove)
	{
		_golemShadow->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 20, _golemRc.top + m_camera->getCameraY(), _frameX, _golemDirection);
	 }
	else if (_isAtk)
	{
		_golemShadow->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 80, _golemRc.top + m_camera->getCameraY(), _frameX, _golemDirection);
	}
	else if (_isHit) _golemShadow->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 80, _golemRc.top + m_camera->getCameraY(), _hitIndex, _golemDirection);
	else if (_isDeath);
	else
	{
		if (m_player->getIsSummonAlive()) _golemShadow->frameRender(getMemDC(), _golemRc.left + m_camera->getCameraX() - 20, _golemRc.top + m_camera->getCameraY(), _frameX, _golemDirection);
	}
}

void golem::animation()
{
	if (_isMove)
	{
		_frameCount++;
		if (_frameCount % 10 == 0)
		{
			_frameX++;
			if (_frameX > _golem->getMaxFrameX()-1)
			{
				_frameX = 0;   
			}
		}
	}
	else if (_isAtk)
	{ 
		_frameCount++;
		if (_frameCount % 3 == 0)
		{ 
			if(_frameX == 10 && !_vBalog.at(_findMonsterIndex)->getIsDeath() && _isFindMoster)
				_vBalog.at(_findMonsterIndex)->balogHit(_atk);
			else if(_frameX == 10 && !_vOgre.at(_findOgreIndex)->getIsDeath() && _isFindOgre)
				_vOgre.at(_findOgreIndex)->ogreHit(_atk);
			else if(_frameX == 10 && !_anda->getIsDeath() && _isFindAnda)
				_anda->andaHit(_atk);
			_frameX++;
			if (_frameX > _golem->getMaxFrameX())
			{
				_frameX = 0;
			}
		} 
	}
	else if (_isHit)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{ 
			_hitIndex++;
			if (_hitIndex > _golem->getMaxFrameX())
			{
				_hitIndex = 0;
				_isHit = false;
			}
		}
	}
	else if (_isDeath)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_deathIndex++;
			if (_deathIndex > _golem->getMaxFrameX())
			{
				_deathIndex = _golem->getMaxFrameX(); 
			}
		}
	}
	else
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _golem->getMaxFrameX())
			{
				_frameX = 0;
				_golem = IMAGEMANAGER->findImage("fire_golem_idle");
				_golemShadow = IMAGEMANAGER->findImage("fire_golem_idle_shadow");
				if (!m_player->getIsSummonAlive())_golemRc = (RectMake(_golemRc.left + 110, _golemRc.top, 25, 25));
				m_player->setIsSummonAlive(true); 
				_currentX = (_golemRc.left + 20 + (_golemRc.top + 70) * 2) / 100;
				_currentY = (((_golemRc.top + 70 + 40) / 25 - (_golemRc.left + 14 + 25) / 50 + 1) / 2);
			}
		}
	}  
}

void golem::rndMove()
{ 
	_isMove = true;
	_isAtk = false;
	int nextPointX = _vAStar.at(_vAStar.size() - 1) % TILEX;
	int nextPointY = _vAStar.at(_vAStar.size() - 1) / TILEX;

	// 시작지점 
	_startX = _currentX;
	_startY = _currentY;
	_endX = _vAStar.at(0) % TILEX;
	_endY = _vAStar.at(0) / TILEX;

	// 도착 지점까지 거리  
	_moveXDistance = nextPointX - _startX;
	_moveYDistance = nextPointY - _startY;
	if (_startX !=_endX || _startY != _endY)
	{
		if (!_isEndPointCheck)
		{
			if (nextPointX <  _startX && nextPointY  < _startY) _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _golemDirection = GOLEM_UP;
			if (nextPointX == _startX && nextPointY  < _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _golemDirection = GOLEM_RIGHTUP;
			if (nextPointX >  _startX && nextPointY  < _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _golemDirection = GOLEM_RIGHT;
			if (nextPointX >  _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _golemDirection = GOLEM_RIGHTDOWN;
			if (nextPointX >  _startX && nextPointY  > _startY)  _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _golemDirection = GOLEM_DOWN;
			if (nextPointX == _startX && nextPointY  > _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _golemDirection = GOLEM_LEFTDOWN;
			if (nextPointX <  _startX && nextPointY  > _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _golemDirection = GOLEM_LEFT;
			if (nextPointX <  _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _golemDirection = GOLEM_LEFTUP;
			_isEndPointCheck = true;
			_frameX = 0;
			_frameCount = 0;
		}
	}
	int _tempRcLeft = _golemRc.left;
	int _tempRcTop = _golemRc.top;
	int _speedX = _speed;
	int _speedY = _speed;
	if (_moveXMaxCount < 0) _speedX = -_speed;
	if (_moveYMaxCount < 0) _speedY = -_speed;

	if (_moveXCount < abs(_moveXMaxCount))
	{
		_tempRcLeft += (_speedX * 2);
		_moveXCount += _speed * 2; 
	}

	if (_moveYCount < abs(_moveYMaxCount))
	{
		_tempRcTop += _speedY;
		_moveYCount += _speed; 
	}

	if (_moveYCount >= abs(_moveYMaxCount) && _moveXCount >= abs(_moveXMaxCount))
	{
		//도착
		if (_currentX == _endX &&_currentY == _endY)
		{
			_moveYCount = _moveYMaxCount = _moveXCount = _moveXMaxCount = 0;
			_isFindRoad = false;
			_isMoveEnd = true;
			_isEndPointCheck = false;
			_moveCoolTime = 0;
		}
		else
		{
			_moveYCount = _moveYMaxCount = _moveXCount = _moveXMaxCount = 0;
			_isEndPointCheck = false;
		}
		_vAStar.pop_back();
	}

	_golemRc = RectMake(_tempRcLeft, _tempRcTop, 25, 25);

	_currentX = (_tempRcLeft + 20 + (_tempRcTop + 70) * 2) / 100;
	_currentY = ((_tempRcTop + 70 + 40) / 25 - (_tempRcLeft + 14 + 25) / 50 + 1) / 2;
}

void golem::waitMove()
{  
	_golem = (IMAGEMANAGER->findImage("fire_golem_idle"));
	_golemShadow = (IMAGEMANAGER->findImage("fire_golem_idle_shadow"));

	_moveCoolTime += TIMEMANAGER->getElapsedTime();

	if (_moveCoolTime > 3) 
		_isMoveEnd = false;
}

void golem::findMonster()
{
	int _tempDis;
	for (int i = 0; i < _vBalog.size(); i++)
	{  
		if (!_vBalog.at(i)->getIsDeath() && !(_currentX == _vBalog.at(i)->getCurrentX() && _currentY == _vBalog.at(i)->getCurrentY()))
		{
			_tempDis = abs(_currentX - _vBalog.at(i)->getCurrentX()) + abs(_currentY - _vBalog.at(i)->getCurrentY()); 
			if (_tempDis < 4)
			{
				_isFindAnda = false;
				_isFindOgre = false;
				_isFindMoster = true;
				_findMonsterIndex = i;
				break;
			}
		}
	}

	for (int i = 0; i < _vOgre.size(); i++)
	{ 
		if (!_vOgre.at(i)->getIsDeath() && !(_currentX == _vOgre.at(i)->getCurrentX() && _currentY == _vOgre.at(i)->getCurrentY()))
		
		{
			_tempDis = abs(_currentX - _vOgre.at(i)->getCurrentX()) + abs(_currentY - _vOgre.at(i)->getCurrentY()); 

			if (_tempDis < 4)
			{
				_isFindAnda = false;
				_isFindMoster = false;
				_isFindOgre = true;
				_findOgreIndex = i;
				break;
			}
		}
	}

	if (!_anda->getIsDeath() && !(_currentX == _anda->getCurrentX() && _currentY == _anda->getCurrentY()))
	{
		_tempDis = abs(_currentX - _anda->getCurrentX()) + abs(_currentY - _anda->getCurrentY());
		if (_tempDis < 4)
		{
			_isFindAnda = true;
			_isFindOgre = false;
			_isFindMoster = false;  
		}
	}
}

void golem::atkGolem()
{
	_isMove = false;
	_isAtk = true;
	_golem = IMAGEMANAGER->findImage("fire_golem_atk"); 
	_golemShadow = IMAGEMANAGER->findImage("fire_golem_atk_shadow"); 

	if (!_vBalog.at(_findMonsterIndex)->getIsDeath())
	{
		int i = _findMonsterIndex;
		if (_vBalog.at(i)->getCurrentX() < _currentX && _vBalog.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_UP;
		if (_vBalog.at(i)->getCurrentX() == _currentX && _vBalog.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHTUP;
		if (_vBalog.at(i)->getCurrentX() > _currentX && _vBalog.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHT;
		if (_vBalog.at(i)->getCurrentX() > _currentX && _vBalog.at(i)->getCurrentY() == _currentY)  _golemDirection = GOLEM_RIGHTDOWN;
		if (_vBalog.at(i)->getCurrentX() > _currentX && _vBalog.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_DOWN;
		if (_vBalog.at(i)->getCurrentX() == _currentX && _vBalog.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFTDOWN;
		if (_vBalog.at(i)->getCurrentX() < _currentX && _vBalog.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFT;
		if (_vBalog.at(i)->getCurrentX() < _currentX && _vBalog.at(i)->getCurrentY() == _currentY)  _golemDirection = GOLEM_LEFTUP;
	}
	else if (!_vOgre.at(_findOgreIndex)->getIsDeath())
	{
		int i = _findOgreIndex;
		if (_vOgre.at(i)->getCurrentX() < _currentX &&  _vOgre.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_UP;
		if (_vOgre.at(i)->getCurrentX() == _currentX && _vOgre.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHTUP;
		if (_vOgre.at(i)->getCurrentX() > _currentX &&  _vOgre.at(i)->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHT;
		if (_vOgre.at(i)->getCurrentX() > _currentX &&  _vOgre.at(i)->getCurrentY() == _currentY)  _golemDirection = GOLEM_RIGHTDOWN;
		if (_vOgre.at(i)->getCurrentX() > _currentX &&  _vOgre.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_DOWN;
		if (_vOgre.at(i)->getCurrentX() == _currentX && _vOgre.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFTDOWN;
		if (_vOgre.at(i)->getCurrentX() < _currentX &&  _vOgre.at(i)->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFT;
		if (_vOgre.at(i)->getCurrentX() < _currentX &&  _vOgre.at(i)->getCurrentY() == _currentY)  _golemDirection = GOLEM_LEFTUP;
	}
	else if (!_anda->getIsDeath())
	{ 
		if (_anda->getCurrentX() < _currentX &&  _anda->getCurrentY() < _currentY)  _golemDirection = GOLEM_UP;
		if (_anda->getCurrentX() == _currentX && _anda->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHTUP;
		if (_anda->getCurrentX() > _currentX &&  _anda->getCurrentY() < _currentY)  _golemDirection = GOLEM_RIGHT;
		if (_anda->getCurrentX() > _currentX &&  _anda->getCurrentY() == _currentY)  _golemDirection = GOLEM_RIGHTDOWN;
		if (_anda->getCurrentX() > _currentX &&  _anda->getCurrentY() > _currentY)  _golemDirection = GOLEM_DOWN;
		if (_anda->getCurrentX() == _currentX && _anda->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFTDOWN;
		if (_anda->getCurrentX() < _currentX &&  _anda->getCurrentY() > _currentY)  _golemDirection = GOLEM_LEFT;
		if (_anda->getCurrentX() < _currentX &&  _anda->getCurrentY() == _currentY)  _golemDirection = GOLEM_LEFTUP;
	}
}

void golem::setIdle()
{ 
	_isAtk = false;
	_isMove = true;
	_isFindRoad = false;
	_golem = IMAGEMANAGER->findImage("fire_golem_idle");
	_golemShadow = IMAGEMANAGER->findImage("fire_golem_idle_shadow");
}

void golem::golemHit(int atk, int dir)
{
	_isHit = true;
	_isAtk = false;
	_isMove = false;
	_hp = _hp - atk;
}

void golem::golemDeath()
{
	_deathCheck = true;
	_isDeath = true;
	_isHit = false;
	_isAtk = false;
	_isMove = false;
	_golem = IMAGEMANAGER->findImage("fire_golem_death");

	for (int i = 0; i < _vBalog.size(); i++)_vBalog.at(i)->golemDeath();
	for (int i = 0; i < _vOgre.size(); i++)_vOgre.at(i)->golemDeath();
	_anda->golemDeath();
}



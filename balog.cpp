#include "stdafx.h"
#include "balog.h"

HRESULT balog::init(int x , int y)
{ 
	//_balogRc = RectMake(336, 1962, 25, 25);
	//_balogRc = RectMake(-114, 2087, 25, 25);
	_balogRc = RectMake(x,y, 25, 25);
	_balog = IMAGEMANAGER->findImage("balog_idle");
	_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");

	_atkCoolTime = 0;
	_speed = 1;
	_hp = 50;
	_atk = 23; 
	_isStart = true;
	return S_OK;
}

void balog::update()
{
	if (_deathIndex != _balog->getMaxFrameX())
	{
		_currentX = (_balogRc.left + 20 + (_balogRc.top) * 2) / 100;
		_currentY = (((_balogRc.top - 12 + 40) / 25 - (_balogRc.left + 14 + 25) / 50 + 1) / 2);
		_balogColRc = RectMake(_balogRc.left + m_camera->getCameraX() - 10, _balogRc.top + m_camera->getCameraY() - 50, 55, 75);
		animation();
		if (!_isAtkTimeCheck)
		{
			if (!_isFindGolem)findTarget();
				if (_isFindRoad && !_isMoveEnd && _vAStar.size() > 0) balogMove();
				else if (_vAStar.size() == 0 && !_isStart) balogAtk();
		}
		else checkAtkCoolTime();
	}
	if (_hp < 0)
	{
		_vAStar.clear();
		_isAtk = false;
		_isMove = false;
		_isDeath = true;
		_balog = IMAGEMANAGER->findImage("balog_die");
		_balogShadow = IMAGEMANAGER->findImage("balog_die_shadow");
	}
}

void balog::render()
{ 
	if (_isDeath) _balog->frameRender(getMemDC(), _balogRc.left + m_camera->getCameraX() - 135, _balogRc.top + m_camera->getCameraY() - 140, _deathIndex, _deathDir);
	else  _balog->frameRender(getMemDC(), _balogRc.left + m_camera->getCameraX() - 135, _balogRc.top + m_camera->getCameraY() - 140, _frameX, _balogDirection);
}

void balog::shadowRender()
{
	if (_isDeath) _balogShadow->frameRender(getMemDC(), _balogRc.left + m_camera->getCameraX() - 195, _balogRc.top + m_camera->getCameraY() - 205, _deathIndex, _deathDir);
	else _balogShadow->frameRender(getMemDC(), _balogRc.left + m_camera->getCameraX() - 195, _balogRc.top + m_camera->getCameraY() - 205, _frameX, _balogDirection);
}

void balog::animation()
{ 
	if (_isAtk)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX == 13)
			{
				if(!_isFindGolem) m_player->playerHit(_atk, _balogDirection);
				else if (_isFindGolem) _golemHit = true;
			}
			if (_frameX > _balog->getMaxFrameX())
			{   
				_isAtk = false; 
				_isAtkTimeCheck = true;
				_frameCount = 0;
				_frameX = 0;
				_frameMaxCheck = false;
				_balog = IMAGEMANAGER->findImage("balog_idle");
				_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");
			}
		}
	} 
	else if (_isMove)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _balog->getMaxFrameX())
			{
				_frameCount = 0;
				_frameX = 0;
				_balog = IMAGEMANAGER->findImage("balog_idle");
				_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");
			}
		}
	}
	else if (_isDeath)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_deathIndex++;
			if (_deathIndex > _balog->getMaxFrameX())
			{
				_deathIndex = _balog->getMaxFrameX();
			}
		}
	}
	else if (_isHit)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _balog->getMaxFrameX())
			{
				_isHit = false;
				_frameCount = 0;
				_frameX = 0;
				_balog = IMAGEMANAGER->findImage("balog_idle");
				_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");
			}
		}
	}
	else
	{ 
		_frameCount++; 
		if (_frameCount % 5 == 0)
		{
			if(!_frameMaxCheck)_frameX++;
			else _frameX--;
			if (_frameX > _balog->getMaxFrameX())
			{
				_frameX--;
				_frameMaxCheck = true;
				_frameCount = 0; 			
			}
			else if(_frameX < 1)
			{
				_frameMaxCheck = false;
			}
		}
	}
	
}

void balog::findTarget()
{ 
	int _tempDis; 
	_tempDis = abs(_currentX - m_player->getPlayerCurrentX()) + abs(_currentY - m_player->getPlayerCurrentY()); 

	if (_tempDis < 6 && !_isAtk )
	{ 
		_isAtkTimeCheck = false;
		_isFindTarget = true; 
		if (!_isFindGolem)
		{
			_endX = m_player->getPlayerCurrentX();
			_endY = m_player->getPlayerCurrentY();
		} 
	} 
	else if (_tempDis >= 6) _isStart = true;
	else
	{
		_isIdle = true;
		_isFindTarget = false;
		_balog = IMAGEMANAGER->findImage("balog_idle");
		_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");
	}
}

void balog::balogMove()
{
	_isStart = false;
	_balog = IMAGEMANAGER->findImage("balog_move");
	_balogShadow = IMAGEMANAGER->findImage("balog_move_shadow");

	_isMove = true;
	_isAtk = false;
	int nextPointX = _vAStar.at(_vAStar.size() - 1) % TILEX;
	int nextPointY = _vAStar.at(_vAStar.size() - 1) / TILEX;
	
	//_endX = _vAStar.at(0) % TILEX;
	//_endY = _vAStar.at(0) / TILEX;
	// 시작지점 
	_startX = _currentX;
	_startY = _currentY; 

	// 도착 지점까지 거리  
	_moveXDistance = nextPointX - _startX;
	_moveYDistance = nextPointY - _startY;
	if (_startX != _endX || _startY != _endY)
	{
		if (!_isEndPointCheck)
		{
			if (nextPointX <  _startX && nextPointY <  _startY) _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _balogDirection = BALOG_UP;
			if (nextPointX == _startX && nextPointY <  _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _balogDirection = BALOG_RIGHTUP;
			if (nextPointX >  _startX && nextPointY <  _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _balogDirection = BALOG_RIGHT;
			if (nextPointX >  _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _balogDirection = BALOG_RIGHTDOWN;
			if (nextPointX >  _startX && nextPointY >  _startY)  _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _balogDirection = BALOG_DOWN;
			if (nextPointX == _startX && nextPointY >  _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _balogDirection = BALOG_LEFTDOWN;
			if (nextPointX <  _startX && nextPointY  > _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _balogDirection = BALOG_LEFT;
			if (nextPointX <  _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _balogDirection = BALOG_LEFTUP;
			_isEndPointCheck = true;
			_frameX = 0;
			_frameCount = 0;
		}
	}
	int _tempRcLeft = _balogRc.left;
	int _tempRcTop = _balogRc.top;
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
		if (_currentX == _endX && _currentY == _endY)
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

	_balogRc = RectMake(_tempRcLeft, _tempRcTop, 25, 25);

	//_currentX = (_balogRc.left + 20 + (_balogRc.top) * 2) / 100;
	//_currentY = (((_balogRc.top - 12 + 40) / 25 - (_balogRc.left + 14 + 25) / 50 + 1) / 2);
}

void balog::balogAtk()
{
	_isMove = false; 
	_isAtk = true;
	_balog = IMAGEMANAGER->findImage("balog_atk");
	_balogShadow = IMAGEMANAGER->findImage("balog_atk_shadow");
	
	if (_isFindGolem)
	{
		if (_endX < _currentX &&  _endY < _currentY)  _balogDirection = BALOG_UP;
		if (_endX == _currentX && _endY < _currentY)  _balogDirection = BALOG_RIGHTUP;
		if (_endX > _currentX &&  _endY < _currentY)  _balogDirection = BALOG_RIGHT;
		if (_endX > _currentX &&  _endY == _currentY) _balogDirection = BALOG_RIGHTDOWN;
		if (_endX > _currentX &&  _endY > _currentY)  _balogDirection = BALOG_DOWN;
		if (_endX == _currentX && _endY > _currentY)  _balogDirection = BALOG_LEFTDOWN;
		if (_endX < _currentX &&  _endY > _currentY)  _balogDirection = BALOG_LEFT;
		if (_endX < _currentX &&  _endY == _currentY) _balogDirection = BALOG_LEFTUP;

	}
	else
	{
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() < _currentY)  _balogDirection = BALOG_UP;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() < _currentY)  _balogDirection = BALOG_RIGHTUP;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() < _currentY)  _balogDirection = BALOG_RIGHT;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() == _currentY) _balogDirection = BALOG_RIGHTDOWN;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() > _currentY)  _balogDirection = BALOG_DOWN;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() > _currentY)  _balogDirection = BALOG_LEFTDOWN;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() > _currentY)  _balogDirection = BALOG_LEFT;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() == _currentY) _balogDirection = BALOG_LEFTUP;

	}
}

void balog::checkAtkCoolTime()
{
	_atkCoolTime += TIMEMANAGER->getElapsedTime();
	if (_atkCoolTime > 3)
	{
		_atkCoolTime = 0;
		_isAtkTimeCheck = false;
		
	}
}
 
void balog::golemDeath()
{
	_isFindGolem = false;
	_isEndPointCheck = false;
	_golemDeathCheck = false;
	//_balog = IMAGEMANAGER->findImage("balog_idle");
	//_balogShadow = IMAGEMANAGER->findImage("balog_idle_shadow");
}

void balog::balogHit(int damage)
{
	_frameX = 0;
	_hp = _hp - damage;
	_isHit = true;

	_balog = IMAGEMANAGER->findImage("balog_hit");
	_balogShadow = IMAGEMANAGER->findImage("balog_hit_shadow");
}
 

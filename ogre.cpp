#include "stdafx.h"
#include "ogre.h"

HRESULT ogre::init(int x, int y)
{
	_ogreRc = RectMake(x, y, 25, 25);
	_ogre = IMAGEMANAGER->findImage("ogre_idle");
	_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");

	_atkCoolTime = 0;
	_speed = 1;
	_hp = 50;
	_atk = 30;
	_isStart = true;
	return S_OK;
}

void ogre::update()
{
	_currentX = (_ogreRc.left + 20 + (_ogreRc.top) * 2) / 100;
	_currentY = (((_ogreRc.top - 12 + 40) / 25 - (_ogreRc.left + 14 + 25) / 50 + 1) / 2);
	_ogreColRc = RectMake(_ogreRc.left + m_camera->getCameraX() - 10, _ogreRc.top + m_camera->getCameraY() - 50, 55, 75);
	animation();
	if (!_isAtkTimeCheck)
	{
		if (!_isFindGolem)findTarget();
		if (_isFindRoad && !_isMoveEnd && _vAStar.size() > 0) ogreMove();
		else if (_vAStar.size() == 0 && !_isStart) ogreAtk();
	}
	else checkAtkCoolTime();

	if (_hp < 0)
	{
		_vAStar.clear();
		_isAtk = false;
		_isMove = false;
		_isDeath = true;
		_ogre = IMAGEMANAGER->findImage("ogre_die");
		_ogreShadow = IMAGEMANAGER->findImage("ogre_die_shadow");
	}
}

void ogre::render()
{
	if (_isDeath) _ogre->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() - 135, _ogreRc.top + m_camera->getCameraY() - 130, _deathIndex, _deathDir);
	else if(_isAtk)  _ogre->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() -90, _ogreRc.top + m_camera->getCameraY() - 160, _frameX, _ogreDirection);
	else if(_isHit)  _ogre->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() -75, _ogreRc.top + m_camera->getCameraY() - 130, _frameX, _ogreDirection);
	else  _ogre->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() -45, _ogreRc.top + m_camera->getCameraY() - 100, _frameX, _ogreDirection);
}

void ogre::shadowRender()
{
	if (_isDeath);
	else if (_isAtk)_ogreShadow->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() - 160, _ogreRc.top + m_camera->getCameraY() - 195, _frameX, _ogreDirection);
	else if(_isHit)_ogreShadow->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() - 145, _ogreRc.top + m_camera->getCameraY() - 165, _frameX, _ogreDirection);
	else _ogreShadow->frameRender(getMemDC(), _ogreRc.left + m_camera->getCameraX() - 95, _ogreRc.top + m_camera->getCameraY() - 130, _frameX, _ogreDirection);
}

void ogre::animation()
{
	if (_isAtk)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX == 13)
			{
				if (!_isFindGolem) m_player->playerHit(_atk, _ogreDirection);
				else if (_isFindGolem) _golemHit = true;
			}
			if (_frameX > _ogre->getMaxFrameX())
			{
				_isAtk = false;
				_isAtkTimeCheck = true;
				_frameCount = 0;
				_frameX = 0;
				_frameMaxCheck = false;
				_ogre = IMAGEMANAGER->findImage("ogre_idle");
				_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");
			}
		}
	}
	else if (_isMove)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _ogre->getMaxFrameX())
			{
				_frameCount = 0;
				_frameX = 0;
				_ogre = IMAGEMANAGER->findImage("ogre_idle");
				_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");
			}
		}
	}
	else if (_isDeath)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_deathIndex++;
			if (_deathIndex > _ogre->getMaxFrameX())
			{
				_deathIndex = _ogre->getMaxFrameX();
			}
		}
	}
	else if (_isHit)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _ogre->getMaxFrameX())
			{
				_isHit = false;
				_frameCount = 0;
				_frameX = 0;
				_ogre = IMAGEMANAGER->findImage("ogre_idle");
				_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");
			}
		}
	}
	else
	{
		_frameCount++;
		if (_frameCount % 5 == 0)
		{
			if (!_frameMaxCheck)_frameX++;
			else _frameX--;
			if (_frameX > _ogre->getMaxFrameX())
			{
				_frameX--;
				_frameMaxCheck = true;
				_frameCount = 0;
			}
			else if (_frameX < 1)
			{
				_frameMaxCheck = false;
			}
		}
	}

}

void ogre::findTarget()
{
	int _tempDis;
	_tempDis = abs(_currentX - m_player->getPlayerCurrentX()) + abs(_currentY - m_player->getPlayerCurrentY());

	if (_tempDis < 6 && !_isAtk)
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
		_ogre = IMAGEMANAGER->findImage("ogre_idle");
		_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");
	}
}

void ogre::ogreMove()
{
	_isStart = false;
	_ogre = IMAGEMANAGER->findImage("ogre_move");
	_ogreShadow = IMAGEMANAGER->findImage("ogre_move_shadow");

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
			if (nextPointX < _startX && nextPointY < _startY) _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _ogreDirection = OGRE_UP;
			if (nextPointX == _startX && nextPointY < _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _ogreDirection = OGRE_RIGHTUP;
			if (nextPointX > _startX && nextPointY < _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _ogreDirection = OGRE_RIGHT;
			if (nextPointX > _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _ogreDirection = OGRE_RIGHTDOWN;
			if (nextPointX > _startX && nextPointY > _startY)  _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _ogreDirection = OGRE_DOWN;
			if (nextPointX == _startX && nextPointY > _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _ogreDirection = OGRE_LEFTDOWN;
			if (nextPointX <  _startX && nextPointY  > _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _ogreDirection = OGRE_LEFT;
			if (nextPointX < _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _ogreDirection = OGRE_LEFTUP;
			_isEndPointCheck = true;
			_frameX = 0;
			_frameCount = 0;
		}
	}
	int _tempRcLeft = _ogreRc.left;
	int _tempRcTop = _ogreRc.top;
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

	_ogreRc = RectMake(_tempRcLeft, _tempRcTop, 25, 25);

	//_currentX = (_ogreRc.left + 20 + (_ogreRc.top) * 2) / 100;
	//_currentY = (((_ogreRc.top - 12 + 40) / 25 - (_ogreRc.left + 14 + 25) / 50 + 1) / 2);
}

void ogre::ogreAtk()
{
	_isMove = false;
	_isAtk = true;
	_ogre = IMAGEMANAGER->findImage("ogre_atk");
	_ogreShadow = IMAGEMANAGER->findImage("ogre_atk_shadow");

	if (_isFindGolem)
	{
		if (_endX < _currentX &&  _endY < _currentY)  _ogreDirection = OGRE_UP;
		if (_endX == _currentX && _endY < _currentY)  _ogreDirection = OGRE_RIGHTUP;
		if (_endX > _currentX &&  _endY < _currentY)  _ogreDirection = OGRE_RIGHT;
		if (_endX > _currentX &&  _endY == _currentY) _ogreDirection = OGRE_RIGHTDOWN;
		if (_endX > _currentX &&  _endY > _currentY)  _ogreDirection = OGRE_DOWN;
		if (_endX == _currentX && _endY > _currentY)  _ogreDirection = OGRE_LEFTDOWN;
		if (_endX < _currentX &&  _endY > _currentY)  _ogreDirection = OGRE_LEFT;
		if (_endX < _currentX &&  _endY == _currentY) _ogreDirection = OGRE_LEFTUP;

	}
	else
	{
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() < _currentY)  _ogreDirection =  OGRE_UP;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() < _currentY)  _ogreDirection = OGRE_RIGHTUP;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() < _currentY)  _ogreDirection =  OGRE_RIGHT;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() == _currentY) _ogreDirection =  OGRE_RIGHTDOWN;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() > _currentY)  _ogreDirection =  OGRE_DOWN;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() > _currentY)  _ogreDirection = OGRE_LEFTDOWN;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() > _currentY)  _ogreDirection =  OGRE_LEFT;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() == _currentY) _ogreDirection =  OGRE_LEFTUP;

	}
}

void ogre::checkAtkCoolTime()
{
	_atkCoolTime += TIMEMANAGER->getElapsedTime();
	if (_atkCoolTime > 3)
	{
		_atkCoolTime = 0;
		_isAtkTimeCheck = false;

	}
}

void ogre::golemDeath()
{
	_isFindGolem = false;
	_isEndPointCheck = false;
	_golemDeathCheck = false;
	//_ogre = IMAGEMANAGER->findImage("ogre_idle");
	//_ogreShadow = IMAGEMANAGER->findImage("ogre_idle_shadow");
}

void ogre::ogreHit(int damage)
{
	_frameX = 0;
	_hp = _hp - damage;
	_isHit = true;

	_ogre = IMAGEMANAGER->findImage("ogre_hit");
	_ogreShadow = IMAGEMANAGER->findImage("ogre_hit_shadow");
}


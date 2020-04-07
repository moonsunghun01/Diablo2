#include "stdafx.h"
#include "anda.h"

HRESULT anda::init(int x, int y)
{ 
	_andaRc = RectMake(x, y, 25, 25);
	_anda = IMAGEMANAGER->findImage("anda_idle");
	_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");

	_atkCoolTime = 0;
	_speed = 1;
	_hp = 500;
	_atk = 30;
	_isStart = true;
	_isDeath = false;
	return S_OK;
}

void anda::update()
{
	_currentX = (_andaRc.left + 20 + (_andaRc.top) * 2) / 100;
	_currentY = (((_andaRc.top - 12 + 40) / 25 - (_andaRc.left + 14 + 25) / 50 + 1) / 2);
	_andaColRc = RectMake(_andaRc.left + m_camera->getCameraX() - 10, _andaRc.top + m_camera->getCameraY() - 50, 55, 75);
	animation();
	if (!_isAtkTimeCheck)
	{
		if (!_isFindGolem)findTarget();
		if (_isFindRoad && !_isMoveEnd && _vAStar.size() > 0) andaMove(); 
		else if (_vAStar.size() == 0 && !_isStart && a < 3) andaAtk();
		else if (_vAStar.size() == 0 && !_isStart && a == 3 && _isActSkill ) andaSkill();
	}
	else checkAtkCoolTime();

	if (_hp < 0)
	{
		_vAStar.clear();
		_isAtk = false;
		_isMove = false;
		_isDeath = true;
		//_anda = IMAGEMANAGER->findImage("anda_die_fire");
		//_andaShadow = IMAGEMANAGER->findImage("anda_die");
		_anda = IMAGEMANAGER->findImage("anda_die");
		_andaShadow = IMAGEMANAGER->findImage("anda_die_fire");
	}	
	if (_vSkill.size() > 0) skillMove();
}

void anda::render()
{
	//if (_isDeath) _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 35, _andaRc.top + m_camera->getCameraY() - 150, _deathIndex, _deathDir);
	if (_isDeath) _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 75, _andaRc.top + m_camera->getCameraY() - 170, _deathIndex, _deathDir);
	else if (_isAtk && a == 3)  _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 85, _andaRc.top + m_camera->getCameraY() - 150, _skillIdx, _andaDirection);
	else if (_isAtk)  _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 100, _andaRc.top + m_camera->getCameraY() - 170, _atkIdx, _andaDirection);
	else if (_isHit)  _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 65, _andaRc.top + m_camera->getCameraY() - 160, _frameX, _andaDirection);
	else  _anda->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 70, _andaRc.top + m_camera->getCameraY() - 150, _frameX, _andaDirection);
	for (int i = 0; i < _vSkill.size(); i++)
	{
		_vSkill.at(i).image->frameRender(getMemDC(), _vSkill.at(i).rc.left, _vSkill.at(i).rc.top,i, _andaDirection);
	} 
}

void anda::shadowRender()
{
	//if (_isDeath)_andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 75, _andaRc.top + m_camera->getCameraY() - 170, _deathIndex, _deathDir);
	if (_isDeath)_andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 25, _andaRc.top + m_camera->getCameraY() - 150, _deathIndex, _deathDir);
	else if (_isAtk && _isActSkill )_andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 105, _andaRc.top + m_camera->getCameraY() - 155, _skillIdx, _andaDirection);
	else if (_isAtk)_andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 120, _andaRc.top + m_camera->getCameraY() - 175, _atkIdx, _andaDirection);
	else if (_isHit)_andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 80, _andaRc.top + m_camera->getCameraY() - 160, _frameX, _andaDirection);
	else _andaShadow->frameRender(getMemDC(), _andaRc.left + m_camera->getCameraX() - 85, _andaRc.top + m_camera->getCameraY() - 160, _frameX, _andaDirection);
}

void anda::animation()
{
	if (_isAtk)
	{
		_atkCount++;
		if (_atkCount % 3 == 0)
		{
			if (a == 3)
			{
				_skillIdx++;
				if (_skillIdx > _anda->getMaxFrameX())
				{
					_isAtk = false;
					_isAtkTimeCheck = true; 
					_atkIdx = 0;
					_frameMaxCheck = false;
					_anda = IMAGEMANAGER->findImage("anda_idle");
					_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");
					_skillIdx = 0; 
					a = 0;
				}
			}
			else
			{
				_atkIdx++;
				if (_atkIdx == 3)
				{
					if (!_isFindGolem) m_player->playerHit(_atk, _andaDirection);
					else if (_isFindGolem) _golemHit = true;
				}
				if (_atkIdx > _anda->getMaxFrameX())
				{
					_isAtk = false;
					_isAtkTimeCheck = true;
					_atkCount = 0;
					_atkIdx = 0;
					_frameMaxCheck = false;
					_anda = IMAGEMANAGER->findImage("anda_idle");
					_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");
					a++;
					if (a == 3) _isActSkill = true;
				}
			}
		}
	}
	else if (_isMove)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _anda->getMaxFrameX())
			{
				_frameCount = 0;
				_frameX = 0;
				_anda = IMAGEMANAGER->findImage("anda_idle");
				_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");
			}
		}
	}
	else if (_isDeath)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_deathIndex++;
			if (_deathIndex > _anda->getMaxFrameX())
			{
				_deathIndex = _anda->getMaxFrameX();
			}
		}
	}
	else if (_isHit)
	{
		_frameCount++;
		if (_frameCount % 3 == 0)
		{
			_frameX++;
			if (_frameX > _anda->getMaxFrameX())
			{
				_isHit = false;
				_frameCount = 0;
				_frameX = 0;
				_anda = IMAGEMANAGER->findImage("anda_idle");
				_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");
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
			if (_frameX > _anda->getMaxFrameX()-1)
			{ 
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

void anda::findTarget()
{
	int _tempDis;
	_tempDis = abs(_currentX - m_player->getPlayerCurrentX()) + abs(_currentY - m_player->getPlayerCurrentY());

	if (_tempDis < 6 && !_isAtk && a != 3)
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
		//_anda = IMAGEMANAGER->findImage("anda_idle");
		//_andaShadow = IMAGEMANAGER->findImage("anda_idle_shadow");
	}
}

void anda::andaMove()
{
	_isStart = false;
	_anda = IMAGEMANAGER->findImage("anda_move");
	_andaShadow = IMAGEMANAGER->findImage("anda_move_shadow");

	_isMove = true;
	_isAtk = false;
	int nextPointX = _vAStar.at(_vAStar.size() - 1) % TILEX;
	int nextPointY = _vAStar.at(_vAStar.size() - 1) / TILEX;
	 
	// ½ÃÀÛÁöÁ¡ 
	_startX = _currentX;
	_startY = _currentY;

	// µµÂø ÁöÁ¡±îÁö °Å¸®  
	_moveXDistance = nextPointX - _startX;
	_moveYDistance = nextPointY - _startY;
	if (_startX != _endX || _startY != _endY)
	{
		if (!_isEndPointCheck)
		{
			if (nextPointX < _startX && nextPointY < _startY) _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _andaDirection = ANDA_UP;
			if (nextPointX == _startX && nextPointY < _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _andaDirection = ANDA_RIGHTUP;
			if (nextPointX > _startX && nextPointY < _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _andaDirection = ANDA_RIGHT;
			if (nextPointX > _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _andaDirection = ANDA_RIGHTDOWN;
			if (nextPointX > _startX && nextPointY > _startY)  _moveYMaxCount = CELL_HEIGHT * _moveYDistance, _andaDirection = ANDA_DOWN;
			if (nextPointX == _startX && nextPointY > _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance), _andaDirection = ANDA_LEFTDOWN;
			if (nextPointX <  _startX && nextPointY  > _startY)                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _andaDirection = ANDA_LEFT;
			if (nextPointX < _startX && nextPointY == _startY) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _andaDirection = ANDA_LEFTUP;
			_isEndPointCheck = true;
			_frameX = 0;
			_frameCount = 0;
		}
	}
	int _tempRcLeft = _andaRc.left;
	int _tempRcTop = _andaRc.top;
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
		//µµÂø
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

	_andaRc = RectMake(_tempRcLeft, _tempRcTop, 25, 25);
	 
}

void anda::andaAtk()
{
	_isMove = false;
	_isAtk = true;
	_anda = IMAGEMANAGER->findImage("anda_atk");
	_andaShadow = IMAGEMANAGER->findImage("anda_atk_shadow");

	if (_isFindGolem)
	{
		if (_endX < _currentX &&  _endY < _currentY)  _andaDirection = ANDA_UP;
		if (_endX == _currentX && _endY < _currentY)  _andaDirection = ANDA_RIGHTUP;
		if (_endX > _currentX &&  _endY < _currentY)  _andaDirection = ANDA_RIGHT;
		if (_endX > _currentX &&  _endY == _currentY) _andaDirection = ANDA_RIGHTDOWN;
		if (_endX > _currentX &&  _endY > _currentY)  _andaDirection = ANDA_DOWN;
		if (_endX == _currentX && _endY > _currentY)  _andaDirection = ANDA_LEFTDOWN;
		if (_endX < _currentX &&  _endY > _currentY)  _andaDirection = ANDA_LEFT;
		if (_endX < _currentX &&  _endY == _currentY) _andaDirection = ANDA_LEFTUP;

	}
	else
	{
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() < _currentY)   _andaDirection = ANDA_UP;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() < _currentY)  _andaDirection = ANDA_RIGHTUP;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() < _currentY)   _andaDirection = ANDA_RIGHT;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() == _currentY)  _andaDirection = ANDA_RIGHTDOWN;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() > _currentY)   _andaDirection = ANDA_DOWN;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() > _currentY)  _andaDirection = ANDA_LEFTDOWN;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() > _currentY)   _andaDirection = ANDA_LEFT;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() == _currentY)  _andaDirection = ANDA_LEFTUP;
	}
}

void anda::andaSkill()
{
	_isMove = false;
	_isAtk = true;
	_anda = IMAGEMANAGER->findImage("anda_skill");
	_andaShadow = IMAGEMANAGER->findImage("anda_skill_shadow");

	if (_isFindGolem)
	{
		if (_endX < _currentX &&  _endY < _currentY)  _andaDirection = ANDA_UP;
		if (_endX == _currentX && _endY < _currentY)  _andaDirection = ANDA_RIGHTUP;
		if (_endX > _currentX &&  _endY < _currentY)  _andaDirection = ANDA_RIGHT;
		if (_endX > _currentX &&  _endY == _currentY) _andaDirection = ANDA_RIGHTDOWN;
		if (_endX > _currentX &&  _endY > _currentY)  _andaDirection = ANDA_DOWN;
		if (_endX == _currentX && _endY > _currentY)  _andaDirection = ANDA_LEFTDOWN;
		if (_endX < _currentX &&  _endY > _currentY)  _andaDirection = ANDA_LEFT;
		if (_endX < _currentX &&  _endY == _currentY) _andaDirection = ANDA_LEFTUP;

	}
	else
	{
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() < _currentY)   _andaDirection = ANDA_UP;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() < _currentY)  _andaDirection = ANDA_RIGHTUP;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() < _currentY)   _andaDirection = ANDA_RIGHT;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() == _currentY)  _andaDirection = ANDA_RIGHTDOWN;
		if (m_player->getPlayerCurrentX() > _currentX && m_player->getPlayerCurrentY() > _currentY)   _andaDirection = ANDA_DOWN;
		if (m_player->getPlayerCurrentX() == _currentX && m_player->getPlayerCurrentY() > _currentY)  _andaDirection = ANDA_LEFTDOWN;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() > _currentY)   _andaDirection = ANDA_LEFT;
		if (m_player->getPlayerCurrentX() < _currentX && m_player->getPlayerCurrentY() == _currentY)  _andaDirection = ANDA_LEFTUP;
	}
	if (_andaDirection == ANDA_DOWN)		_skillAngle = -0.75; // –A´Ù
	if (_andaDirection == ANDA_LEFTDOWN)	_skillAngle = -1.5;  // –A´Ù
	if (_andaDirection == ANDA_LEFT)		_skillAngle = -2.25; // »«´Ù
	if (_andaDirection == ANDA_LEFTUP)		_skillAngle = 3;	  //»«´Ù
	if (_andaDirection == ANDA_UP)			_skillAngle = 2.25;
	if (_andaDirection == ANDA_RIGHTUP)		_skillAngle = 1.5;
	if (_andaDirection == ANDA_RIGHT)		_skillAngle = 0.75;
	if (_andaDirection == ANDA_RIGHTDOWN)	_skillAngle = 0;
	for (int i = 0; i < 9; i++)
	{
		_bossSkill.angle = _skillAngle - (i * 0.2);

		_bossSkill.rc = RectMake(_andaColRc.left + cosf(_bossSkill.angle) * 100, _andaColRc.top - sinf(_bossSkill.angle) * 100, 50, 50);
		_bossSkill.image = IMAGEMANAGER->findImage("anda_active_skill");
		_vSkill.push_back(_bossSkill);
	}

	_isActSkill = false; 
}

void anda::skillMove()
{
	for (int i = 0; i < _vSkill.size(); i++)
	{
		_vSkill.at(i).rc = RectMake(_vSkill.at(i).rc.left + cosf(_vSkill.at(i).angle) *5, _vSkill.at(i).rc.top - sinf(_vSkill.at(i).angle ) * 5, 50, 50);
		_vSkill.at(i).currentX = ((_vSkill.at(i).rc.left + 20 - m_camera->getCameraX()) + (_vSkill.at(i).rc.top - m_camera->getCameraY()) * 2) / 100;
		_vSkill.at(i).currentY = ((_vSkill.at(i).rc.top - m_camera->getCameraY() - 12 + 40) / 25 - (_vSkill.at(i).rc.left + 14 + 25 - m_camera->getCameraX()) / 50 + 1) / 2;

	}
}

void anda::checkAtkCoolTime()
{
	_atkCoolTime += TIMEMANAGER->getElapsedTime();
	if (_atkCoolTime > 1)
	{
		_atkCoolTime = 0;
		_isAtkTimeCheck = false;

	}
}

void anda::golemDeath()
{
	_isFindGolem = false;
	_isEndPointCheck = false;
	_golemDeathCheck = false; 
}

void anda::andaHit(int damage)
{
	_frameX = 0;
	_hp = _hp - damage;
	_isHit = true;

	_anda = IMAGEMANAGER->findImage("anda_hit");
	_andaShadow = IMAGEMANAGER->findImage("anda_hit_shadow");
}


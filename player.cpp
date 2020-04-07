#include "stdafx.h"
#include "player.h"

HRESULT player::init()
{ 
	_strength = 15;
	_dex = 15;
	_life = 25;
	_mana = 25;

	_attackDamage = 20;
	_attackGrade = 20;
	_def = 300;
	_stamina = 200;

	_hp = _currentHp = _life * 20;
	_mp = _currentMp = _mana * 10;

	_money = 10000;
	_isMove = false;
	_moveSpeed = 1; 
	//½ÃÀÛ
	_playerRc = RectMake(36, 2012, 25, 25); 
	
	//_playerRc = RectMake(786 ,1737, 25, 25); 
	 
	_playerStartX = _playerCurrentX = _playerEndX = 40;
	//_playerStartX = _playerCurrentX = _playerEndX = 42;
	_playerStartY = _playerCurrentY = _playerEndY = 40;
	//_playerStartY = _playerCurrentY = _playerEndY = 27;

	_rightSkill = 0;
	_leftSkill = 0;
	_isSummon = false;
	_playerLevel = 1;
	_skillPoint = 0;
	_statusPoint = 0;

	_playerEqSd.kind = EMPTY;
	_playerEqWp.kind = EMPTY;

	_isHit = false;
	return S_OK;
}

void player::update()
{
	_hp = _life * 20;
	_mp = _mana * 10;
	_playerColRc = RectMake(_playerRc.left + m_camera->getCameraX() -5, _playerRc.top+m_camera->getCameraY()- 50, 40, 75);
}

void player::playerHit(int atk,int dir)
{
	_isHit = true;
	_hitDir = dir;
	_currentHp = _currentHp - atk;
	_playerImage = IMAGEMANAGER->findImage("necro_hit");
	_playerShadowImage = IMAGEMANAGER->findImage("necro_hit_shadow");
	_playerImage->setFrameY(dir);
}
 

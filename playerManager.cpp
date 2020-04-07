#include "stdafx.h"
#include "playerManager.h"

HRESULT playerManager::init()
{
	_playerDirection = PLAYER_DOWN;
	m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_idle"));
	m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_idle_shadow"));

	_playerSkillTree = new playerSkillTree;
	_playerSkillTree->init();

	_playerStatus = new playerStatus;
	_playerStatus->init();

	_item = new item;
	_item->init();

	_playerInventory = new playerInventory;
	_playerInventory->init(_item);
	  
	
	//990 700
	_skillPanelButton = IMAGEMANAGER->findImage("skillPanelButton");
	_skillPanelButtonRc = RectMake(996,708,40,40);

	_statusPanelButton = IMAGEMANAGER->findImage("statusPanelButton");
	_statusPanelButtonRc = RectMake(461, 708, 40, 40);

	for (int i = 0; i < 4; i++)
	{
		_potionPanelRc[i] = RectMake(783 + i*46,705,45,45);
	}

	_mana = IMAGEMANAGER->findImage("mana");
	return S_OK;
}

void playerManager::release()
{
	SAFE_DELETE(_playerSkillTree);
	SAFE_DELETE(_playerStatus);
}

void playerManager::update()
{  
	
	if (m_player->getIsMove() ) playerMove(); 
	keyManager();
	animation();

	for (int i = 0; i < _vSkill.size(); i++)
	{
		skillMove(i);
	}
	_item->update();
	_playerSkillTree->update();
	_playerStatus->update();
	_playerInventory->update(_item);

	if(_isShieldClick)_item->setShieldRc(m_ptMouse);
	if(_isWeaponClick)_item->setWeaponRc(m_ptMouse);
	if (_isPositionClick) _item->setVPotionRc(_potionNum, m_ptMouse); 
}

void playerManager::render()
{ 
	// 본쉴드 뒤에있을때
	for (int i = 0; i < _vAuraSkill.size(); i++)
	{
		if(_vAuraSkill.at(i).skillAngle == 0 )
		_vAuraSkill.at(i).skillImage->frameRender(getMemDC(), m_player->getPlayerRc().left - 50 + m_camera->getCameraX()
			, m_player->getPlayerRc().top - 45 + m_camera->getCameraY(),
			_vAuraSkill.at(i).skillAniIndex, _vAuraSkill.at(i).skillAngle);
	}
	// 이동 및 기본
	if (!_isSKillActiveMotion)
	{
		m_player->getPlayerImage()->frameRender(getMemDC(), m_player->getPlayerRc().left - 30 + m_camera->getCameraX(), m_player->getPlayerRc().top - 50 + m_camera->getCameraY());
	}
	// 스킬
	// 케릭터 모션
	if (_isSKillActiveMotion)
	{ 
		m_player->getPlayerImage()->frameRender(getMemDC(), m_player->getPlayerRc().left - 30 + m_camera->getCameraX(), m_player->getPlayerRc().top - 60 + m_camera->getCameraY());
		 
	}
	// 액티브스킬
	for (int i = 0; i < _vSkillTrail.size(); i++)
	{
		_vSkillTrail.at(i).skillImage->alphaFrameRender(getMemDC(), _vSkillTrail.at(i).skillRc.left - 20, _vSkillTrail.at(i).skillRc.top - 35,
			_vSkillTrail.at(i).skillAniIndex, _vSkillTrail.at(i).skillAngle, 150);
	}
	for (int i = 0; i < _vSkill.size(); i++)
	{
		_vSkill.at(i).skillImage->alphaFrameRender(getMemDC(), _vSkill.at(i).skillRc.left-25 
				, _vSkill.at(i).skillRc.top-25 ,
			_vSkill.at(i).skillAniIndex, _vSkill.at(i).skillAngle,150);
	}
	// 오라스킬
	for (int i = 0; i < _vAuraSkill.size(); i++)
	{
		if(_vAuraSkill.at(i).skillAngle == 1)
		_vAuraSkill.at(i).skillImage->frameRender(getMemDC(), m_player->getPlayerRc().left -50 + m_camera->getCameraX()
			, m_player->getPlayerRc().top - 45 + m_camera->getCameraY(),
			_vAuraSkill.at(i).skillAniIndex, _vAuraSkill.at(i).skillAngle);
	}
	

	RECT _tempRc = RectMake(0, 0, 0, 0);
	if (_isOpenSkillTree) _playerSkillTree->render(); 
	else _playerSkillTree->setDefaultSkillTreeRc(_tempRc);
	if (_isOpenStatus)	 _playerStatus->render();
	else _playerStatus->setPlayerStatusRc(_tempRc);
	if (_isOpenInven)	_playerInventory->render();
	else _playerInventory->setPlayerInvenRc(_tempRc);

	if (m_player->getSkillPoint() > 0) _skillPanelButton->render(getMemDC(), _skillPanelButtonRc.left, _skillPanelButtonRc.top);
	if (m_player->getStatusPoint() > 0) _statusPanelButton->render(getMemDC(), _statusPanelButtonRc.left, _statusPanelButtonRc.top);
	if (m_player->getButton1Potion().kind == POTION) m_player->getButton1Potion().itemImg->render(getMemDC(), _potionPanelRc[0].left + 5, _potionPanelRc[0].top + 5);
	if (m_player->getButton2Potion().kind == POTION) m_player->getButton2Potion().itemImg->render(getMemDC(), _potionPanelRc[1].left + 5, _potionPanelRc[1].top + 5);
	if (m_player->getButton3Potion().kind == POTION) m_player->getButton3Potion().itemImg->render(getMemDC(), _potionPanelRc[2].left + 5, _potionPanelRc[2].top + 5);
	if (m_player->getButton4Potion().kind == POTION) m_player->getButton4Potion().itemImg->render(getMemDC(), _potionPanelRc[3].left + 5, _potionPanelRc[3].top + 5);

	//sprintf_s(str, "_skillAngleValue : %lf  ", _skillAngleValue);
	//TextOut(getMemDC(), 800, 100, str, strlen(str));
	 
	// 마나 없을때
	if (_isManaLcak)
	{
		_mana->alphaRender(getMemDC(), m_player->getPlayerColRc().left - 25, m_player->getPlayerColRc().top - 30,150);
		_manaCount += TIMEMANAGER->getElapsedTime();
		if (_manaCount > 2) _isManaLcak = false , _manaCount = 0;
	}
	// 아이템 정보
	if (_isOpenInven)
	{
		for (int i = 0; i < _item->getVPotion().size(); i++)
		{
			if (PtInRect(&_item->getVPotion().at(i).itemRc, m_ptMouse) && !_isPositionClick && _item->getVPotion().at(i).mp >50)
			{
				IMAGEMANAGER->findImage("mpPotionInfo")->alphaRender(getMemDC(), _item->getVPotion().at(i).itemRc.left -46, _item->getVPotion().at(i).itemRc.top - 46, 180);
			}
			if (PtInRect(&_item->getVPotion().at(i).itemRc, m_ptMouse) && !_isPositionClick && _item->getVPotion().at(i).hp > 50)
			{
				IMAGEMANAGER->findImage("hpPotionInfo")->alphaRender(getMemDC(), _item->getVPotion().at(i).itemRc.left - 46, _item->getVPotion().at(i).itemRc.top - 46, 180);
			}
		}
		if (PtInRect(&_item->getShieldRc(), m_ptMouse) && !_isShieldClick )
		{
			IMAGEMANAGER->findImage("shieldInfo")->alphaRender(getMemDC(), _item->getShieldRc().left - 56, _item->getShieldRc().top - 150, 180);
		}
		if (PtInRect(&_item->getWeaponRc(), m_ptMouse) && !_isWeaponClick)
		{
			IMAGEMANAGER->findImage("weaponInfo")->alphaRender(getMemDC(), _item->getWeaponRc().left - 56, _item->getWeaponRc().top - 150, 180);
		}
	}

}

void playerManager::shadowRender()
{
	// 이동 및 기본
	if (!_isSKillActiveMotion)
	{
		m_player->getPlayerShadowImage()->frameRender(getMemDC(), m_player->getPlayerRc().left - 50 + m_camera->getCameraX(), m_player->getPlayerRc().top - 50 + m_camera->getCameraY());
}
	// 스킬
	// 케릭터 모션
	if (_isSKillActiveMotion)
	{
		m_player->getPlayerShadowImage()->frameRender(getMemDC(), m_player->getPlayerRc().left - 50 + m_camera->getCameraX(), m_player->getPlayerRc().top - 60 + m_camera->getCameraY());
	}
}

void playerManager::playerMove()
{
	m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_run"));
	m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_run_shadow"));
	int nextPointX = _vAStar.at(_vAStar.size()-1) % TILEX;
	int nextPointY = _vAStar.at(_vAStar.size()-1) / TILEX;

	// 시작지점 
	m_player->setPlayerStartX(m_player->getPlayerCurrentX()); 
	m_player->setPlayerStartY(m_player->getPlayerCurrentY()); 
	// 도착 지점까지 거리  
	_moveXDistance = nextPointX - m_player->getPlayerStartX();
	_moveYDistance = nextPointY - m_player->getPlayerStartY();
	if (m_player->getPlayerStartX() != m_player->getPlayerEndX() || m_player->getPlayerStartY() != m_player->getPlayerEndY())
	{
		if(!_isEndPointCheck)
		{
			if (nextPointX < m_player->getPlayerStartX() &&  nextPointY < m_player->getPlayerStartY()) _moveYMaxCount = CELL_HEIGHT * _moveYDistance , _playerDirection = PLAYER_UP;
			if (nextPointX == m_player->getPlayerStartX() && nextPointY < m_player->getPlayerStartY()) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance) , _playerDirection = PLAYER_RIGHTUP;
			if (nextPointX > m_player->getPlayerStartX() &&  nextPointY < m_player->getPlayerStartY())                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance , _playerDirection = PLAYER_RIGHT;
			if (nextPointX > m_player->getPlayerStartX() &&  nextPointY == m_player->getPlayerStartY()) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance , _playerDirection = PLAYER_RIGHTDOWN;
			if (nextPointX > m_player->getPlayerStartX() &&  nextPointY > m_player->getPlayerStartY())  _moveYMaxCount = CELL_HEIGHT * _moveYDistance , _playerDirection = PLAYER_DOWN;
			if (nextPointX == m_player->getPlayerStartX() && nextPointY > m_player->getPlayerStartY()) _moveYMaxCount = CELL_HEIGHT / 2 * _moveYDistance, _moveXMaxCount = -(CELL_WIDTH / 2 * _moveYDistance) , _playerDirection = PLAYER_LEFTDOWN;
			if (nextPointX < m_player->getPlayerStartX() &&  nextPointY > m_player->getPlayerStartY())                                                     _moveXMaxCount = CELL_WIDTH * _moveXDistance, _playerDirection = PLAYER_LEFT;
			if (nextPointX < m_player->getPlayerStartX() &&  nextPointY == m_player->getPlayerStartY()) _moveYMaxCount = CELL_HEIGHT / 2 * _moveXDistance, _moveXMaxCount = CELL_WIDTH / 2 * _moveXDistance, _playerDirection = PLAYER_LEFTUP;
			_playerDirection;
			_isEndPointCheck = true;
			_aniCount = 0;
			_aniIndex = 0;
		}
	}   
	float _tempRcLeft = m_player->getPlayerRc().left;
	float _tempRcTop = m_player->getPlayerRc().top;
	float _speedX = m_player->getMoveSpeed(); 
	float _speedY = m_player->getMoveSpeed(); 
	if (_moveXMaxCount < 0) _speedX = -m_player->getMoveSpeed(); 
	if (_moveYMaxCount < 0) _speedY = -m_player->getMoveSpeed(); 
		
	if (_moveXCount < abs(_moveXMaxCount))
	{
		_tempRcLeft += (_speedX * 2);
		_moveXCount += m_player->getMoveSpeed() * 2; 
		m_camera->setCameraX(-(_tempRcLeft + m_player->getPlayerImage()->getFrameWidth() / 2 - WINSIZEX / 2));
	}

	if (_moveYCount < abs(_moveYMaxCount))
	{
		_tempRcTop += _speedY;
		_moveYCount += m_player->getMoveSpeed(); 
		m_camera->setCameraY(-(_tempRcTop + m_player->getPlayerImage()->getFrameHeight() / 2 - WINSIZEY / 2));
	}
			
	if (_moveYCount >= abs(_moveYMaxCount) && _moveXCount >= abs(_moveXMaxCount))
	{
		//도착
		if (m_player->getPlayerCurrentX() == m_player->getPlayerEndX() && m_player->getPlayerCurrentY() == m_player->getPlayerEndY())
		{ 
			_moveYCount = _moveYMaxCount = _moveXCount = _moveXMaxCount = 0;
			_isEndPointCheck = false;
			m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_idle"));
			m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_idle_shadow"));
			m_player->setIsMove(false);
		}
		else
		{
			_moveYCount = _moveYMaxCount = _moveXCount = _moveXMaxCount = 0;
			_isEndPointCheck = false;
		}
		_vAStar.pop_back();
	}  
	m_player->setPlayerCurrentX((m_player->getPlayerRc().left + 20 +  m_player->getPlayerRc().top  *2 ) / 100 );  
	m_player->setPlayerCurrentY(((m_player->getPlayerRc().top - 12 +40 ) / 25 - (m_player->getPlayerRc().left +14 +25 ) / 50 +1)  /2 );

	m_player->setPlayerRc(RectMake(_tempRcLeft, _tempRcTop, 25, 25));
	
}

void playerManager::keyManager()
{
	if (KEYMANAGER->isOnceKeyDown(VK_RBUTTON) && !_isSKillActiveMotion && !_isEndPointCheck  )
	{
		// 스킬
		if (!PtInRect(&_playerInventory->getPlayerInvenRc(), m_ptMouse) && (m_player->getRightSkill() != 0))
		{
			m_player->setIsMove(false);
			_skillAngleValue = getAngle(m_player->getPlayerRcCenter().x + m_camera->getCameraX(), m_player->getPlayerRcCenter().y + m_camera->getCameraY(), m_ptMouse.x, m_ptMouse.y);
			if (_skillAngleValue <= -1.2 && _skillAngleValue >= -1.9) _skillAngle = SKILL_ANGLE_DOWN, _playerDirection = PLAYER_DOWN;
			if (_skillAngleValue <= -1.9 && _skillAngleValue >= -2.4) _skillAngle = SKILL_ANGLE_LEFTDOWN1, _playerDirection = PLAYER_LEFTDOWN;
			if (_skillAngleValue <= -2.4 && _skillAngleValue >= -2.8) _skillAngle = SKILL_ANGLE_LEFTDOWN2, _playerDirection = PLAYER_LEFTDOWN;
			if (_skillAngleValue <= -2.8 && _skillAngleValue >= -3.0) _skillAngle = SKILL_ANGLE_LEFTDOWN3, _playerDirection = PLAYER_LEFTDOWN;
			if (_skillAngleValue <= -3.0 || _skillAngleValue >= 3.0) _skillAngle = SKILL_ANGLE_LEFT, _playerDirection = PLAYER_LEFT;
			if (_skillAngleValue <= 3.0 && _skillAngleValue >= 2.7) _skillAngle = SKILL_ANGLE_LEFTUP1, _playerDirection = PLAYER_LEFTUP;
			if (_skillAngleValue <= 2.7 && _skillAngleValue >= 2.3) _skillAngle = SKILL_ANGLE_LEFTUP2, _playerDirection = PLAYER_LEFTUP;
			if (_skillAngleValue <= 2.3 && _skillAngleValue >= 1.8) _skillAngle = SKILL_ANGLE_LEFTUP3, _playerDirection = PLAYER_LEFTUP;
			if (_skillAngleValue <= 1.8 && _skillAngleValue >= 1.1) _skillAngle = SKILL_ANGLE_UP, _playerDirection = PLAYER_UP;
			if (_skillAngleValue <= 1.1 && _skillAngleValue >= 0.5) _skillAngle = SKILL_ANGLE_RIGHTUP1, _playerDirection = PLAYER_RIGHTUP;
			if (_skillAngleValue <= 0.5 && _skillAngleValue >= 0.3) _skillAngle = SKILL_ANGLE_RIGHTUP2, _playerDirection = PLAYER_RIGHTUP;
			if (_skillAngleValue <= 0.3 && _skillAngleValue >= 0.0) _skillAngle = SKILL_ANGLE_RIGHTUP3, _playerDirection = PLAYER_RIGHTUP;
			if (_skillAngleValue <= 0.0 && _skillAngleValue >= -0.2) _skillAngle = SKILL_ANGLE_RIGHT, _playerDirection = PLAYER_RIGHT;
			if (_skillAngleValue <= -0.2 && _skillAngleValue >= -0.4) _skillAngle = SKILL_ANGLE_RIGHTDOWN1, _playerDirection = PLAYER_RIGHTDOWN;
			if (_skillAngleValue <= -0.4 && _skillAngleValue >= -0.7) _skillAngle = SKILL_ANGLE_RIGHTDOWN2, _playerDirection = PLAYER_RIGHTDOWN;
			if (_skillAngleValue <= -0.7 && _skillAngleValue >= -1.2) _skillAngle = SKILL_ANGLE_RIGHTDOWN3, _playerDirection = PLAYER_RIGHTDOWN;

			_isSKillActiveMotion = true;
			m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_skill"));
			m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_skill_shadow"));
			// 본쉴드
			if (m_player->getRightSkill() == 1 && m_player->getCurrentMp() > 30)
			{
				_vAuraSkill.clear();
				_tagSkill.skillTime = 0;
				_tagSkill.skillAngle = 0;
				_tagSkill.skillImage = IMAGEMANAGER->findImage("bone_shield");
				_vAuraSkill.push_back(_tagSkill);
				m_player->setDef(m_player->getDef() + 50);
				if (m_player->getDef() > 350) m_player->setDef(350);
				m_player->setCurrentMp(m_player->getCurrentMp() - 30);
			}
			// 본스피어
			else if (m_player->getRightSkill() == 2 && m_player->getCurrentMp() > 10)
			{
				int _tempX = cosf(_skillAngleValue) * 50 + m_player->getPlayerRcCenter().x - 20;
				int _tempY = -sinf(_skillAngleValue) * 50 + m_player->getPlayerRcCenter().y - 30;
				_tagSkill.skillRc = RectMake(_tempX + m_camera->getCameraX(), _tempY + m_camera->getCameraY(), 50, 50);
				_tagSkill.skillAnlgeValue = _skillAngleValue;
				_tagSkill.skillAngle = _skillAngle;
				_tagSkill.skillImage = IMAGEMANAGER->findImage("bone_spear");
				_vSkill.push_back(_tagSkill);
				m_player->setCurrentMp(m_player->getCurrentMp() - 10);
			}
			// 파이어 골렘
			else if (m_player->getRightSkill() == 4 && m_player->getCurrentMp() > 10)
			{ 
				m_player->setIsSummon(true);
				m_player->setIsSummonAlive(false);
				m_player->setSummonDir(_playerDirection);
				m_player->setCurrentMp(m_player->getCurrentMp() - 10);
			} 
			else
			{ 
				_isSKillActiveMotion = false;
				m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_idle"));
				m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_idle_shadow"));
				_isManaLcak = true;
			}
		}
		// 물약 먹기
		else 
		{
			for (int i = 0; i < _item->getVPotion().size(); i++)
			{
				if (PtInRect(&_item->getVPotion().at(i).itemRc, m_ptMouse))
				{
					_playerInventory->setTagInvenIsExist(_item->getVPotion().at(i).index);
					m_player->setCurrentHp(m_player->getCurrentHp() + _item->getVPotion().at(i).hp);
					if (m_player->getCurrentHp() > m_player->getHp()) m_player->setCurrentHp(m_player->getHp());

					m_player->setCurrentMp(m_player->getCurrentMp() + _item->getVPotion().at(i).mp);
					if (m_player->getCurrentMp() > m_player->getMp()) m_player->setCurrentMp(m_player->getMp());
					_item->deleteItem(i); 
				}
			}
		}
	}
	
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		// Panel 스킬 
		if (PtInRect(&_skillPanelButtonRc, m_ptMouse) && m_player->getSkillPoint() > 0 )
		{
			_isOpenSkillTree = true;
		}
		// Panel 스텟
		if (PtInRect(&_statusPanelButtonRc, m_ptMouse) && m_player->getStatusPoint() > 0)
		{
			_isOpenStatus = true;
		}

		// 스텟창이 열려있을경우 
		if (_isOpenStatus)
		{
			if (PtInRect(&_playerStatus->getStatusCloseButtonRc(), m_ptMouse))
			{
				_isOpenStatus = false;
			}
			if (m_player->getStatusPoint() > 0)
			{
				for (int i = 0; i < 4; i++)
				{
					if (PtInRect(&_playerStatus->getStatusUpRc(i), m_ptMouse) && !_isClickStatusUpButton)
					{
						_isClickStatusUpButton = true;
						if (i == 0) m_player->setStrength(m_player->getStrength() + 1);
						if (i == 1) m_player->setDex(m_player->getDex() + 1);
						if (i == 2) m_player->setLife(m_player->getLife() + 1);
						if (i == 3) m_player->setMana(m_player->getMana() + 1);
						m_player->setStatusPoint(m_player->getStatusPoint() - 1);
					}
				}
			}
		} 
		// 스킬창 열려있을 경우 
		if (_isOpenSkillTree)
		{
			// 닫기 버튼
			if (PtInRect(&_playerSkillTree->getSkillTreeCloseButton(), m_ptMouse))
			{
				_isOpenSkillTree = false;
			}
			for (int i = 0; i < 3; i++)
			{
				if (PtInRect(&_playerSkillTree->getSkillTreeNumRc(i), m_ptMouse))
				{
					_playerSkillTree->setSkillTreeNum(i);
				}
			}

			// 스킬 찍기
			for (int i = 0; i < 10; i++)
			{
				if (PtInRect(&_playerSkillTree->getBoneSkillTreeIconRc(i), m_ptMouse) && m_player->getSkillPoint() > 0
					&& !_isClickSkillTreeIcon && _playerSkillTree->getBoneSkillTreeIconSkillLevel(i) < 20
					&& _playerSkillTree->getBoneSkillTreeIconLearnLevel(i) <= m_player->getPlayerLevel()
					&& _playerSkillTree->getBoneSkillTreeIconSkillLevel(_playerSkillTree->getBoneSkillTreeIconPreSkillNum(i)) > 0)
				{
					_isClickSkillTreeIcon = true;
					_playerSkillTree->setBoneSkillTreeIconSkillLevel(i, _playerSkillTree->getBoneSkillTreeIconSkillLevel(i) + 1);
					m_player->setSkillPoint(m_player->getSkillPoint() - 1);
				}
			}
		}

		// 인벤토리
		if (_isOpenInven)
		{ 
			// 닫기
			if (PtInRect(&_playerInventory->getPlayerInvenCloseButtonRc(), m_ptMouse))
			{
				_isOpenInven = false;
			}
			// 방패
			if (_isShieldClick)
			{ 
				// 방패 장착
				if (PtInRect(&_playerInventory->getShieldRc(), m_ptMouse))
				{
					m_player->setPlayerEqSd(_item->getShield());
					_item->setShieldIndex(40);
					_isShieldClick = false;
					_isShieldChange = true;

					m_player->setAttackDamage(m_player->getAttackDamage() + _item->getShield().atk);
					m_player->setDef(m_player->getDef() + _item->getShield().def);
					m_player->setStrength(m_player->getStrength() + _item->getShield().str);
					m_player->setDex(m_player->getDex() + _item->getShield().dex);
					m_player->setLife(m_player->getLife() + _item->getShield().life);
					m_player->setMana(m_player->getMana() + _item->getShield().mana);
				}
				// 인벤토리에서 아이템 이동 
				for (int i = 0; i < 40; i++)
				{
					if (PtInRect(&_playerInventory->getTagInvenRc(i), m_ptMouse))
					{
						if (i < 20 && i % 10 != 9 && !_playerInventory->getTagInvenIsExist(i) && !_playerInventory->getTagInvenIsExist(i+1)
							&& !_playerInventory->getTagInvenIsExist(i+10) && !_playerInventory->getTagInvenIsExist(i + 11)
							&& !_playerInventory->getTagInvenIsExist(i + 20) && !_playerInventory->getTagInvenIsExist(i + 21))
						{
							_item->setShieldIndex(i);
							_isShieldClick = false;
							_isShieldChange = true;
							_playerInventory->setTagInvenIsExist(i);
							_playerInventory->setTagInvenIsExist(i+1);
							_playerInventory->setTagInvenIsExist(i+10);
							_playerInventory->setTagInvenIsExist(i+11);
							_playerInventory->setTagInvenIsExist(i+20);
							_playerInventory->setTagInvenIsExist(i+21);
						}
					}
				}
			}

			//방패 옮기려고 클릭 시 
			if (PtInRect(&(_item->getShieldRc()), m_ptMouse) && !_isShieldClick && !_isShieldChange && 
				!_isPositionClick && !_isWeaponClick && m_player->getPlayerEqSd().kind == EMPTY)
			{
				_isShieldClick = true;
				_playerInventory->setTagInvenIsExist(_item->getShield().index);
				_playerInventory->setTagInvenIsExist(_item->getShield().index + 1);
				_playerInventory->setTagInvenIsExist(_item->getShield().index + 10);
				_playerInventory->setTagInvenIsExist(_item->getShield().index + 11);
				_playerInventory->setTagInvenIsExist(_item->getShield().index + 20);
				_playerInventory->setTagInvenIsExist(_item->getShield().index + 21); 
			} 
			// 방패 벗을 경우 
			if (PtInRect(&_playerInventory->getShieldRc(), m_ptMouse) && !_isShieldClick && !_isShieldChange 
				&& !_isPositionClick && !_isWeaponClick)
			{
				_isShieldClick = true;
				tagItem _tempItem;
				_tempItem.kind = EMPTY;
				m_player->setPlayerEqSd(_tempItem);
				m_player->setAttackDamage(m_player->getAttackDamage() - _item->getShield().atk);
				m_player->setDef(m_player->getDef() - _item->getShield().def);
				m_player->setStrength(m_player->getStrength() - _item->getShield().str);
				m_player->setDex(m_player->getDex() - _item->getShield().dex);
				m_player->setLife(m_player->getLife() - _item->getShield().life);
				m_player->setMana(m_player->getMana() - _item->getShield().mana);
			}
			_isShieldChange = false;

			// 무기
			if (_isWeaponClick)
			{
				// 무기 장착
				if (PtInRect(&_playerInventory->getWeaponRc(), m_ptMouse))
				{
					m_player->setPlayerEqWp(_item->getWeapon());
					_item->setWeaponIndex(40);
					_isWeaponClick = false;
					_isWeaponChange = true;

					m_player->setAttackDamage(m_player->getAttackDamage() + _item->getWeapon().atk);
					m_player->setDef(m_player->getDef() + _item->getWeapon().def);
					m_player->setStrength(m_player->getStrength() + _item->getWeapon().str);
					m_player->setDex(m_player->getDex() + _item->getWeapon().dex);
					m_player->setLife(m_player->getLife() + _item->getWeapon().life);
					m_player->setMana(m_player->getMana() + _item->getWeapon().mana);
				}
				// 인벤토리에서 아이템 이동 
				for (int i = 0; i < 40; i++)
				{
					if (PtInRect(&_playerInventory->getTagInvenRc(i), m_ptMouse))
					{
						if (i < 30 && !_playerInventory->getTagInvenIsExist(i) && !_playerInventory->getTagInvenIsExist(i + 10) )
						{
							_item->setWeaponIndex(i);
							_isWeaponClick = false;
							_isWeaponChange = true;
							_playerInventory->setTagInvenIsExist(i);
							_playerInventory->setTagInvenIsExist(i + 10);
						}
					}
				}
			}

			//무기 옮기려고 클릭 시 
			if (PtInRect(&_item->getWeaponRc(), m_ptMouse) && !_isWeaponClick && !_isWeaponChange && 
				!_isPositionClick && !_isShieldClick && m_player->getPlayerEqWp().kind == EMPTY)
			{
				_isWeaponClick = true;
				_playerInventory->setTagInvenIsExist(_item->getWeapon().index); 
				_playerInventory->setTagInvenIsExist(_item->getWeapon().index + 10); 
			}
			// 무기 벗을 경우 
			if (PtInRect(&_playerInventory->getWeaponRc(), m_ptMouse) && !_isWeaponClick && !_isWeaponChange && 
				!_isPositionClick && !_isShieldClick)
			{
				_isWeaponClick = true;
				tagItem _tempItem;
				_tempItem.kind = EMPTY;
				m_player->setPlayerEqWp(_tempItem);
				m_player->setAttackDamage(m_player->getAttackDamage() - _item->getWeapon().atk);
				m_player->setDef(m_player->getDef() - _item->getWeapon().def);
				m_player->setStrength(m_player->getStrength() - _item->getWeapon().str);
				m_player->setDex(m_player->getDex() - _item->getWeapon().dex);
				m_player->setLife(m_player->getLife() - _item->getWeapon().life);
				m_player->setMana(m_player->getMana() - _item->getWeapon().mana);
			}
			_isWeaponChange = false;

			// 물약
			if (_isPositionClick)
			{
				// 물약 이동
				for (int i = 0; i < 40; i++)
				{
					if (PtInRect(&_playerInventory->getTagInvenRc(i), m_ptMouse))
					{
						if (!_playerInventory->getTagInvenIsExist(i))
						{
							_item->setVPotionIndex(_potionNum,i);
							_isPositionClick = false;
							_isPositionChange = true;
							_playerInventory->setTagInvenIsExist(i); 
						}
					}
				}
				// 물약 패널에 장착
				for (int i = 0; i < 4; i++)
				{
					if (PtInRect(&_potionPanelRc[i], m_ptMouse))
					{
						if (i == 0) m_player->setButton1Potion(_item->getVPotion().at(_potionNum));
						if (i == 1) m_player->setButton2Potion(_item->getVPotion().at(_potionNum));
						if (i == 2) m_player->setButton3Potion(_item->getVPotion().at(_potionNum));
						if (i == 3) m_player->setButton4Potion(_item->getVPotion().at(_potionNum));
						_isPositionClick = false;
						_item->deleteItem(_potionNum);
						
					}
				}
			}
			for (int i = 0; i < _item->getVPotion().size(); i++)
			{
				if (PtInRect(&_item->getVPotion().at(i).itemRc, m_ptMouse) && !_isPositionClick && !_isPositionChange &&
					!_isShieldClick && !_isWeaponClick)
				{
					_potionNum = i;
					_isPositionClick = true;
					_playerInventory->setTagInvenIsExist(_item->getVPotion().at(_potionNum).index);
				}
			}
			_isPositionChange = false;
		}
	}
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		_isClickStatusUpButton = false;
		_isClickSkillTreeIcon = false; 
	}
	if (KEYMANAGER->isOnceKeyDown('T'))
	{
		_isOpenSkillTree = !_isOpenSkillTree;
		_isOpenInven = false;
	}
	if (KEYMANAGER->isOnceKeyDown('C'))
	{
		_isOpenStatus = !_isOpenStatus;
	}
	if (KEYMANAGER->isOnceKeyDown('I'))
	{
		_isOpenInven = !_isOpenInven;
		_isOpenSkillTree = false;
		if (!_isOpenInven) _isPositionClick = false, _isShieldClick = false;
	}
	if (KEYMANAGER->isOnceKeyDown('P'))
	{
		m_player->setPlayerLevel(m_player->getPlayerLevel()+1);
		m_player->setSkillPoint(m_player->getSkillPoint()+1);
		m_player->setStatusPoint(m_player->getStatusPoint()+5);
		m_player->setCurrentHp(m_player->getHp());
		m_player->setCurrentMp(m_player->getMp());
		m_player->setMoney(m_player->getMoney() + 5000);
	} 
	if (KEYMANAGER->isOnceKeyDown('L'))
	{
		m_player->setCurrentHp(m_player->getCurrentHp() - 5);
		m_player->setCurrentMp(m_player->getCurrentMp() - 5);
	}
	if (KEYMANAGER->isOnceKeyDown('1') && m_player->getButton1Potion().kind == POTION)
	{
		tagItem _tempPotion;
		_tempPotion.kind = EMPTY;

		m_player->setCurrentHp(m_player->getCurrentHp() + m_player->getButton1Potion().hp);
		if (m_player->getCurrentHp() > m_player->getHp()) m_player->setCurrentHp(m_player->getHp());

		m_player->setCurrentMp(m_player->getCurrentMp() + m_player->getButton1Potion().mp);
		if (m_player->getCurrentMp() > m_player->getMp()) m_player->setCurrentMp(m_player->getMp());
		m_player->setButton1Potion(_tempPotion);
	}
	if (KEYMANAGER->isOnceKeyDown('2') && m_player->getButton2Potion().kind == POTION)
	{
		tagItem _tempPotion;
		_tempPotion.kind = EMPTY;

		m_player->setCurrentHp(m_player->getCurrentHp() + m_player->getButton2Potion().hp);
		if (m_player->getCurrentHp() > m_player->getHp()) m_player->setCurrentHp(m_player->getHp());

		m_player->setCurrentMp(m_player->getCurrentMp() + m_player->getButton2Potion().mp);
		if (m_player->getCurrentMp() > m_player->getMp()) m_player->setCurrentMp(m_player->getMp());
		m_player->setButton2Potion(_tempPotion);
	}
	if (KEYMANAGER->isOnceKeyDown('3') && m_player->getButton3Potion().kind == POTION)
	{
		tagItem _tempPotion;
		_tempPotion.kind = EMPTY;

		m_player->setCurrentHp(m_player->getCurrentHp() + m_player->getButton3Potion().hp);
		if (m_player->getCurrentHp() > m_player->getHp()) m_player->setCurrentHp(m_player->getHp());

		m_player->setCurrentMp(m_player->getCurrentMp() + m_player->getButton3Potion().mp);
		if (m_player->getCurrentMp() > m_player->getMp()) m_player->setCurrentMp(m_player->getMp());
		m_player->setButton3Potion(_tempPotion);
	}
	if (KEYMANAGER->isOnceKeyDown('4') && m_player->getButton4Potion().kind == POTION)
	{
		tagItem _tempPotion;
		_tempPotion.kind = EMPTY;

		m_player->setCurrentHp(m_player->getCurrentHp() + m_player->getButton4Potion().hp);
		if (m_player->getCurrentHp() > m_player->getHp()) m_player->setCurrentHp(m_player->getHp());

		m_player->setCurrentMp(m_player->getCurrentMp() + m_player->getButton4Potion().mp);
		if (m_player->getCurrentMp() > m_player->getMp()) m_player->setCurrentMp(m_player->getMp());
		m_player->setButton4Potion(_tempPotion);
	}
}

void playerManager::animation()
{
	// 이동 및 기본 행동
	if (!_isSKillActiveMotion && !m_player->getIsHit())
	{ 
		m_player->getPlayerImage()->setFrameY(_playerDirection);
		m_player->getPlayerShadowImage()->setFrameY(_playerDirection); 
		_aniCount++;

		if (_aniCount % 5 == 0)
		{
			_aniCount = 0;
			_aniIndex++;
			if (_aniIndex >= m_player->getPlayerImage()->getMaxFrameX())
			{
				_aniIndex = 0;
			}
			m_player->getPlayerImage()->setFrameX(_aniIndex);
			m_player->getPlayerShadowImage()->setFrameX(_aniIndex);
		}
	}
	// 스킬 사용시 케릭터 모션 
	else if (_isSKillActiveMotion)
	{
		m_player->getPlayerImage()->setFrameY(_skillAngle);
		m_player->getPlayerShadowImage()->setFrameY(_skillAngle);
		_aniCount++; 
		if (_aniCount % 3 == 0)
		{
			_aniCount = 0;
			m_player->getPlayerImage()->setFrameX(_aniIndex);
			m_player->getPlayerShadowImage()->setFrameX(_aniIndex);
			_aniIndex++;
			if (_aniIndex >= m_player->getPlayerImage()->getMaxFrameX())
			{
				_isSKillActiveMotion = false;
				m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_idle"));
				m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_idle_shadow"));
			} 
		}
	}
	// 피격시
	else if (m_player->getIsHit())
	{ 
		_aniCount++;
		if (_aniCount % 3 == 0)
		{
			_aniCount = 0;
			m_player->getPlayerImage()->setFrameX(_hitIndex);
			m_player->getPlayerShadowImage()->setFrameX(_hitIndex);
			_hitIndex++;
			if (_hitIndex >= m_player->getPlayerImage()->getMaxFrameX())
			{ 
				_hitIndex = 0;
				m_player->setIsHit(false);
				m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_idle"));
				m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_idle_shadow"));
			}
		}
	}
	// 스킬 모션 액티브
	for (int i = 0; i < _vSkill.size(); i++)
	{
		_vSkill.at(i).skillAniCount++;
		if (_vSkill.at(i).skillAniCount % 5 == 0)
		{
			_vSkill.at(i).skillAniCount = 0;
			_vSkill.at(i).skillAniIndex++;
			if (_vSkill.at(i).skillAniIndex >= _vSkill.at(i).skillImage->getMaxFrameX())
			{
				_vSkill.at(i).skillAniIndex = 0;
			}
			tagSkill _tempSKill;
			_tempSKill.skillAnlgeValue = _skillAngleValue;
			_tempSKill.skillAngle = _vSkill.at(i).skillAngle;
			_tempSKill.skillImage = IMAGEMANAGER->findImage("bone_trail");
			_tempSKill.skillRc = RectMake(_vSkill.at(i).skillRc.left, _vSkill.at(i).skillRc.top, 50,50);
			_vSkillTrail.push_back(_tempSKill);
		}
	}

	for (int i = 0; i < _vSkillTrail.size(); i++)
	{
		_vSkillTrail.at(i).skillAniCount++;
		if (_vSkillTrail.at(i).skillAniCount % 5 == 0)
		{
			_vSkillTrail.at(i).skillAniCount = 0;
			_vSkillTrail.at(i).skillAniIndex++;
			if (_vSkillTrail.at(i).skillAniIndex >= _vSkillTrail.at(i).skillImage->getMaxFrameX())
			{
				_vSkillTrail.erase(_vSkillTrail.begin() + i);
			} 
		}
	}

	// 본쉴드
	for (int i = 0; i < _vAuraSkill.size(); i++)
	{
		_vAuraSkill.at(i).skillTime += TIMEMANAGER->getElapsedTime() ;
		_vAuraSkill.at(i).skillAniCount++;
		if (_vAuraSkill.at(i).skillAniCount % 3 == 0)
		{
			_vAuraSkill.at(i).skillAniCount = 0;
			_vAuraSkill.at(i).skillAniIndex++;
			if (_vAuraSkill.at(i).skillAniIndex >= _vAuraSkill.at(i).skillImage->getMaxFrameX())
			{
				if (_vAuraSkill.at(i).skillAngle == 0) _vAuraSkill.at(i).skillAngle = 1;
				else if (_vAuraSkill.at(i).skillAngle == 1) _vAuraSkill.at(i).skillAngle = 0;
				_vAuraSkill.at(i).skillAniIndex = 0;
				
			} 
		}
		if (_vAuraSkill.at(i).skillTime > 5)
		{
			_vAuraSkill.clear();
			m_player->setDef(m_player->getDef() - 50);
		}
	}
	
}

void playerManager::skillMove(int skillNum)
{
	RECT _tempRc = _vSkill.at(skillNum).skillRc;
	float _tempAngle = _vSkill.at(skillNum).skillAnlgeValue;


	_vSkill.at(skillNum).skillRc = RectMake(_tempRc.left + +cosf(_tempAngle) * 5, _tempRc.top + -sinf(_tempAngle) * 5, 50, 50);
	//_vSkill.at(skillNum).skillColRc = RectMake(_vSkill.at(skillNum).skillRc.left + m_camera->getCameraX(), _vSkill.at(skillNum).skillRc.top + m_camera->getCameraY(), 50, 50);

	_vSkill.at(skillNum).currentX = ( (_vSkill.at(skillNum).skillRc.left + 20 - m_camera->getCameraX() )+ (_vSkill.at(skillNum).skillRc.top - m_camera->getCameraY()) * 2) / 100; 
	_vSkill.at(skillNum).currentY = ((_vSkill.at(skillNum).skillRc.top - m_camera->getCameraY()  - 12 + 40) / 25 - (_vSkill.at(skillNum).skillRc.left + 14 + 25 - m_camera->getCameraX() ) / 50 + 1) / 2;
	
}

void playerManager::buyWeapon()
{
	for (int i = 0; i < 31; i++)
	{
		if (!_playerInventory->getTagInvenIsExist(i) && !_playerInventory->getTagInvenIsExist(i + 10)
			&& m_player->getMoney() >= _item->getShopWeapon().cost)
		{
			_item->setWeaponIndex(i); 
			_playerInventory->setTagInvenIsExist(i);
			_playerInventory->setTagInvenIsExist(i+10);
			_isBuyWeapon = true;

			m_player->setMoney(m_player->getMoney() - _item->getShopWeapon().cost);
			break;
		}
	}
}

void playerManager::buyShield()
{
	for (int i = 0; i < 21; i++)
	{
		if ( !_playerInventory->getTagInvenIsExist(i) && !_playerInventory->getTagInvenIsExist(i + 1) 
			&& !_playerInventory->getTagInvenIsExist(i+10) && !_playerInventory->getTagInvenIsExist(i + 11)
			&& !_playerInventory->getTagInvenIsExist(i +20) && !_playerInventory->getTagInvenIsExist(i + 21)
			&& m_player->getMoney() >= _item->getShopShield().cost)
		{
			_item->setShieldIndex(i); 
			_playerInventory->setTagInvenIsExist(i);
			_playerInventory->setTagInvenIsExist(i+1);
			_playerInventory->setTagInvenIsExist(i+10);
			_playerInventory->setTagInvenIsExist(i+11);
			_playerInventory->setTagInvenIsExist(i+20);
			_playerInventory->setTagInvenIsExist(i+21);
			_isBuyShield = true;

			m_player->setMoney(m_player->getMoney() - _item->getShopShield().cost);
			break;
		}
	}
}
void playerManager::buyHpPotiion()
{
	for (int i = 0; i < 41; i++)
	{
		if (!_playerInventory->getTagInvenIsExist(i) && m_player->getMoney() >= _item->getShopHpPoton().cost)
		{
			tagItem _tempItem = _item->getShopHpPotion();
			_tempItem.index = i;
			_playerInventory->setTagInvenIsExist(i); 
			vector<tagItem> _tempVItem = _item->getVPotion();
			_tempVItem.push_back(_tempItem);
			_item->setVPotion(_tempVItem);
			m_player->setMoney(m_player->getMoney() - _item->getShopHpPoton().cost);
			break;
		}
	}
}


void playerManager::buyMpPotion()
{
	for (int i = 0; i < 41; i++)
	{
		if (!_playerInventory->getTagInvenIsExist(i) && m_player->getMoney() >= _item->getShopMpPoton().cost)
		{
			tagItem _tempItem = _item->getShopMpPotion();
			_tempItem.index = i;
			_playerInventory->setTagInvenIsExist(i);
			vector<tagItem> _tempVItem = _item->getVPotion();
			_tempVItem.push_back(_tempItem);
			_item->setVPotion(_tempVItem);
			m_player->setMoney(m_player->getMoney() - _item->getShopMpPoton().cost);
			break;
		}
	}
}



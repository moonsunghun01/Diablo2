#include "stdafx.h"
#include "campScene.h"

 

HRESULT campScene::init()
{
	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));
	 

	_startX = m_camera->getCameraX();
	_startY = m_camera->getCameraY();
	_isDebug = false;

	TlieInit();

	_playerManager = new playerManager;
	_playerManager->init();
	_moveEndPoint.x = m_player->getPlayerStartX();
	_moveEndPoint.y = m_player->getPlayerStartY();

	_golem = new golem;
	_golem->init();

	ogre* _ogre = new ogre;
	_ogre->init(0, 0);
	_vOgre.push_back(_ogre);

	balog* _balog = new balog;
	_balog->init(0, 0);
	_vBalog.push_back(_balog);

	_anda = new anda;
	_anda->init(0, 0);

	_akara = new akara;
	_akara->init();

	_shop = new shop;
	_shop->init();

	_panel = IMAGEMANAGER->findImage("panel");
	_panelRc = RectMake(396, 690, 700, 100);
	_lifegauge = IMAGEMANAGER->findImage("lifegauge");
	_managauge = IMAGEMANAGER->findImage("managauge");
	_leftSkill = IMAGEMANAGER->findImage("skill_panel");
	_rightSkill = IMAGEMANAGER->findImage("skill_panel");
	_golemIcon = IMAGEMANAGER->findImage("fire_golem_icon");
	_golemHp = IMAGEMANAGER->findImage("fire_golem_hp");

	for (int i = 0; i < 5; i++) _rightSkillList[i] = IMAGEMANAGER->findImage("skill_panel");

	_leftSkillRc = RectMake(325, 693, _leftSkill->getFrameWidth(), _leftSkill->getFrameHeight());
	_rightSkillRc = RectMake(1103, 693, _rightSkill->getFrameWidth(), _rightSkill->getFrameHeight());

	for (int i = 0; i < 5; i++) _rightSkillListRc[i] = RectMake(830 + i * _rightSkill->getFrameWidth(), 580, _rightSkill->getFrameWidth(), _rightSkill->getFrameHeight());
	 
	isFind = false;
	noPath = false;
	startAstar = false;

	_potal = IMAGEMANAGER->findImage("way");
	_potalRc = RectMake(200, 100, _potal->getWidth(), _potal->getHeight());

	return S_OK;
}

void campScene::release()
{
	SAFE_DELETE(_playerManager);
	SAFE_DELETE(_golem);
}

void campScene::update()
{
	_wayRc = RectMake(476 + m_camera->getCameraX(), 1657 + m_camera->getCameraY(), 150, 75);
	if (_isOpenWay) _potalButtonRc = RectMake(220, 500, 380, 50), _potalCloseRc =RectMake(540,550,40,40);
	else if (!_isOpenWay) _potalButtonRc = RectMake(0, 0, 0, 0), _potalCloseRc = RectMake(0, 0, 0, 0);

	_startX = m_camera->getCameraX();
	_startY = m_camera->getCameraY();
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		_golem->setHp(_golem->getHp() - 5);
	}
	if (KEYMANAGER->isOnceKeyDown(VK_TAB))
	{
		_isDebug = !_isDebug;
	}
	if (!PtInRect(&_playerManager->getPlayerSkillTree()->getDefaultSkillTreeRc(), m_ptMouse)
		&& !PtInRect(&_playerManager->getPlayerStatus()->getPlayerStatusRc(), m_ptMouse)
		&& !PtInRect(&_panelRc, m_ptMouse) && !PtInRect(&_playerManager->getPlayerInventory()->getPlayerInvenRc(), m_ptMouse))
	{
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			float cellX = (float)(m_ptMouse.x - _startX);

			if (cellX < 0)
			{
				cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
			}
			else
			{
				//얼만큼 떨어져서 클릭했는가를 수치화(가로지름 기준)
				cellX = cellX / (float)CELL_WIDTH;
			}

			//cellY는 -개념이 없기 때문에(기준점이 가장 위)절대값으로 구하자
			int cellY = abs(m_ptMouse.y - _startY) / CELL_HEIGHT;
			//만약 -시작점 자ㅏ체가 -이면 -1을 곱해주자
			cellY = (m_ptMouse.y < _startY) ? cellY * -1 : cellY;
			//x는 y가 기준점에 가까워 질수록 작아져야 한다.
			//마름모 이기 때문에 x축은 y의기준값으로부터 멀리 떨어진수록 x값은 커진다.
			int isoX = (int)cellX + (int)cellY;
			//y축은 x기준값으로 부터 멀리 떨어질수록 y는 늘어나게 된다.
			int isoY = (int)cellY - (int)cellX;
			/*
			//만약 x좌표가 0보다 크고 최대타일수 보다 작고
			//Y좌표 0보다 크고, 최대타일수보다 작으면 검출대상에 집어넣자
			*/
			if (isoX >= 0 && isoX < TILE_COUNT_X &&isoY >= 0 && isoY < TILE_COUNT_Y)
			{ 
				int corner = GetCornerIndex(isoX, isoY); 
				if (IsInRhombus(corner, isoX, isoY))corner = 0; 
				switch (corner)
				{
				case 1:
					isoX = isoX - 1;//마름모의 4변중에 좌측 윗변과 맞닿아 있는 마름모는 x축으로 한단계 나ㅏㅈ은 좌표임.
					break;
				case 2:
					isoY = isoY - 1;
					break;
				case 3:
					isoY = isoY + 1;
					break;
				case 4:
					isoX = isoX + 1;
					break;
				}

				_center = corner;
				_isoX = isoX;
				_isoY = isoY;
			}

			if (_isRightSkillCheck || _isLeftSkillCheck)
			{
				for (int i = 0; i < 5; i++)
				{
					if (PtInRect(&_rightSkillListRc[i], m_ptMouse))
					{
						m_player->setRightSkill(i);
						_isRightSkillCheck = false;
						_isLeftSkillCheck = false;
					}
				}
			}

			// 아이템 구매
			else if (_isShopOpen && PtInRect(&_shop->getShieldRc(), m_ptMouse) )
			{
				_playerManager->buyShield();
				if (_playerManager->getIsBuyShield())
				{ 
					_shop->sellShield();
				}
			}
			else if (_isShopOpen && PtInRect(&_shop->getWeaponRc(), m_ptMouse))
			{
				_playerManager->buyWeapon();
				if (_playerManager->getIsBuyWeapon())
				{
					_shop->sellWepon();
				}
			}
			else if (_isShopOpen && PtInRect(&_shop->getHpPotionRc(), m_ptMouse))
			{
				_playerManager->buyHpPotiion(); 
			}
			else if (_isShopOpen && PtInRect(&_shop->getMpPotionRc(), m_ptMouse))
			{
				_playerManager->buyMpPotion(); 
			}
			else if (_isShopOpen && PtInRect(&_shop->getShopCloseButtonRc(), m_ptMouse))
			{
				_isShopOpen = false;
			}
			else if (PtInRect(&_wayRc, m_ptMouse))
			{
				_isOpenWay = true;
			}
			else if (_isOpenWay && PtInRect(&_potalCloseRc, m_ptMouse))
			{
				_isOpenWay = false;
			}
			else if (_isOpenWay && PtInRect(&_potalButtonRc, m_ptMouse))
			{
				_isChangeScene = true;
			}
			else if (_playerManager->getIsWeaponClick() && PtInRect(&_shop->getShopRc(), m_ptMouse))
			{
				_playerManager->setIsBuyWeapon(false);
				_shop->buyWeapon();
				m_player->setMoney(m_player->getMoney() + 2500);
				_playerManager->setIsWeaponClick(false);
				_playerManager->getItem()->setWeaponIndex(-1);
				_playerManager->getItem()->setWeaponRc({ 0,0 });
			}
			else if (_playerManager->getIsShieldClick() && PtInRect(&_shop->getShopRc(), m_ptMouse))
			{
				_playerManager->setIsBuyShield(false);
				_shop->buyShield();
				m_player->setMoney(m_player->getMoney() + 2000);
				_playerManager->setIsShieldClick(false);
				_playerManager->getItem()->setShieldIndex(-1);
				_playerManager->getItem()->setShieldRc({ 0,0 });
			}
			else
			{
				if (PtInRect(&_rightSkillRc, m_ptMouse))
				{
					_isRightSkillCheck = true;
				}
				else if (PtInRect(&_leftSkillRc, m_ptMouse))
				{
					_isLeftSkillCheck = true;
				} 
				else if (PtInRect(&_akara->getAkaraColRc(), m_ptMouse) && getDistance(m_player->getPlayerRc().left,m_player->getPlayerRc().top,_akara->getAkaraRc().left,_akara->getAkaraRc().top) <100 )
				{
					_isShopOpen = true;
					_playerManager->setIsOpenInven(true);
				}
				else if (_tileMap[_isoX][_isoY].tileKind[0] != TILEKIND_OBJECT && !PtInRect(&_shop->getShopRc(),m_ptMouse))
				{  
					noPath = false;
					isFind = false; 

					m_player->setPlayerEndX(isoX);
					m_player->setPlayerEndY(isoY);

					endTile = m_player->getPlayerEndY() * TILEX + m_player->getPlayerEndX();

					openList.clear();
					closeList.clear();
					startAstar = true;
					if (m_player->getIsSummonAlive()) _isFindGolemRoad = true;
				}
			}
		}
	}

	if (!isFind && !noPath &&startAstar && !_playerManager->getIsEndPointCheck())
	{
		startTile = m_player->getPlayerCurrentY() * TILEX + m_player->getPlayerCurrentX();
		currentTile = startTile;
		while (!isFind && !noPath) Astar();
	}
	_akara->update();
	_shop->update();
	if (!_isShopOpen) _shop->setShopRc(RectMake(0, 0, 0, 0));
	_playerManager->update();

	// 골렘 길찾기
	if (m_player->getIsSummon())
	{  
		_golem->setBalogLink(_vBalog);
		_golem->setOgreLink(_vOgre);
		_golem->setAndaLink(_anda);
		_golem->update();
		// 플레이어 가만히 있을 경우
		if (!_golem->getIsFindRoad() && m_player->getIsSummonAlive() && !_golem->getIsMoveEnd())
		{
			gIsFind = false;
			gNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int rndX = m_player->getPlayerCurrentX() + RND->getFromIntTo(-2, 2);
				int rndY = m_player->getPlayerCurrentY() + RND->getFromIntTo(-2, 2);
				endGolemTile = rndY * TILEX + rndX;
				if (rndX != m_player->getPlayerCurrentX() && rndY != m_player->getPlayerCurrentY()
					&& rndX != _golem->getCurrentX() && rndY != _golem->getCurrentY()
					&& _tileMap[rndX][rndY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startGolemTile = _golem->getCurrentY() * TILEX + _golem->getCurrentX();

			openList.clear();
			closeList.clear();
			startGolemAstar = true;
			currentGolemTile = startGolemTile;
		}
		// 플레이어 움직일 경우
		if (m_player->getIsMove() && !_golem->getIsEndPointCheck() && _isFindGolemRoad)
		{
			_isFindGolemRoad = false;
			_golem->setIsMoveEnd(false);
			gIsFind = false;
			gNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int rndX = _isoX + RND->getFromIntTo(-2, 2);
				int rndY = _isoY + RND->getFromIntTo(-2, 2);
				endGolemTile = rndY * TILEX + rndX;
				if (_tileMap[rndX][rndY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startGolemTile = _golem->getCurrentY() * TILEX + _golem->getCurrentX();

			openList.clear();
			closeList.clear();
			startGolemAstar = true;
			currentGolemTile = startGolemTile;
		} 
	}

	if (!gIsFind && !gNoPath &&startGolemAstar)
	{
		while (!gIsFind && !gNoPath) golemAstar();
	} 
}

void campScene::render()
{
	DrawObjTile();
	DrawTileMap(); 
	if (_isOpenWay) _potal->render(getMemDC(), 200, 100); 

	//sprintf_s(str, "isoX : %d, isoY : %d, corner : %d", _isoX, _isoY, _center);
	//TextOut(getMemDC(), 700, 20, str, strlen(str));

	//sprintf_s(str, "c.x : %d,c.y : %d ", m_player->getPlayerCurrentX(), m_player->getPlayerCurrentY());
	//TextOut(getMemDC(), 700, 40, str, strlen(str));

	//sprintf_s(str, "m_ptMouse.x : %d, m_ptMouse.y : %d ", m_ptMouse.x, m_ptMouse.y);
	//TextOut(getMemDC(), 800, 160, str, strlen(str));

	//sprintf_s(str, "m_player->getPlayerRc.left : %d, m_player->getPlayerRc.top : %d ", m_player->getPlayerRc().left, m_player->getPlayerRc().top);
	//TextOut(getMemDC(), 800, 180, str, strlen(str));

	//sprintf_s(str, "camemra : %d, camemra : %d ", m_camera->getCameraX(), m_camera->getCameraY());
	//TextOut(getMemDC(), 800, 200, str, strlen(str));

	// 그림자 랜더 
	if (m_player->getIsSummon()) _golem->shadowRender();
	_playerManager->shadowRender();
	  
	_akara->render();
	if (m_player->getIsSummon() && _golem->getGolemColRc().top <= m_player->getPlayerColRc().top) _golem->render(); 
	
	_panel->render(getMemDC(), 150, 610);

	float _tempWidth = 1 - (float)m_player->getCurrentHp() / (float)m_player->getHp();
	_lifegauge->render(getMemDC(), 195, 625 + _tempWidth * 115, 0, 0 + _tempWidth * 115, 115, 115 - _tempWidth * 115);
	_tempWidth = 1 - (float)m_player->getCurrentMp() / (float)m_player->getMp();
	_managauge->render(getMemDC(), 1185, 625 + _tempWidth * 125, 0, 0 + _tempWidth * 125, 125, 125 - _tempWidth * 125);

	// 골렘 정보
	if (m_player->getIsSummon() && !_golem->getIsDeath())
	{
		if (_golem->getHp() >= 50) _golemHp = IMAGEMANAGER->findImage("fire_golem_hp");
		if (_golem->getHp() < 50) _golemHp = IMAGEMANAGER->findImage("fire_golem_hp2");
		_golemIcon->render(getMemDC(), 10, 20);
		_golemHp->render(getMemDC(), 10, 10, 0, 0, _golemHp->getWidth() * _golem->getHp() / 100, _golemHp->getHeight());
	}
	_leftSkill->frameRender(getMemDC(), _leftSkillRc.left, _leftSkillRc.top, m_player->getLeftSkill(), 0);
	_rightSkill->frameRender(getMemDC(), _rightSkillRc.left, _rightSkillRc.top, m_player->getRightSkill(), 0);

	//for (int i = 0; i < 5; i++) Rectangle(getMemDC(), _rightSkillListRc[i].left, _rightSkillListRc[i].top, _rightSkillListRc[i].right, _rightSkillListRc[i].bottom);
	if (_isRightSkillCheck) for (int i = 0; i < 5; i++) _rightSkillList[i]->frameRender(getMemDC(), _rightSkillListRc[i].left, _rightSkillListRc[i].top, i, 0);

	if (_isDebug)
	{
		Rectangle(getMemDC(), m_player->getPlayerColRc().left, m_player->getPlayerColRc().top, m_player->getPlayerColRc().right, m_player->getPlayerColRc().bottom);
		Rectangle(getMemDC(), _golem->getGolemColRc().left, _golem->getGolemColRc().top, _golem->getGolemColRc().right, _golem->getGolemColRc().bottom);
	}

	DrawAlphaObj(); 

	if (_isShopOpen)
	{
		_shop->render();
		if (PtInRect(&_shop->getShieldRc(), m_ptMouse))
		{
			IMAGEMANAGER->findImage("shopShieldInfo")->alphaRender(getMemDC(), _shop->getShieldRc().left - 56, _shop->getShieldRc().top - 150, 180);
		}
		else if (PtInRect(&_shop->getWeaponRc(), m_ptMouse))
		{
			IMAGEMANAGER->findImage("shopWeaponInfo")->alphaRender(getMemDC(), _shop->getWeaponRc().left - 56, _shop->getWeaponRc().top - 150, 180);
		}
		else if (PtInRect(&_shop->getMpPotionRc(), m_ptMouse))
		{
			IMAGEMANAGER->findImage("mpPotionInfo")->alphaRender(getMemDC(), _shop->getMpPotionRc().left - 46, _shop->getMpPotionRc().top - 46, 180);
		}
		else if (PtInRect(&_shop->getHpPotionRc(), m_ptMouse))
		{
			IMAGEMANAGER->findImage("hpPotionInfo")->alphaRender(getMemDC(), _shop->getHpPotionRc().left - 46, _shop->getHpPotionRc().top - 46, 180);
		}
	}

	_playerManager->render();
	if (m_player->getIsSummon() && _golem->getGolemColRc().top > m_player->getPlayerColRc().top) _golem->render();
	if (_playerManager->getVSkill().size() > 0) skillCollisionCheck();
	

	// chageSCene
	if (_isChangeScene)
	{
		_black = IMAGEMANAGER->findImage("blackIcon");
		_black->render(getMemDC(), 0, 0, WINSIZEX, WINSIZEY);
		_loading = IMAGEMANAGER->findImage("loading");
		_loading->frameRender(getMemDC(), WINSIZEX / 2 - _loading->getFrameWidth() / 2, WINSIZEY / 2 - _loading->getFrameHeight() / 2, _loadingIdx, 0);

		_loadingCount++;
		if (_loadingCount % 5 == 0)
		{
			_loadingIdx++;
			if (_loadingIdx > _loading->getMaxFrameX())
			{
				//SCENEMANAGER->changeScene("campScene");
				SCENEMANAGER->changeScene("townScene");
			}
		}
	}
}

void campScene::DrawTileMap()
{ 
	int _tileX = -(m_camera->getCameraX() + 20 + m_camera->getCameraY() * 2) / 100;
	if (_tileX < 0) _tileX = 0;
	else if (_tileX > TILE_COUNT_X - 34) _tileX = TILE_COUNT_X - 34;

	int _tileY = -(m_camera->getCameraY() / 25 - (m_camera->getCameraX() - 1500) / 50) / 2;
	if (_tileY < 0) _tileY = 0;
	else if (_tileY > TILE_COUNT_Y) _tileY = TILE_COUNT_Y;

	for (int i = _tileX; i < 31 + _tileX; i++)
	{
		for (int j = _tileY; j < 31 + _tileY; j++)
		{
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;
			_tileMap[i][j].alpha[1] = 255;
			// z-order
			if (!(m_player->getPlayerCurrentX() == 35 && m_player->getPlayerCurrentY() == 41))
			{
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY()	 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() && j == m_player->getPlayerCurrentY() + 1	 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() && j == m_player->getPlayerCurrentY() + 2	 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY() + 1 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY() - 1 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY()	 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY() + 1 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY() + 2 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 1 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 2 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 3 && _tileMap[i][j].tilePos[1].y == 0 && _tileMap[i][j].tileNum[1] == 1) _tileMap[i][j].alpha[1] = 100;
			}
			for (int z = 0; z <= _tileMap[i][j].index; z++)
			{
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE)
				{
					switch (_tileMap[i][j].tileNum[z])
					{
					case 0:
						//if (_tileMap[i][j].tileKind[0] != TILEKIND_OBJECT)
							IMAGEMANAGER->frameRender("townTile", getMemDC(),
								_tileMap[i][j].left,
								_tileMap[i][j].top,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y);
						break;
					case 1:
						if (_tileMap[i][j].alpha[1] > 100)
							IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
								_tileMap[i][j].left - 50,
								_tileMap[i][j].top - 290,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y, _tileMap[i][j].alpha[1]);
						break;
					case 2: 
							IMAGEMANAGER->alphaFrameRender("townWall2", getMemDC(),
								_tileMap[i][j].left - 50,
								_tileMap[i][j].top - 290,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y, _tileMap[i][j].alpha[1]);
						break;
					}
				}
			}

			if (_isDebug)
			{
				DrawRhombus(left, top);		// 마름모
				if (_tileMap[i][j].tileKind[0] == TILEKIND_TERRAIN) SetTextColor(getMemDC(), RGB(255, 0, 0));
				else if (_tileMap[i][j].tileKind[0] == TILEKIND_OBJECT) SetTextColor(getMemDC(), RGB(0, 0, 255));
				else if (_tileMap[i][j].tileKind[0] == TILEKIND_ROAD) SetTextColor(getMemDC(), RGB(0, 255, 0));
				else if (_tileMap[i][j].tileKind[0] == TILEKIND_GOLEMROAD) SetTextColor(getMemDC(), RGB(255, 0, 255));
				else SetTextColor(getMemDC(), RGB(0, 255, 0));
				sprintf_s(str, "(%d,%d)", i, j);
				TextOut(getMemDC(), left + RADIUS_WIDTH / 2 + 8, top + RADIUS_HEIGHT / 2 + 5, str, strlen(str));
			}
		}
	}
}
// 오브젝트 밑 타일 
void campScene::DrawObjTile()
{   
		IMAGEMANAGER->frameRender("townTile", getMemDC(),_tileMap[48][37].left-5,_tileMap[48][37].top,0,1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(),_tileMap[48][38].left-5,_tileMap[48][38].top,0,1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(),_tileMap[48][39].left-5,_tileMap[48][39].top,0,1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[49][37].left - 5, _tileMap[49][37].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[49][38].left - 5, _tileMap[49][38].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[49][39].left - 5, _tileMap[49][39].top, 0, 1);


		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][37].left , _tileMap[51][37].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][38].left , _tileMap[51][38].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][39].left , _tileMap[51][39].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][37].left , _tileMap[52][37].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][38].left , _tileMap[52][38].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][39].left , _tileMap[52][39].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[38][39].left , _tileMap[38][39].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[39][39].left, _tileMap[39][39].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[41][38].left , _tileMap[41][38].top, 0, 1);
		 
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[43][37].left , _tileMap[43][37].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[43][38].left , _tileMap[43][38].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[38][41].left , _tileMap[38][41].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[38][43].left , _tileMap[38][43].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[38][45].left , _tileMap[38][45].top, 0, 1);
		for(int i = 32; i <41 ; i++) IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[31][i].left +3 , _tileMap[31][i].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[42][48].left , _tileMap[42][48].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[42][49].left , _tileMap[42][49].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[43][48].left , _tileMap[43][48].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[43][49].left , _tileMap[43][49].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[35][46].left , _tileMap[35][46].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[35][47].left , _tileMap[35][47].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[35][48].left , _tileMap[35][48].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[35][49].left , _tileMap[35][49].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[36][46].left, _tileMap[36][46].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[36][47].left, _tileMap[36][47].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[36][48].left, _tileMap[36][48].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[36][49].left, _tileMap[36][49].top, 0, 1);

		//상점
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][30].left, _tileMap[52][30].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][31].left, _tileMap[51][31].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][31].left, _tileMap[52][31].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[50][32].left, _tileMap[50][32].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][32].left, _tileMap[51][32].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[52][32].left, _tileMap[52][32].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[50][33].left, _tileMap[50][33].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[51][33].left, _tileMap[51][33].top, 0, 1);

		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[56][34].left, _tileMap[56][34].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[57][33].left, _tileMap[57][33].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[57][34].left, _tileMap[57][34].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[57][35].left, _tileMap[57][35].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[58][30].left, _tileMap[58][30].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[58][31].left, _tileMap[58][31].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[59][30].left, _tileMap[59][30].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[59][31].left, _tileMap[59][31].top, 0, 1);
		IMAGEMANAGER->frameRender("townTile", getMemDC(), _tileMap[60][33].left, _tileMap[60][33].top, 0, 1); 
		  
}

void campScene::DrawAlphaObj()
{
	int i = m_player->getPlayerCurrentX();
	int j = m_player->getPlayerCurrentY();

	if (_tileMap[i + 1][j].alpha[1] == 100 && _tileMap[i + 1][j].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 1][j].left - 50,
			_tileMap[i + 1][j].top - 290,
			_tileMap[i + 1][j].tilePos[1].x,
			_tileMap[i + 1][j].tilePos[1].y, _tileMap[i + 1][j].alpha[1]);
	}

	if (_tileMap[i][j + 1].alpha[1] == 100 && _tileMap[i][j + 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i][j + 1].left - 50,
			_tileMap[i][j + 1].top - 290,
			_tileMap[i][j + 1].tilePos[1].x,
			_tileMap[i][j + 1].tilePos[1].y, _tileMap[i][j + 1].alpha[1]);
	}


	if (_tileMap[i][j + 2].alpha[1] == 100 && _tileMap[i][j + 2].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i][j + 2].left - 50,
			_tileMap[i][j + 2].top - 290,
			_tileMap[i][j + 2].tilePos[1].x,
			_tileMap[i][j + 2].tilePos[1].y, _tileMap[i][j + 2].alpha[1]);
	}

	if (_tileMap[i + 1][j - 1].alpha[1] == 100 && _tileMap[i + 1][j - 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 1][j - 1].left - 50,
			_tileMap[i + 1][j - 1].top - 290,
			_tileMap[i + 1][j - 1].tilePos[1].x,
			_tileMap[i + 1][j - 1].tilePos[1].y, _tileMap[i + 1][j - 1].alpha[1]);

	}

	if (_tileMap[i + 1][j + 1].alpha[1] == 100 && _tileMap[i + 1][j + 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 1][j + 1].left - 50,
			_tileMap[i + 1][j + 1].top - 290,
			_tileMap[i + 1][j + 1].tilePos[1].x,
			_tileMap[i + 1][j + 1].tilePos[1].y, _tileMap[i + 1][j + 1].alpha[1]);
	}
	if (_tileMap[i + 2][j].alpha[1] == 100 && _tileMap[i + 2][j].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 2][j].left - 50,
			_tileMap[i + 2][j].top - 290,
			_tileMap[i + 2][j].tilePos[1].x,
			_tileMap[i + 2][j].tilePos[1].y, _tileMap[i + 2][j].alpha[1]);
	}

	if (_tileMap[i + 2][j + 1].alpha[1] == 100 && _tileMap[i + 2][j + 1].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 2][j + 1].left - 50,
			_tileMap[i + 2][j + 1].top - 290,
			_tileMap[i + 2][j + 1].tilePos[1].x,
			_tileMap[i + 2][j + 1].tilePos[1].y, _tileMap[i + 2][j + 1].alpha[1]);
	}

	if (_tileMap[i + 2][j + 2].alpha[1] == 100 && _tileMap[i + 2][j + 2].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 2][j + 2].left - 50,
			_tileMap[i + 2][j + 2].top - 290,
			_tileMap[i + 2][j + 2].tilePos[1].x,
			_tileMap[i + 2][j + 2].tilePos[1].y, _tileMap[i + 2][j + 2].alpha[1]);
	}

	if (_tileMap[i + 3][j + 1].alpha[1] == 100 && _tileMap[i + 3][j + 1].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 3][j + 1].left - 50,
			_tileMap[i + 3][j + 1].top - 290,
			_tileMap[i + 3][j + 1].tilePos[1].x,
			_tileMap[i + 3][j + 1].tilePos[1].y, _tileMap[i + 3][j + 1].alpha[1]);
	}


	if (_tileMap[i + 3][j + 2].alpha[1] == 100 && _tileMap[i + 3][j + 2].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 3][j + 2].left - 50,
			_tileMap[i + 3][j + 2].top - 290,
			_tileMap[i + 3][j + 2].tilePos[1].x,
			_tileMap[i + 3][j + 2].tilePos[1].y, _tileMap[i + 3][j + 2].alpha[1]);
	}


	if (_tileMap[i + 3][j + 3].alpha[1] == 100 && _tileMap[i + 3][j + 3].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("townWall", getMemDC(),
			_tileMap[i + 3][j + 3].left - 50,
			_tileMap[i + 3][j + 3].top - 290,
			_tileMap[i + 3][j + 3].tilePos[1].x,
			_tileMap[i + 3][j + 3].tilePos[1].y, _tileMap[i + 3][j + 3].alpha[1]);
	}


}

void campScene::DrawRhombus(int left, int top)
{
	int centerX = left + RADIUS_WIDTH;
	int centerY = top + RADIUS_HEIGHT;

	POINT p[5];
	p[0].x = centerX;
	p[0].y = centerY - RADIUS_HEIGHT;

	p[1].x = centerX + RADIUS_WIDTH;
	p[1].y = centerY;

	p[2].x = centerX;
	p[2].y = centerY + RADIUS_HEIGHT;

	p[3].x = centerX - RADIUS_WIDTH;
	p[3].y = centerY;

	p[4].x = centerX;
	p[4].y = centerY - RADIUS_HEIGHT;

	for (int i = 1; i < 5; i++)
	{
		HPEN hpen, hpenOld;
		hpen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hpenOld = (HPEN)SelectObject(getMemDC(), (HGDIOBJ)hpen);

		LineMake(getMemDC(), p[i - 1], p[i]); // 줄그음
		hpen = (HPEN)SelectObject(getMemDC(), hpenOld);
		DeleteObject(hpen);
	}
}

int campScene::GetCornerIndex(int isoX, int isoY)
{
	//클릭한 좌표점을 통해서 공식을 통해 렉트의 left,top을 구한다.
	int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

	float dx = (m_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (m_ptMouse.y - top) / (float)CELL_HEIGHT;

	//막약 둘다 중점 보다 작으면 1/4분면
	if (dx < 0.5f && dy < 0.5f) return 1;
	//만약 dx 중점(0.5)보다 크면 2/4분면
	if (dx >= 0.5f && dy < 0.5f) return 2;
	//만약 dx 중점(0.5)보다 작고 dy 중점보다 크면 3/4분면
	if (dx < 0.5f && dy >= 0.5f) return 3;
	//만약 둘다 중점(0.5)보다 크면 4/4
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool campScene::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) <= dx) return true;
		//정삼각형 밖에 있다면 
		else return false;
	}
	// righttop
	else if (corner == 2)
	{
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy >= dx) return true;
		else return false;
	}
	// leftdown
	else if (corner == 3) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if (dy < dx) return true;
		else return false;
	}
	// rightdown
	else if (corner == 4)
	{
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		left += RADIUS_WIDTH;
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);
		top += RADIUS_HEIGHT;

		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		if ((1.0f - dy) > dx) return true;
		else return false;
	}

	return false;
}

void campScene::TlieInit()
{
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			_tileMap[i][j].index = -1;
		}
	}
	// 바탁 타일
	for (int j = 20; j < 70; j++)
	{
		for (int i = 24; i < 70; i++)
		{
			if (j == 39 && i == 37) continue;
			if (j == 41 && i == 37) continue;
			if (j == 43 && i == 37) continue;
			 
			if (j == 39 && i == 38) continue; 
			if (j == 41 && i == 38) continue;
			if (j == 43 && i == 38) continue;

			if (j == 38 && i == 39) continue;
			if (j == 38 && i == 41) continue;
			if (j == 38 && i == 43) continue;
			if (j == 38 && i == 45) continue;

			if (j == 39 && i == 39) continue;
			  
			if (j == 48 && i == 37) continue;
			if (j == 49 && i == 37) continue;
			if (j == 50 && i == 37) continue;
			if (j == 51 && i == 37) continue;
			if (j == 52 && i == 37) continue;

			if (j == 48 && i == 38) continue;
			if (j == 49 && i == 38) continue;
			if (j == 50 && i == 38) continue;
			if (j == 51 && i == 38) continue;
			if (j == 52 && i == 38) continue;

			if (j == 48 && i == 39) continue;
			if (j == 49 && i == 39) continue;
			if (j == 50 && i == 39) continue;
			if (j == 51 && i == 39) continue;
			if (j == 52 && i == 39) continue;

			//마차2
			if (j == 31 && i == 39) continue;
			if (j == 31 && i == 37) continue;
			if (j == 31 && i == 35) continue;
			if (j == 31 && i == 33) continue;

			// 도끼
			if (j == 43 && i == 48) continue;
			if (j == 42 && i == 49) continue;
			if (j == 43 && i == 49) continue;
			
			//통나무
			if (j == 35 && i == 47) continue;
			if (j == 36 && i == 46) continue;
			if (j == 36 && i == 47) continue;
			if (j == 36 && i == 48) continue;
			if (j == 35 && i == 48) continue;
			if (j == 35 && i == 49) continue;
			if (j == 36 && i == 49) continue;

			// 상점
			if (j == 51 && i == 31) continue;
			if (j == 51 && i == 32) continue;
			if (j == 52 && i == 31) continue;
			if (j == 52 && i == 32) continue;
			if (j == 50 && i == 33) continue;
			if (j == 51 && i == 33) continue;

			if (j == 58 && i == 35) continue;
			if (j == 59 && i == 34) continue;
			if (j == 59 && i == 35) continue;

			if (j == 58 && i == 33) continue;
			if (j == 59 && i == 33) continue;
			if (j == 59 && i == 32) continue;

			if (j == 57 && i == 33) continue;
			if (j == 57 && i == 34) continue;
			if (j == 57 && i == 35) continue;
			if (j == 58 && i == 31) continue;
			if (j == 59 && i == 30) continue;
			if (j == 59 && i == 31) continue;
			if (j == 60 && i == 33) continue;


			_tileMap[j][i].index = 0;
			_tileMap[j][i].tileNum[0] = 0;
			_tileMap[j][i].tilePos[0] = { RND->getFromIntTo(0,4),RND->getFromIntTo(5,8) };
			_tileMap[j][i].tileKind[0] = TILEKIND_TERRAIN;
		}
	}


	// 웨이
	_tileMap[38][29].index = 0;
	_tileMap[38][29].tileNum[0] = 0;
	_tileMap[38][29].tilePos[0] = {1,0};
	_tileMap[38][29].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[38][28].index = 0;
	_tileMap[38][28].tileNum[0] = 0;
	_tileMap[38][28].tilePos[0] = { 3,0 };
	_tileMap[38][28].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[39][29].index = 0;
	_tileMap[39][29].tileNum[0] = 0;
	_tileMap[39][29].tilePos[0] = { 0,0 };
	_tileMap[39][29].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[39][28].index = 0;
	_tileMap[39][28].tileNum[0] = 0;
	_tileMap[39][28].tilePos[0] = { 2,0 };
	_tileMap[39][28].tileKind[0] = TILEKIND_TERRAIN;

	// 돌 
	_tileMap[44][42].index = 0;
	_tileMap[44][42].tileNum[0] = 0;
	_tileMap[44][42].tilePos[0] = { 2,37 };
	_tileMap[44][42].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[40][43].index = 0;
	_tileMap[40][43].tileNum[0] = 0;
	_tileMap[40][43].tilePos[0] = { 4,37 };
	_tileMap[40][43].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[40][34].index = 0;
	_tileMap[40][34].tileNum[0] = 0;
	_tileMap[40][34].tilePos[0] = { 4,39 };
	_tileMap[40][34].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[32][34].index = 0;
	_tileMap[32][34].tileNum[0] = 0;
	_tileMap[32][34].tilePos[0] = { 4,39 };
	_tileMap[32][34].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[33][40].index = 0;
	_tileMap[33][40].tileNum[0] = 0;
	_tileMap[33][40].tilePos[0] = { 2,39 };
	_tileMap[33][40].tileKind[0] = TILEKIND_TERRAIN;
	objInit();

}

void campScene::objInit()
{ 
	// 케릭터 위 돌기동
	_tileMap[38][38].index = 2; 
	_tileMap[38][38].tileNum[1] = 2;
	_tileMap[38][38].tilePos[1] = { 2,0 };
	_tileMap[38][38].tileNum[2] = 2;
	_tileMap[38][38].tilePos[2] = { 1,0 };
	_tileMap[38][38].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[40][38].index = 1;
	_tileMap[40][38].tileNum[1] = 2;
	_tileMap[40][38].tilePos[1] = { 3,0 }; 
	_tileMap[40][38].tileKind[0] = TILEKIND_OBJECT;
	 
	_tileMap[42][38].index = 1;
	_tileMap[42][38].tileNum[1] = 2;
	_tileMap[42][38].tilePos[1] = { 3,0 };
	_tileMap[42][38].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[44][38].index = 1;
	_tileMap[44][38].tileNum[1] = 2;
	_tileMap[44][38].tilePos[1] = { 7,0 };
	_tileMap[44][38].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[38][40].index = 1;
	_tileMap[38][40].tileNum[1] = 2;
	_tileMap[38][40].tilePos[1] = { 5,0 };
	_tileMap[38][40].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[38][42].index = 1;
	_tileMap[38][42].tileNum[1] = 2;
	_tileMap[38][42].tilePos[1] = { 5,0 };
	_tileMap[38][42].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[38][44].index = 1;
	_tileMap[38][44].tileNum[1] = 2;
	_tileMap[38][44].tilePos[1] = { 10,0 };
	_tileMap[38][44].tileKind[0] = TILEKIND_OBJECT;

	// 마차
	_tileMap[48][38].index = 1; 
	_tileMap[48][38].tileNum[0] = -1;
	_tileMap[48][38].tileNum[1] = 1;
	_tileMap[48][38].tilePos[1] = { 2,1 };
	_tileMap[48][38].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[48][38].alpha[1] = 255;

	_tileMap[50][38].index = 1; 
	_tileMap[50][38].tileNum[0] = -1;
	_tileMap[50][38].tileNum[1] = 1;
	_tileMap[50][38].tilePos[1] = { 1,1};
	_tileMap[50][38].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[50][38].alpha[1] = 255;

	_tileMap[52][38].index = 1; 
	_tileMap[52][38].tileNum[0] = -1;
	_tileMap[52][38].tileNum[1] = 1;
	_tileMap[52][38].tilePos[1] = {0,1 };
	_tileMap[52][38].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[52][38].alpha[1] = 255;

	// 마차2
	_tileMap[31][40].index = 1;
	_tileMap[31][40].tileNum[0] = -1;
	_tileMap[31][40].tileNum[1] = 1;
	_tileMap[31][40].tilePos[1] = { 8,1 };
	_tileMap[31][40].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[31][40].alpha[1] = 255;

	_tileMap[31][38].index = 1;
	_tileMap[31][38].tileNum[0] = -1;
	_tileMap[31][38].tileNum[1] = 1;
	_tileMap[31][38].tilePos[1] = { 9,1 };
	_tileMap[31][38].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[31][38].alpha[1] = 255;

	_tileMap[31][36].index = 1;
	_tileMap[31][36].tileNum[0] = -1;
	_tileMap[31][36].tileNum[1] = 1;
	_tileMap[31][36].tilePos[1] = { 10,1 };
	_tileMap[31][36].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[31][36].alpha[1] = 255;

	_tileMap[31][34].index = 1;
	_tileMap[31][34].tileNum[0] = -1;
	_tileMap[31][34].tileNum[1] = 1;
	_tileMap[31][34].tilePos[1] = { 11,1 };
	_tileMap[31][34].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[31][34].alpha[1] = 255;

	_tileMap[31][32].index = 1;
	_tileMap[31][32].tileNum[0] = -1;
	_tileMap[31][32].tileNum[1] = 1;
	_tileMap[31][32].tilePos[1] = { 12,1 };
	_tileMap[31][32].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[31][32].alpha[1] = 255;


	// 도끼
	_tileMap[42][48].index = 1;
	_tileMap[42][48].tileNum[0] = -1;
	_tileMap[42][48].tileNum[1] = 1;
	_tileMap[42][48].tilePos[1] = { 3,2 };
	_tileMap[42][48].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[42][48].alpha[1] = 255;

	// 통나무
	_tileMap[35][46].index = 1;
	_tileMap[35][46].tileNum[0] = -1;
	_tileMap[35][46].tileNum[1] = 1;
	_tileMap[35][46].tilePos[1] = { 9,2 };
	_tileMap[35][46].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[35][46].alpha[1] = 255;
	 
	_tileMap[35][48].index = 1;
	_tileMap[35][48].tileNum[0] = -1;
	_tileMap[35][48].tileNum[1] = 1;
	_tileMap[35][48].tilePos[1] = { 10,2 };
	_tileMap[35][48].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[35][48].alpha[1] = 255;


	// 상점 뒤에
	_tileMap[50][32].index = 1;
	_tileMap[50][32].tileNum[0] = -1;
	_tileMap[50][32].tileNum[1] = 1;
	_tileMap[50][32].tilePos[1] = { 20,2 };
	_tileMap[50][32].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[50][32].alpha[1] = 255;
	 
	_tileMap[52][30].index = 1;
	_tileMap[52][30].tileNum[0] = -1;
	_tileMap[52][30].tileNum[1] = 1;
	_tileMap[52][30].tilePos[1] = { 21,2 };
	_tileMap[52][30].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[52][30].alpha[1] = 255;

	// 상점 아래
	_tileMap[58][34].index = 1;
	_tileMap[58][34].tileNum[0] = -1;
	_tileMap[58][34].tileNum[1] = 1;
	_tileMap[58][34].tilePos[1] = { 12,2 };
	_tileMap[58][34].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[58][34].alpha[1] = 255;

	_tileMap[56][34].index = 1;
	_tileMap[56][34].tileNum[0] = -1;
	_tileMap[56][34].tileNum[1] = 1;
	_tileMap[56][34].tilePos[1] = { 15,2 };
	_tileMap[56][34].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[56][34].alpha[1] = 255;

	_tileMap[58][32].index = 1;
	_tileMap[58][32].tileNum[0] = -1;
	_tileMap[58][32].tileNum[1] = 1;
	_tileMap[58][32].tilePos[1] = { 14,2 };
	_tileMap[58][32].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[58][32].alpha[1] = 255;

	_tileMap[58][30].index = 1;
	_tileMap[58][30].tileNum[0] = -1;
	_tileMap[58][30].tileNum[1] = 1;
	_tileMap[58][30].tilePos[1] = { 13,2 };
	_tileMap[58][30].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[58][30].alpha[1] = 255;

	// 울타리
	 
	_tileMap[19][23].index = 2;
	_tileMap[19][23].tileNum[0] = -1;
	_tileMap[19][23].tileNum[1] = 1;
	_tileMap[19][23].tilePos[1] = { 6,0 };
	_tileMap[19][23].tileNum[2] = 1;
	_tileMap[19][23].tilePos[2] = { 7,0 };
	_tileMap[19][23].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[19][23].alpha[1] = 255;

	for (int i = 21; i < 68; i += 2)
	{
		_tileMap[i][23].index = 1;
		_tileMap[i][23].tileNum[0] = -1;
		_tileMap[i][23].tileNum[1] = 1;
		_tileMap[i][23].tilePos[1] = { 4,0 };
		_tileMap[i][23].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][23].alpha[1] = 255;
	}
	_tileMap[69][23].index = 1;
	_tileMap[69][23].tileNum[0] = -1;
	_tileMap[69][23].tileNum[1] = 1;
	_tileMap[69][23].tilePos[1] = { 24,0 }; 
	_tileMap[69][23].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[69][23].alpha[1] = 255;

	_tileMap[70][23].index = 1;
	_tileMap[70][23].tileNum[0] = -1;
	_tileMap[70][23].tileNum[1] = 1;
	_tileMap[70][23].tilePos[1] = { 17,0 };
	_tileMap[70][23].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[70][23].alpha[1] = 255;
	 
	for (int i = 25; i < 68; i += 2)
	{
		_tileMap[70][i].index = 1;
		_tileMap[70][i].tileNum[0] = -1;
		_tileMap[70][i].tileNum[1] = 1;
		_tileMap[70][i].tilePos[1] = { 22,0 };
		_tileMap[70][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[70][i].alpha[1] = 255;
	}

	_tileMap[70][69].index = 1;
	_tileMap[70][69].tileNum[0] = -1;
	_tileMap[70][69].tileNum[1] = 1;
	_tileMap[70][69].tilePos[1] = { 15,0 };
	_tileMap[70][69].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[70][69].alpha[1] = 255;

	_tileMap[69][71].index = 1;
	_tileMap[69][71].tileNum[0] = -1;
	_tileMap[69][71].tileNum[1] = 1;
	_tileMap[69][71].tilePos[1] = { 13,0 };
	_tileMap[69][71].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[69][71].alpha[1] = 255;

}

void campScene::Astar()
{

	// A*
	int endX = endTile % TILEX;
	int endY = endTile / TILEX;


	int currentX = currentTile % TILEX;
	int currentY = currentTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];

	// 방향 찾는 반복문
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// 해당 방향으로 움직인 타일이 유효한 타일인지 확인
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// 대각선 타일의 이동 문제로 (주변에 블락있으면 못감) 임시로 블락 상태 저장
			if (_tileMap[x][y].tileKind[0] == TILEKIND_OBJECT) tempBlock[i] = true;

			//if (_tileMap[x][y].block) tempBlock[i] = true;
			else {
				// check closeList z
				bool isClose = false;
				for (int j = 0; j < closeList.size(); j++)
				{
					if (closeList[j] == y * TILEX + x)
					{
						isClose = true;
						break;
					}
				}
				if (isClose) continue;

				if (i < 4)
				{
					_tileMap[x][y].g = 10;
				}
				else
				{
					// leftup인 경우 left나 up에 블락있으면 안됨
					if (i == CAMP_DIRECTION_LEFTUP &&
						tempBlock[CAMP_DIRECTION_LEFT] || tempBlock[CAMP_DIRECTION_UP]) continue;
					// rightdown인 경우 right나 down에 블락있으면 안됨
					if (i == CAMP_DIRECTION_RIGHTDOWN &&
						tempBlock[CAMP_DIRECTION_RIGHT] || tempBlock[CAMP_DIRECTION_DOWN]) continue;
					// rightup인 경우 right나 up에 블락있으면 안됨
					if (i == CAMP_DIRECTION_RIGHTUP &&
						tempBlock[CAMP_DIRECTION_RIGHT] || tempBlock[CAMP_DIRECTION_UP]) continue;
					// leftdown인 경우 left나 down에 블락있으면 안됨
					if (i == CAMP_DIRECTION_LEFTDOWN &&
						tempBlock[CAMP_DIRECTION_LEFT] || tempBlock[CAMP_DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs절대값

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// 오픈리스트에 있으면 g 비용 비교 후 처리
				isOpen = false;
				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILEX + x)
					{
						isOpen = true;
						if (_tileMap[openList[i] % TILEX][openList[i] / TILEX].g > _tileMap[x][y].g)
						{
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].h = _tileMap[x][y].h;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].g = _tileMap[x][y].g;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].f = _tileMap[x][y].f;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].node = currentTile;
						}
					}
				}
				// 없으면 그냥 넣고 부모 설정
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tileMap[x][y].node = currentTile;
				}
				// find
				if (y * TILEX + x == endTile)
					isFind = true;
			}
		}
	}

	// 선택 지점 열린목록에서 빼기
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		noPath = true;
	}

	// 현재 타일 클로즈리스트에 넣기
	closeList.push_back(currentTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		//int min = _tileMap[x][y][*openList.begin()].h;
		int min = _tileMap[*openList.begin() % TILEX][*openList.begin() / TILEX].h;
		currentTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tileMap[*iter % TILEX][*iter / TILEX].h)
			{
				min = _tileMap[*iter % TILEX][*iter / TILEX].h;
				currentTile = *iter;
			}
		}
	}
	int tempTile = endTile;

	if (isFind)
	{
		for (int i = 0; i < _vAStar.size(); i++)
			_tileMap[_vAStar.at(i) % TILEX][_vAStar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vAStar.clear();
		_vAStar.push_back(_isoY * TILEX + _isoX);
		_tileMap[_isoX][_isoY].tileKind[0] = TILEKIND_ROAD;
		m_player->setIsMove(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startTile && isFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_ROAD;
		_vAStar.push_back(tempTile);
	}
	_playerManager->setAstar(_vAStar);
}

// 골렘
void campScene::golemAstar()
{
	// A*
	int endX = endGolemTile % TILEX;
	int endY = endGolemTile / TILEX;


	int currentX = currentGolemTile % TILEX;
	int currentY = currentGolemTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];

	// 방향 찾는 반복문
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// 해당 방향으로 움직인 타일이 유효한 타일인지 확인
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// 대각선 타일의 이동 문제로 (주변에 블락있으면 못감) 임시로 블락 상태 저장
			if (_tileMap[x][y].tileKind[0] == TILEKIND_OBJECT) tempBlock[i] = true;

			//if (_tileMap[x][y].block) tempBlock[i] = true;
			else {
				// check closeList z
				bool isClose = false;
				for (int j = 0; j < closeList.size(); j++)
				{
					if (closeList[j] == y * TILEX + x)
					{
						isClose = true;
						break;
					}
				}
				if (isClose) continue;

				if (i < 4)
				{
					_tileMap[x][y].g = 10;
				}
				else
				{
					// leftup인 경우 left나 up에 블락있으면 안됨
					if (i == CAMP_DIRECTION_LEFTUP &&
						tempBlock[CAMP_DIRECTION_LEFT] || tempBlock[CAMP_DIRECTION_UP]) continue;
					// rightdown인 경우 right나 down에 블락있으면 안됨
					if (i == CAMP_DIRECTION_RIGHTDOWN &&
						tempBlock[CAMP_DIRECTION_RIGHT] || tempBlock[CAMP_DIRECTION_DOWN]) continue;
					// rightup인 경우 right나 up에 블락있으면 안됨
					if (i == CAMP_DIRECTION_RIGHTUP &&
						tempBlock[CAMP_DIRECTION_RIGHT] || tempBlock[CAMP_DIRECTION_UP]) continue;
					// leftdown인 경우 left나 down에 블락있으면 안됨
					if (i == CAMP_DIRECTION_LEFTDOWN &&
						tempBlock[CAMP_DIRECTION_LEFT] || tempBlock[CAMP_DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs절대값

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// 오픈리스트에 있으면 g 비용 비교 후 처리
				isOpen = false;
				for (int i = 0; i < openList.size(); i++)
				{
					if (openList[i] == y * TILEX + x)
					{
						isOpen = true;
						if (_tileMap[openList[i] % TILEX][openList[i] / TILEX].g > _tileMap[x][y].g)
						{
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].h = _tileMap[x][y].h;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].g = _tileMap[x][y].g;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].f = _tileMap[x][y].f;
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].node = currentGolemTile;
						}
					}
				}
				// 없으면 그냥 넣고 부모 설정
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tileMap[x][y].node = currentGolemTile;
				}
				// find
				if (y * TILEX + x == endGolemTile)
					gIsFind = true;
			}
		}
	}

	// 선택 지점 열린목록에서 빼기
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentGolemTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		gNoPath = true;
	}

	// 현재 타일 클로즈리스트에 넣기
	closeList.push_back(currentGolemTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		//int min = _tileMap[x][y][*openList.begin()].h;
		int min = _tileMap[*openList.begin() % TILEX][*openList.begin() / TILEX].h;
		currentGolemTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tileMap[*iter % TILEX][*iter / TILEX].h)
			{
				min = _tileMap[*iter % TILEX][*iter / TILEX].h;
				currentGolemTile = *iter;
			}
		}
	}
	int tempTile = endGolemTile;

	if (gIsFind )
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_vGolemAstar.push_back(endY* TILEX + endX);
		_tileMap[endX][endY].tileKind[0] = TILEKIND_GOLEMROAD;
		_golem->setIsFindRoad(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startGolemTile && gIsFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_GOLEMROAD;
		_vGolemAstar.push_back(tempTile);
	}
	if (gIsFind && _vGolemAstar.size() > 0 && _golem->getIsFindMonster()) _vGolemAstar.erase(_vGolemAstar.begin());
	_golem->setAstar(_vGolemAstar);
}

void campScene::skillCollisionCheck()
{
	vector<tagSkill> tempSkill;
	tempSkill = _playerManager->getVSkill();
	for (int i = 0; i < _playerManager->getVSkill().size(); i++)
	{
		int tempX = _playerManager->getVSkill().at(i).currentX;
		int tempY = _playerManager->getVSkill().at(i).currentY;

		if (_tileMap[tempX][tempY].tileKind[0] == TILEKIND_OBJECT)
		{
			tempSkill.erase(tempSkill.begin() + i);
			_playerManager->setVSkill(tempSkill);
		}
	}
}

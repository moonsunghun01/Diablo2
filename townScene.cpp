#include "stdafx.h"
#include "townScene.h"


townScene::townScene()
{
}


townScene::~townScene()
{
}

HRESULT townScene::init()
{
	m_player->setPlayerCurrentX(41);
	m_player->setPlayerCurrentY(34);
	//386 1887
	m_player->setPlayerRc(RectMake(386, 1887, 25, 25));

	m_camera->setCameraX(326);
	m_camera->setCameraY(-1544);

	_isoX = 0;
	_isoY = 0;
	_center = 0;
	memset(_tileMap, 0, sizeof(_tileMap));

	//_startX = INIT_X;
	//_startY = INIT_Y;
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
	 
	//40,43
	_anda = new anda;
	_anda->init(436, 762);  
	
	balog* _balog1 = new balog;

	_balog1->init(-114, 2087);
	_vBalog.push_back(_balog1);
	
	balog* _balog2 = new balog;

	_balog2->init(-64, 2212);
	_vBalog.push_back(_balog2);

	balog* _balog3 = new balog;

	_balog3->init(186,2137);
	_vBalog.push_back(_balog3);

	balog* _balog4 = new balog;

	_balog4->init(-264,2212);
	_vBalog.push_back(_balog4);

	balog* _balog5 = new balog;

	_balog5->init(-614 ,1637);
	_vBalog.push_back(_balog5);

	balog* _balog6 = new balog;

	_balog6->init(-714, 1537);
	_vBalog.push_back(_balog6);

	ogre* _ogre1 = new ogre;
	_ogre1->init(-764,1812);
	_vOgre.push_back(_ogre1);

	ogre* _ogre2 = new ogre;
	_ogre2->init(-1014, 1837);
	_vOgre.push_back(_ogre2);

	ogre* _ogre3 = new ogre;
	_ogre3->init(-264, 1612);
	_vOgre.push_back(_ogre3);

	ogre* _ogre4 = new ogre;
	_ogre4->init(-1014 ,1337);
	_vOgre.push_back(_ogre4);
	 


	_panel			= IMAGEMANAGER->findImage("panel");
	_panelRc = RectMake(396,690,700,100);
	_lifegauge		= IMAGEMANAGER->findImage("lifegauge");
	_managauge		= IMAGEMANAGER->findImage("managauge");
	_leftSkill		= IMAGEMANAGER->findImage("skill_panel");
	_rightSkill		= IMAGEMANAGER->findImage("skill_panel"); 
	_golemIcon		= IMAGEMANAGER->findImage("fire_golem_icon");
	_golemHp		= IMAGEMANAGER->findImage("fire_golem_hp");

	_backHp			= IMAGEMANAGER->findImage("backHp");
	_frontHp		 = IMAGEMANAGER->findImage("frontHp");
	for (int i = 0; i < 5; i++) _rightSkillList[i] = IMAGEMANAGER->findImage("skill_panel");

	_leftSkillRc = RectMake(325, 693, _leftSkill->getFrameWidth(), _leftSkill->getFrameHeight());
	_rightSkillRc = RectMake(1103, 693, _rightSkill->getFrameWidth(), _rightSkill->getFrameHeight());
	
	for (int i = 0; i <5; i++) _rightSkillListRc[i] = RectMake(830 + i * _rightSkill->getFrameWidth(), 580 , _rightSkill->getFrameWidth(), _rightSkill->getFrameHeight());
	
	currentSelect = SELECT_START;

	isFind = false;
	noPath = false;
	startAstar = false;
	return S_OK;
}

void townScene::release()
{
	SAFE_DELETE(_playerManager);
}

void townScene::update()
{

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
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON) )
		{
			float cellX = (float)(m_ptMouse.x - _startX);

			if (cellX < 0)
			{
				cellX = (cellX - CELL_WIDTH) / (float)CELL_WIDTH;
			}
			else
			{
				//��ŭ �������� Ŭ���ߴ°��� ��ġȭ(�������� ����)
				cellX = cellX / (float)CELL_WIDTH;
			}

			//cellY�� -������ ���� ������(�������� ���� ��)���밪���� ������
			int cellY = abs(m_ptMouse.y - _startY) / CELL_HEIGHT;
			//���� -������ �ڤ�ü�� -�̸� -1�� ��������
			cellY = (m_ptMouse.y < _startY) ? cellY * -1 : cellY;
			//x�� y�� �������� ����� ������ �۾����� �Ѵ�.
			//������ �̱� ������ x���� y�Ǳ��ذ����κ��� �ָ� ���������� x���� Ŀ����.
			int isoX = (int)cellX + (int)cellY;
			//y���� x���ذ����� ���� �ָ� ���������� y�� �þ�� �ȴ�.
			int isoY = (int)cellY - (int)cellX;
			/*
			//���� x��ǥ�� 0���� ũ�� �ִ�Ÿ�ϼ� ���� �۰�
			//Y��ǥ 0���� ũ��, �ִ�Ÿ�ϼ����� ������ ������ �������
			*/
			if (isoX >= 0 && isoX < TILE_COUNT_X &&isoY >= 0 && isoY < TILE_COUNT_Y)
			{

				//1�и��̸�[x - 1][y]
				//2�и��̸�[x][y - 1]
				//3�и��̸�[x1][y + 1]
				//4�и��̸�[x + 1][y]

				//�ڳ� ����(��� �и鿡 ��������)
				int corner = GetCornerIndex(isoX, isoY);

				if (IsInRhombus(corner, isoX, isoY))corner = 0;

				//���� 1/4�и鿡�� ������ �ȿ� ������ �ƴ϶�� �� ���� ������ ������ �ȴ�.
				switch (corner)
				{
				case 1:
					isoX = isoX - 1;//�������� 4���߿� ���� ������ �´�� �ִ� ������� x������ �Ѵܰ� �������� ��ǥ��.
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

				else if(_tileMap[_isoX][_isoY].tileKind[0] != TILEKIND_OBJECT )
				{
					//if (_isoX != m_player->getPlayerCurrentX() || _isoY != m_player->getPlayerCurrentY() )
					{ 
						noPath = false;
						isFind = false;
						//m_player->setPlayerImage(IMAGEMANAGER->findImage("necro_run"));
						//m_player->setPlayerShadowImage(IMAGEMANAGER->findImage("necro_run_shadow")); 

						m_player->setPlayerEndX(isoX);
						m_player->setPlayerEndY(isoY);
						 
						endTile = m_player->getPlayerEndY() * TILEX + m_player->getPlayerEndX();

						openList.clear();
						closeList.clear();
						startAstar = true; 
						if (m_player->getIsSummonAlive()) _isFindGolemRoad = true; 
					}/*
					else
					{
						m_player->setIsMove(false);
					}*/
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
	_playerManager->update();

	// �� ��ã��
	if (m_player->getIsSummon())
	{  
		// �� �¾������
		for (int i = 0; i < _vBalog.size(); i++)
		{
			if (_vBalog.at(i)->getGolemHit())
			{
				_vBalog.at(i)->setGolemHit(false);
				_golem->golemHit(_vBalog.at(i)->getAtk(), _vBalog.at(i)->getBalogDirection());
			}
		}

		// �� �¾������
		for (int i = 0; i < _vOgre.size(); i++)
		{
			if (_vOgre.at(i)->getGolemHit())
			{
				_vOgre.at(i)->setGolemHit(false);
				_golem->golemHit(_vOgre.at(i)->getAtk(), _vOgre.at(i)->getOgreDirection());
			}
		}
		// �� �¾������
		if (_anda->getGolemHit())
		{
			_anda->setGolemHit(false);
			_golem->golemHit(_anda->getAtk(), _anda->getAndaDirection());
		}

		_golem->setBalogLink(_vBalog);
		_golem->setOgreLink(_vOgre);
		_golem->setAndaLink(_anda);
		_golem->update();
		// �÷��̾� ������ ���� ���
		if (!_golem->getIsFindRoad() && m_player->getIsSummonAlive()  && !_golem->getIsMoveEnd())
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
					&&_tileMap[rndX][rndY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			} 

			startGolemTile = _golem->getCurrentY() * TILEX + _golem->getCurrentX(); 

			openList.clear();
			closeList.clear();
			startGolemAstar = true;
			currentGolemTile = startGolemTile; 
		}
		// �÷��̾� ������ ���
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
		// ���� ã���� ��� ( �ȴ�)
		if (_golem->getIsFindAnda() && !_golem->getIsEndPointCheck() && !_anda->getIsDeath()
			&& !(_golem->getCurrentX() == _anda->getCurrentX() && _golem->getCurrentY() == _anda->getCurrentY()))
		{
			_isFindGolemRoad = false;
			_golem->setIsMoveEnd(false);
			gIsFind = false;
			gNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int endX = _anda->getCurrentX();
				int endY = _anda->getCurrentY(); 
				endGolemTile = endY * TILEX + endX;
				if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startGolemTile = _golem->getCurrentY() * TILEX + _golem->getCurrentX();

			openList.clear();
			closeList.clear();
			startGolemAstar = true;
			currentGolemTile = startGolemTile;
		}

		// ���� ã���� ��� ( �߷�)
		if (_golem->getIsFindMonster() && !_golem->getIsEndPointCheck() && _vBalog.size()>0 
			&& !(_golem->getCurrentX() == _vBalog.at(_golem->getFindMonsterIndex())->getCurrentX() && _golem->getCurrentY() == _vBalog.at(_golem->getFindMonsterIndex())->getCurrentY()))
		{
			_isFindGolemRoad = false;
			_golem->setIsMoveEnd(false);
			gIsFind = false; 
			gNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int endX = _vBalog.at(_golem->getFindMonsterIndex())->getCurrentX();
				int endY = _vBalog.at(_golem->getFindMonsterIndex())->getCurrentY();
				//int endX = m_player->getPlayerCurrentX();
				//int endY = m_player->getPlayerCurrentY();
				endGolemTile = endY * TILEX + endX;
				if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startGolemTile = _golem->getCurrentY() * TILEX + _golem->getCurrentX();

			openList.clear();
			closeList.clear();
			startGolemAstar = true;
			currentGolemTile = startGolemTile;
		}

		// ���� ã���� ��� (�����)
		if (_golem->getIsFindOgre() && !_golem->getIsEndPointCheck() && _vOgre.size() > 0
			&& !(_golem->getCurrentX() == _vOgre.at(_golem->getFindOgreIndex())->getCurrentX() && _golem->getCurrentY() == _vOgre.at(_golem->getFindOgreIndex())->getCurrentY()))
		{
			_isFindGolemRoad = false;
			_golem->setIsMoveEnd(false);
			gIsFind = false;
			gNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int endX = _vOgre.at(_golem->getFindOgreIndex())->getCurrentX();
				int endY = _vOgre.at(_golem->getFindOgreIndex())->getCurrentY(); 
				endGolemTile = endY * TILEX + endX;
				if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
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

	for(int i =0 ; i<_vBalog.size();i++) _vBalog.at(i)->update();
	for(int i =0 ; i<_vOgre.size();i++) _vOgre.at(i)->update();
	_anda->update();

	if(_vBalog.size() > 0 )balogMove();
	if(_vOgre.size() > 0 ) ogreMove();
	andaMove();
	
	
}

void townScene::render()
{  
	DrawObjTile();
	DrawTileMap();  
	/*
	sprintf_s(str, "isoX : %d, isoY : %d, corner : %d", _isoX, _isoY, _center);
	TextOut(getMemDC(), 700, 20, str, strlen(str)); 

	sprintf_s(str, "c.x : %d,c.y : %d ",m_player->getPlayerCurrentX(), m_player->getPlayerCurrentY());
	TextOut(getMemDC(), 700, 40, str, strlen(str));
	
	sprintf_s(str, "m_ptMouse.x : %d, m_ptMouse.y : %d ", m_ptMouse.x, m_ptMouse.y);
	TextOut(getMemDC(), 800, 160, str, strlen(str));
	
	sprintf_s(str, "m_player->getPlayerRc.left : %d, m_player->getPlayerRc.top : %d ", m_player->getPlayerRc().left, m_player->getPlayerRc().top);
	TextOut(getMemDC(), 800, 180, str, strlen(str));

	sprintf_s(str, "golemX : %d, golemY : %d ",_golem->getCurrentX(), _golem->getCurrentY());
	TextOut(getMemDC(), 800, 200, str, strlen(str));*/

	// �׸��� ����
	for (int i = 0; i < _vBalog.size(); i++) _vBalog.at(i)->shadowRender();
	for (int i = 0; i < _vOgre.size(); i++) _vOgre.at(i)->shadowRender();
	_anda->shadowRender();
	if (m_player->getIsSummon()) _golem->shadowRender();
	_playerManager->shadowRender();

	renderOrder();
	

	_panel->render(getMemDC(), 150, 610);
	 
	float _tempWidth = 1 - (float)m_player->getCurrentHp() / (float)m_player->getHp(); 
	_lifegauge->render(getMemDC(), 195, 625 + _tempWidth * 115 ,0, 0 + _tempWidth * 115 ,115, 115 - _tempWidth * 115);
	_tempWidth = 1 - (float)m_player->getCurrentMp() / (float)m_player->getMp();
	_managauge->render(getMemDC(), 1185, 625 + _tempWidth * 125 ,0, 0 + _tempWidth * 125 ,125, 125 - _tempWidth * 125);
	
	// �� ����
	if (m_player->getIsSummon() && _golem->getHp() >= 0 )
	{
		if (_golem->getHp() >= 50) _golemHp = IMAGEMANAGER->findImage("fire_golem_hp");
		if (_golem->getHp() < 50) _golemHp = IMAGEMANAGER->findImage("fire_golem_hp2");
		_golemIcon->render(getMemDC(), 10, 20);
		_golemHp->render(getMemDC(), 10, 10,0,0,_golemHp->getWidth() * _golem->getHp() / 100, _golemHp->getHeight());
	}
	_leftSkill->frameRender(getMemDC(), _leftSkillRc.left, _leftSkillRc.top, m_player->getLeftSkill(), 0);
	_rightSkill->frameRender(getMemDC(), _rightSkillRc.left, _rightSkillRc.top, m_player->getRightSkill() , 0);

	//for (int i = 0; i < 5; i++) Rectangle(getMemDC(), _rightSkillListRc[i].left, _rightSkillListRc[i].top, _rightSkillListRc[i].right, _rightSkillListRc[i].bottom);
	if(_isRightSkillCheck) for (int i = 0; i < 5; i++) _rightSkillList[i]->frameRender(getMemDC(), _rightSkillListRc[i].left, _rightSkillListRc[i].top, i,0);

	if (_isDebug)
	{
		Rectangle(getMemDC(), m_player->getPlayerColRc().left, m_player->getPlayerColRc().top, m_player->getPlayerColRc().right, m_player->getPlayerColRc().bottom);
		Rectangle(getMemDC(), _golem->getGolemColRc().left, _golem->getGolemColRc().top, _golem->getGolemColRc().right, _golem->getGolemColRc().bottom);
		for (int i =0; i<_vBalog.size();i++)
		Rectangle(getMemDC(), _vBalog.at(i)->getBalogColRc().left, _vBalog.at(i)->getBalogColRc().top, _vBalog.at(i)->getBalogColRc().right, _vBalog.at(i)->getBalogColRc().bottom);
	}
	
	DrawAlphaObj();

	if (_playerManager->getVSkill().size() > 0) skillCollisionCheck();
	if (_anda->getVSkill().size() > 0) bossSkillCollisionCheck();
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	for (int i = 0; i < _vBalog.size(); i++)
	{
		if (PtInRect(&_vBalog.at(i)->getBalogColRc(), m_ptMouse) && !_vBalog.at(i)->getIsDeath())
		{ 
			_backHp->render(getMemDC(),WINSIZEX/2 - _backHp->getWidth() / 2,30,0,0, _backHp->getWidth(), _backHp->getHeight());
			_frontHp->render(getMemDC(),WINSIZEX/2 - _frontHp->getWidth() / 2,30,0,0, _frontHp->getWidth() * _vBalog.at(i)->getHp() / 50, _frontHp->getHeight());
			
			sprintf_s(str, " �� �� " );
			TextOut(getMemDC(), WINSIZEX / 2 - 20, 38, str, strlen(str));
		}
	}

	for (int i = 0; i < _vOgre.size(); i++)
	{
		if (PtInRect(&_vOgre.at(i)->getOgreColRc(), m_ptMouse) && !_vOgre.at(i)->getIsDeath())
		{
			_backHp->render(getMemDC(), WINSIZEX / 2 - _backHp->getWidth() / 2, 30, 0, 0, _backHp->getWidth(), _backHp->getHeight());
			_frontHp->render(getMemDC(), WINSIZEX / 2 - _frontHp->getWidth() / 2, 30, 0, 0, _frontHp->getWidth() * _vOgre.at(i)->getHp() / 50, _frontHp->getHeight());

			sprintf_s(str, " �� �� �� ");
			TextOut(getMemDC(), WINSIZEX / 2 - 30, 38, str, strlen(str));
		}
	}

	if (PtInRect(&_anda->getAndaColRc(), m_ptMouse) && !_anda->getIsDeath())
	{
		_backHp->render(getMemDC(), WINSIZEX / 2 - _backHp->getWidth() / 2, 30, 0, 0, _backHp->getWidth(), _backHp->getHeight());
		_frontHp->render(getMemDC(), WINSIZEX / 2 - _frontHp->getWidth() / 2, 30, 0, 0, _frontHp->getWidth() * _anda->getHp() / 500, _frontHp->getHeight());

		sprintf_s(str, " �� �� �� �� ");
		TextOut(getMemDC(), WINSIZEX / 2 - 40, 38, str, strlen(str));
	}
}

void townScene::renderOrder()
{
	for (int i = 0; i < _vBalog.size(); i++)
		if (_vBalog.at(i)->getBalogColRc().top <= m_player->getPlayerColRc().top && _vBalog.at(i)->getBalogColRc().top <= _golem->getGolemColRc().top) _vBalog.at(i)->render();
	for (int i = 0; i < _vOgre.size(); i++)
		if (_vOgre.at(i)->getOgreColRc().top <= m_player->getPlayerColRc().top  && _vOgre.at(i)->getOgreColRc().top <= _golem->getGolemColRc().top) _vOgre.at(i)->render();

	if (_anda->getAndaColRc().top <= m_player->getPlayerColRc().top && _anda->getAndaColRc().top <= _golem->getGolemColRc().top) _anda->render();

	if (m_player->getIsSummon() && _golem->getGolemColRc().top <= m_player->getPlayerColRc().top) _golem->render();

	if (_anda->getAndaColRc().top <= m_player->getPlayerColRc().top && _anda->getAndaColRc().top > _golem->getGolemColRc().top) _anda->render();

	for (int i = 0; i < _vBalog.size(); i++)
		if (_vBalog.at(i)->getBalogColRc().top <= m_player->getPlayerColRc().top && _vBalog.at(i)->getBalogColRc().top > _golem->getGolemColRc().top) _vBalog.at(i)->render();
	for (int i = 0; i < _vOgre.size(); i++)
		if (_vOgre.at(i)->getOgreColRc().top <= m_player->getPlayerColRc().top && _vOgre.at(i)->getOgreColRc().top > _golem->getGolemColRc().top) _vOgre.at(i)->render();

	_playerManager->render();
	 
	if (_anda->getAndaColRc().top > m_player->getPlayerColRc().top && _anda->getAndaColRc().top <= _golem->getGolemColRc().top) _anda->render();

	for (int i = 0; i < _vOgre.size(); i++)
		if (_vOgre.at(i)->getOgreColRc().top > m_player->getPlayerColRc().top && _vOgre.at(i)->getOgreColRc().top <= _golem->getGolemColRc().top )_vOgre.at(i)->render();
	
	for (int i = 0; i < _vBalog.size(); i++)
		if (_vBalog.at(i)->getBalogColRc().top > m_player->getPlayerColRc().top && _vBalog.at(i)->getBalogColRc().top <= _golem->getGolemColRc().top)_vBalog.at(i)->render();
	if (m_player->getIsSummon() && _golem->getGolemColRc().top > m_player->getPlayerColRc().top) _golem->render();
	
	for (int i = 0; i < _vBalog.size(); i++)
		if (_vBalog.at(i)->getBalogColRc().top > m_player->getPlayerColRc().top && _vBalog.at(i)->getBalogColRc().top > _golem->getGolemColRc().top)_vBalog.at(i)->render();
	for (int i = 0; i < _vOgre.size(); i++)
		if (_vOgre.at(i)->getOgreColRc().top > m_player->getPlayerColRc().top && _vOgre.at(i)->getOgreColRc().top > _golem->getGolemColRc().top)_vOgre.at(i)->render();
	if (_anda->getAndaColRc().top > m_player->getPlayerColRc().top && _anda->getAndaColRc().top > _golem->getGolemColRc().top) _anda->render();

}

void townScene::DrawTileMap()
{ 
	
	int _tileX = -(m_camera->getCameraX() + 20 + m_camera->getCameraY() * 2) / 100; 
	if (_tileX < 0) _tileX = 0;
	else if (_tileX > TILE_COUNT_X -34) _tileX = TILE_COUNT_X-34;
 
	int _tileY = -(m_camera->getCameraY() /25 -  (m_camera->getCameraX() - 1500) / 50 ) / 2;
	if (_tileY < 0) _tileY = 0;
	else if (_tileY > TILE_COUNT_Y) _tileY = TILE_COUNT_Y; 

	for (int i = _tileX; i < 32 + _tileX; i++)
	{
		for (int j = _tileY; j < 35 + _tileY; j++)
		{
			int left = _startX + (i * RADIUS_WIDTH) - (j * RADIUS_WIDTH);
			int top = _startY + (i * RADIUS_HEIGHT) + (j * RADIUS_HEIGHT);

			_tileMap[i][j].left = left;
			_tileMap[i][j].top = top;
			_tileMap[i][j].alpha[1] = 255;
			// z-order
			if (!(m_player->getPlayerCurrentX() == 35 && m_player->getPlayerCurrentY() == 41))
			{
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY()) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() && j == m_player->getPlayerCurrentY() + 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() && j == m_player->getPlayerCurrentY() + 2) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY() + 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 1 && j == m_player->getPlayerCurrentY() - 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY()) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY() + 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 2 && j == m_player->getPlayerCurrentY() + 2) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 1) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 2) _tileMap[i][j].alpha[1] = 100;
				if (i == m_player->getPlayerCurrentX() + 3 && j == m_player->getPlayerCurrentY() + 3) _tileMap[i][j].alpha[1] = 100;
			}
			for (int z = 0; z <= _tileMap[i][j].index; z++)
			{
				if (_tileMap[i][j].tileKind[z] != TILEKIND_NONE)
				{
					switch (_tileMap[i][j].tileNum[z])
					{
					case 0:
						if (_tileMap[i][j].tileKind[0] != TILEKIND_OBJECT)
						IMAGEMANAGER->frameRender("act1Tiles", getMemDC(),
							_tileMap[i][j].left,
							_tileMap[i][j].top,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y);
						break;
					case 1:
						if (_tileMap[i][j].alpha[1] > 100)
							IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
								_tileMap[i][j].left - 50,
								_tileMap[i][j].top - 290,
								_tileMap[i][j].tilePos[z].x,
								_tileMap[i][j].tilePos[z].y, _tileMap[i][j].alpha[1]);
						break;
					case 2:
						IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(),
							_tileMap[i][j].left ,
							_tileMap[i][j].top,
							_tileMap[i][j].tilePos[z].x,
							_tileMap[i][j].tilePos[z].y );
						break;
					}
				}
			}

			if (_isDebug)
			{
				DrawRhombus(left, top);		// ������
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

void townScene::DrawObjTile()
{
	
	// ������Ʈ �Ʒ� Ÿ�� 
	for (int i = 13; i < 29; i++)
	{
		IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(),
			_tileMap[i][33].left + 5,
			_tileMap[i][33].top,
			0,
			10);
	}
	
	for (int i = 18; i < 28; i++)
	{
		IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(),
			_tileMap[i][32].left + 5,
			_tileMap[i][32].top,
			0,
			10);
	} 
	
	for (int i = 33; i < 53; i++)
	{
		IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(),
			_tileMap[35][i].left +5,
			_tileMap[35][i].top ,
			0,
			10);
	}

	for (int i = 13; i < 48; i++)
	{
		IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(),
			_tileMap[i][33].left,
			_tileMap[i][33].top,
			0,
			10);
	}
	for (int i =34; i < 52; i++)
	{
		IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(), _tileMap[13][i].left, _tileMap[13][i].top,
			0,
			0);
	}  

	IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(), _tileMap[19][35].left, _tileMap[19][35].top, 0, 0); 
	IMAGEMANAGER->frameRender("act1Tiles2", getMemDC(), _tileMap[23][35].left, _tileMap[23][35].top, 0, 0); 
}

void townScene::DrawAlphaObj()
{
	int i = m_player->getPlayerCurrentX();
	int j = m_player->getPlayerCurrentY();
	
	if (_tileMap[i + 1][j].alpha[1] == 100 && _tileMap[i + 1][j].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 1][j].left - 50,
			_tileMap[i + 1][j].top - 290,
			_tileMap[i + 1][j].tilePos[1].x,
			_tileMap[i + 1][j].tilePos[1].y, _tileMap[i + 1][j].alpha[1]);
	}

	if (_tileMap[i][j + 1].alpha[1] == 100 && _tileMap[i][j + 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i][j + 1].left - 50,
			_tileMap[i][j + 1].top - 290,
			_tileMap[i][j + 1].tilePos[1].x,
			_tileMap[i][j + 1].tilePos[1].y, _tileMap[i][j + 1].alpha[1]);
	}


	if (_tileMap[i][j + 2].alpha[1] == 100 && _tileMap[i][j + 2].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i][j + 2].left - 50,
			_tileMap[i][j + 2].top - 290,
			_tileMap[i][j + 2].tilePos[1].x,
			_tileMap[i][j + 2].tilePos[1].y, _tileMap[i][j + 2].alpha[1]);
	}

	if (_tileMap[i + 1][j - 1].alpha[1] == 100 && _tileMap[i + 1][j - 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 1][j - 1].left - 50,
			_tileMap[i + 1][j - 1].top - 290,
			_tileMap[i + 1][j - 1].tilePos[1].x,
			_tileMap[i + 1][j - 1].tilePos[1].y, _tileMap[i + 1][j - 1].alpha[1]);

	}

	if (_tileMap[i + 1][j + 1].alpha[1] == 100 && _tileMap[i + 1][j + 1].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 1][j + 1].left - 50,
			_tileMap[i + 1][j + 1].top - 290,
			_tileMap[i + 1][j + 1].tilePos[1].x,
			_tileMap[i + 1][j + 1].tilePos[1].y, _tileMap[i + 1][j + 1].alpha[1]);
	}
	if (_tileMap[i + 2][j].alpha[1] == 100 && _tileMap[i + 2][j].tileNum[1] == 1)
	{

		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 2][j].left - 50,
			_tileMap[i + 2][j].top - 290,
			_tileMap[i + 2][j].tilePos[1].x,
			_tileMap[i + 2][j].tilePos[1].y, _tileMap[i + 2][j].alpha[1]);
	}

	if (_tileMap[i + 2][j + 1].alpha[1] == 100 && _tileMap[i + 2][j + 1].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 2][j + 1].left - 50,
			_tileMap[i + 2][j + 1].top - 290,
			_tileMap[i + 2][j + 1].tilePos[1].x,
			_tileMap[i + 2][j + 1].tilePos[1].y, _tileMap[i + 2][j + 1].alpha[1]);
	}

	if (_tileMap[i + 2][j + 2].alpha[1] == 100 && _tileMap[i + 2][j + 2].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 2][j + 2].left - 50,
			_tileMap[i + 2][j + 2].top - 290,
			_tileMap[i + 2][j + 2].tilePos[1].x,
			_tileMap[i + 2][j + 2].tilePos[1].y, _tileMap[i + 2][j + 2].alpha[1]);
	}

	if (_tileMap[i + 3][j + 1].alpha[1] == 100 && _tileMap[i + 3][j + 1].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 3][j + 1].left - 50,
			_tileMap[i + 3][j + 1].top - 290,
			_tileMap[i + 3][j + 1].tilePos[1].x,
			_tileMap[i + 3][j + 1].tilePos[1].y, _tileMap[i + 3][j + 1].alpha[1]);
	}


	if (_tileMap[i + 3][j + 2].alpha[1] == 100 && _tileMap[i + 3][j + 2].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 3][j + 2].left - 50,
			_tileMap[i + 3][j + 2].top - 290,
			_tileMap[i + 3][j + 2].tilePos[1].x,
			_tileMap[i + 3][j + 2].tilePos[1].y, _tileMap[i + 3][j + 2].alpha[1]);
	}


	if (_tileMap[i + 3][j + 3].alpha[1] == 100 && _tileMap[i + 3][j + 3].tileNum[1] == 1)
	{
		IMAGEMANAGER->alphaFrameRender("act1Wall3", getMemDC(),
			_tileMap[i + 3][j + 3].left - 50,
			_tileMap[i + 3][j + 3].top - 290,
			_tileMap[i + 3][j + 3].tilePos[1].x,
			_tileMap[i + 3][j + 3].tilePos[1].y, _tileMap[i + 3][j + 3].alpha[1]);
	}
	

}

void townScene::DrawRhombus(int left, int top)
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

		LineMake(getMemDC(), p[i - 1], p[i]); // �ٱ���
		hpen = (HPEN)SelectObject(getMemDC(), hpenOld);
		DeleteObject(hpen);
	}
}

int townScene::GetCornerIndex(int isoX, int isoY)
{
	//Ŭ���� ��ǥ���� ���ؼ� ������ ���� ��Ʈ�� left,top�� ���Ѵ�.
	int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
	int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);


	//left���� ���콺���� ���� ����Ʈ���� �̵��� ���� ���µ�,
	//�̰� ��ü���̷� ������ 0.0~ 1.0 (float���� ������ , ������ float�̴ϱ�)�� ���� ���´�
	//���������� ������ ������ ��Ʈ�� 1~4������ ��������
	//���� ��ġ�� �� �� �ִ�.
	float dx = (m_ptMouse.x - left) / (float)CELL_WIDTH;
	float dy = (m_ptMouse.y - top) / (float)CELL_HEIGHT;

	//���� �Ѵ� ���� ���� ������ 1/4�и�
	if (dx < 0.5f && dy < 0.5f) return 1;
	//���� dx ����(0.5)���� ũ�� 2/4�и�
	if (dx >= 0.5f && dy < 0.5f) return 2;
	//���� dx ����(0.5)���� �۰� dy �������� ũ�� 3/4�и�
	if (dx < 0.5f && dy >= 0.5f) return 3;
	//���� �Ѵ� ����(0.5)���� ũ�� 4/4
	if (dx >= 0.5f && dy >= 0.5f) return 4;

	return 0;
}

bool townScene::IsInRhombus(int corner, int isoX, int isoY)
{
	// lefttop
	if (corner == 1) {
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		int top = _startY + (isoX * RADIUS_HEIGHT) + (isoY * RADIUS_HEIGHT);

		//left ���� ���콺���� ����(left���� �̵��Ÿ�)��
		//left�� ���������� top�� ������ ���̷� �ϴ� ���� �簢������ 
		//����ϱ� ���� dx�� dy�� ���Ѵ�.
		float dx = (float)(m_ptMouse.x - left) / RADIUS_WIDTH;
		float dy = (float)(m_ptMouse.y - top) / RADIUS_HEIGHT;

		//���� ��ü ��ʣO���� ���ﰢ�� �ȿ� ���� �ϱ� ������
		//������ dy�� dx���� ���ų� ���� �� �ۿ� ����.
		//���ﰢ�� �ȿ� �� �մ� ���� ������ y�� x���� �۰ų� 
		//���� �� �ۿ� ����.

		//1���ִ� ������ top�������� ���̸� ��� �߱� ������
		//���ҿ��������� ���� ���ϱ� ����
		if ((1.0f - dy) <= dx) return true;
		//���ﰢ�� �ۿ� �ִٸ� 
		else return false;
	}
	// righttop
	else if (corner == 2)
	{
		int left = _startX + (isoX * RADIUS_WIDTH) - (isoY * RADIUS_WIDTH);
		//����Ʈ�� ������ ������ ���ϴ� ������
		//�˻��ϴ� ��Ʈ�� ���������� �������� �������� left��
		//�ƴ� ���������� ���� ��Ʈ(�簢��)�̱� ������
		//���� �������� ���Ѵ�
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

void townScene::TlieInit()
{
	for (int i = 0; i < TILE_COUNT_X; i++)
	{
		for (int j = 0; j < TILE_COUNT_Y; j++)
		{
			_tileMap[i][j].index = -1; 
		}
	}  

	for (int j = 36; j < 49; j++)
	{
		for (int i = 34; i < 52; i++)
		{
			_tileMap[j][i].index = 0;
			_tileMap[j][i].tileNum[0] = 2;
			_tileMap[j][i].tilePos[0] = { RND->getFromIntTo(0,4),10 };
			_tileMap[j][i].tileKind[0] = TILEKIND_TERRAIN;
		}
	}

	// Ÿ�� �ٴ� ����
	// �ذ�
	_tileMap[38][38].index = 0;
	_tileMap[38][38].tileNum[0] = 2;
	_tileMap[38][38].tilePos[0] = { 2,1 };
	_tileMap[38][38].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[39][38].index = 0;
	_tileMap[39][38].tileNum[0] = 2;
	_tileMap[39][38].tilePos[0] = { 1,1 };
	_tileMap[39][38].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[38][39].index = 0;
	_tileMap[38][39].tileNum[0] = 2;
	_tileMap[38][39].tilePos[0] = { 0,1 };
	_tileMap[38][39].tileKind[0] = TILEKIND_TERRAIN;
	 
	_tileMap[39][39].tileNum[0] = 2;
	_tileMap[39][39].tilePos[0] = { 4,0 };
	_tileMap[39][39].tileKind[0] = TILEKIND_OBJECT;

	// �ذ� 
	_tileMap[42][46].tileNum[0] = 2;
	_tileMap[42][46].tilePos[0] = { 2,1 };
	_tileMap[42][46].tileKind[0] = TILEKIND_OBJECT;
	 
	_tileMap[43][46].tileNum[0] = 2;
	_tileMap[43][46].tilePos[0] = { 1,1 };
	_tileMap[43][46].tileKind[0] = TILEKIND_OBJECT;
	 
	_tileMap[42][47].tileNum[0] = 2;
	_tileMap[42][47].tilePos[0] = { 0,1 };
	_tileMap[42][47].tileKind[0] = TILEKIND_TERRAIN;
	 
	_tileMap[43][47].tileNum[0] = 2;
	_tileMap[43][47].tilePos[0] = { 4,0 };
	_tileMap[43][47].tileKind[0] = TILEKIND_OBJECT;

	// ���ڱ�
	_tileMap[42][38].tileNum[0] = 2;
	_tileMap[42][38].tilePos[0] = { 2,8 };
	_tileMap[42][38].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[41][40].tileNum[0] = 2;
	_tileMap[41][40].tilePos[0] = { 2,9 };
	_tileMap[41][40].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[37][43].tileNum[0] = 2;
	_tileMap[37][43].tilePos[0] = { 1,8 };
	_tileMap[37][43].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[39][44].tileNum[0] = 2;
	_tileMap[39][44].tilePos[0] = { 1,8 };
	_tileMap[39][44].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[42][43].tileNum[0] = 2;
	_tileMap[42][43].tilePos[0] = { 3,1 };
	_tileMap[42][43].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[36][46].tileNum[0] = 2;
	_tileMap[36][46].tilePos[0] = { 4,7 };
	_tileMap[36][46].tileKind[0] = TILEKIND_TERRAIN;

	for (int j = 14; j < 35; j++)
	{
		for (int i = 34; i < 52; i++)
		{
			_tileMap[j][i].index = 0;
			_tileMap[j][i].tileNum[0] = 2;
			_tileMap[j][i].tilePos[0] = { RND->getFromIntTo(0,4),10 };
			_tileMap[j][i].tileKind[0] = TILEKIND_TERRAIN;
		}
	}

	for (int j = 14; j < 28; j++)
	{
		for (int i = 6; i < 32; i++)
		{
			if (j == 20 && i == 9)continue;
			_tileMap[j][i].index = 0;
			_tileMap[j][i].tileNum[0] = 2;
			_tileMap[j][i].tilePos[0] = { RND->getFromIntTo(0,4),10 };
			_tileMap[j][i].tileKind[0] = TILEKIND_TERRAIN;
		}
	}


	// �ǿ�����
	_tileMap[19][40].tileNum[0] = 2;
	_tileMap[19][40].tilePos[0] = { 0,11 };
	_tileMap[19][40].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[19][41].tileNum[0] = 2;
	_tileMap[19][41].tilePos[0] = { 1,5 };
	_tileMap[19][41].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[20][40].tileNum[0] = 2;
	_tileMap[20][40].tilePos[0] = { 4,5 };
	_tileMap[20][40].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[21][40].tileNum[0] = 2;
	_tileMap[21][40].tilePos[0] = { 3,5 };
	_tileMap[21][40].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[21][41].tileNum[0] = 2;
	_tileMap[21][41].tilePos[0] = { 4,4 };
	_tileMap[21][41].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[20][41].tileNum[0] = 2;
	_tileMap[20][41].tilePos[0] = { 0,5 };
	_tileMap[20][41].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[20][42].tileNum[0] = 2;
	_tileMap[20][42].tilePos[0] = { 2,4 };
	_tileMap[20][42].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[19][42].tileNum[0] = 2;
	_tileMap[19][42].tilePos[0] = { 3,4 };
	_tileMap[19][42].tileKind[0] = TILEKIND_TERRAIN;

	////////////////

	// �ذ�
	_tileMap[26][37].index = 0;
	_tileMap[26][37].tileNum[0] = 2;
	_tileMap[26][37].tilePos[0] = { 2,1 };
	_tileMap[26][37].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[27][37].index = 0;
	_tileMap[27][37].tileNum[0] = 2;
	_tileMap[27][37].tilePos[0] = { 1,1 };
	_tileMap[27][37].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[26][38].index = 0;
	_tileMap[26][38].tileNum[0] = 2;
	_tileMap[26][38].tilePos[0] = { 0,1 };
	_tileMap[26][38].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[27][38].tileNum[0] = 2;
	_tileMap[27][38].tilePos[0] = { 4,0 };
	_tileMap[27][38].tileKind[0] = TILEKIND_OBJECT;

	//// �ذ�
	_tileMap[24][44].tileNum[0] = 2;
	_tileMap[24][44].tilePos[0] = { 3,0 };
	_tileMap[24][44].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[25][44].tileNum[0] = 2;
	_tileMap[25][44].tilePos[0] = { 2,0 };
	_tileMap[25][44].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[24][45].tileNum[0] = 2;
	_tileMap[24][45].tilePos[0] = { 1,0 };
	_tileMap[24][45].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[25][45].tileNum[0] = 2;
	_tileMap[25][45].tilePos[0] = { 0,0 };
	_tileMap[25][45].tileKind[0] = TILEKIND_TERRAIN;

	//// �ذ�
	_tileMap[15][42].index = 0;
	_tileMap[15][42].tileNum[0] = 2;
	_tileMap[15][42].tilePos[0] = { 2,1 };
	_tileMap[15][42].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[16][42].index = 0;
	_tileMap[16][42].tileNum[0] = 2;
	_tileMap[16][42].tilePos[0] = { 1,1 };
	_tileMap[16][42].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[15][43].index = 0;
	_tileMap[15][43].tileNum[0] = 2;
	_tileMap[15][43].tilePos[0] = { 0,1 };
	_tileMap[15][43].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[16][43].tileNum[0] = 2;
	_tileMap[16][43].tilePos[0] = { 4,0 };
	_tileMap[16][43].tileKind[0] = TILEKIND_OBJECT;
	//// �ذ�

	// ���ڱ�
	_tileMap[30][42].index = 0;
	_tileMap[30][42].tileNum[0] = 2;
	_tileMap[30][42].tilePos[0] = { 1,8 };

	_tileMap[29][45].index = 0;
	_tileMap[29][45].tileNum[0] = 2;
	_tileMap[29][45].tilePos[0] = { 1,9 };

	_tileMap[26][41].index = 0;
	_tileMap[26][41].tileNum[0] = 2;
	_tileMap[26][41].tilePos[0] = { 0,9 };

	_tileMap[19][45].index = 0;
	_tileMap[19][45].tileNum[0] = 2;
	_tileMap[19][45].tilePos[0] = { 4,7 };

	objInit();
	
}

void townScene::objInit()
{ 
	for (int i = 15; i < 29; i += 2)
	{
		_tileMap[i][5].index = 1;
		_tileMap[i][5].tileNum[1] = 1;
		_tileMap[i][5].tilePos[1] = { RND->getFromIntTo(2,5),2 };
		_tileMap[i][5].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][5].alpha[1] = 255;
	}

	_tileMap[13][5].index = 2;
	_tileMap[13][5].tileNum[1] = 1;
	_tileMap[13][5].tilePos[1] = { 0,1 };
	_tileMap[13][5].alpha[1] = 255;

	_tileMap[13][5].index = 2;
	_tileMap[13][5].tileNum[0] = 1;
	_tileMap[13][5].tilePos[0] = { 3,1 };
	_tileMap[13][5].alpha[0] = 255;

	for (int i = 7; i < 32; i += 2)
	{
		_tileMap[13][i].index = 1;
		_tileMap[13][i].tileNum[1] = 1;
		_tileMap[13][i].tilePos[1] = { RND->getFromIntTo(2,5),3 };
		_tileMap[13][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[13][i].alpha[1] = 255;
	}

	// ���� �� ��
	for (int i = 7; i < 32; i += 2)
	{
		_tileMap[29][i].index = 1;
		_tileMap[29][i].tileNum[1] = 1;
		_tileMap[29][i].tilePos[1] = { RND->getFromIntTo(2,5),3 };
		_tileMap[29][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[29][i].alpha[1] = 255;
	}
	_tileMap[29][5].index = 1;
	_tileMap[29][5].tileNum[1] = 1;
	_tileMap[29][5].tilePos[1] = {1,3 };
	_tileMap[29][5].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[29][5].alpha[1] = 255;

	// ������ ���
	for (int i = 25; i > 9; i -= 4)
	{
		_tileMap[16][i].index = 1;
		_tileMap[16][i].tileNum[1] = 1;
		_tileMap[16][i].tilePos[1] = { 1,7 };
		_tileMap[16][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[16][i].alpha[1] = 255;
		 
	}
	


	// 1�� ��
	for (int i = 37; i < 48; i += 2)
	{
		_tileMap[i][33].index = 1;
		_tileMap[i][33].tileNum[1] = 1;
		_tileMap[i][33].tilePos[1] = { RND->getFromIntTo(2,5),2 };
		_tileMap[i][33].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][33].alpha[1] = 255;
	}
	_tileMap[39][33].index = 1;
	_tileMap[39][33].tileNum[1] = 1;
	_tileMap[39][33].tilePos[1] = { 4,4 };
	_tileMap[39][33].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[39][33].alpha[1] = 255;

	_tileMap[41][33].index = 1;
	_tileMap[41][33].tileNum[1] = 1;
	_tileMap[41][33].tilePos[1] = { 5,4 };
	_tileMap[41][33].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[41][33].alpha[1] = 255;

	_tileMap[49][33].index = 1;
	_tileMap[49][33].tileNum[1] = 1;
	_tileMap[49][33].tilePos[1] = { 1 ,3 };
	_tileMap[49][33].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[49][33].alpha[1] = 255;

	// 2�� ��
	for (int i = 35; i < 52; i += 2)
	{
		_tileMap[49][i].index = 1;
		_tileMap[49][i].tileNum[1] = 1;
		_tileMap[49][i].tilePos[1] = { RND->getFromIntTo(2,5),3 };
		_tileMap[49][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[49][i].alpha[1] = 255;
	}
	_tileMap[49][53].index = 1;
	_tileMap[49][53].tileNum[1] = 1;
	_tileMap[49][53].tilePos[1] = { 0,0 };
	_tileMap[49][53].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[49][53].alpha[1] = 255;

	//3�� ��
	for (int i = 37; i < 48; i += 2)
	{
		_tileMap[i][53].index = 1;
		_tileMap[i][53].tileNum[1] = 1;
		_tileMap[i][53].tilePos[1] = { RND->getFromIntTo(2,5),2 };
		_tileMap[i][53].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][53].alpha[1] = 255;
	}

	_tileMap[35][53].index = 1;
	_tileMap[35][53].tileNum[1] = 1;
	_tileMap[35][53].tilePos[1] = { 0,2 };
	_tileMap[35][53].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[35][53].alpha[1] = 255;

	// 4����
	for (int i = 35; i < 52; i += 2)
	{
		_tileMap[35][i].index = 1;
		_tileMap[35][i].tileNum[1] = 1;
		_tileMap[35][i].tilePos[1] = { 3,3 };
		_tileMap[35][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[35][i].alpha[1] = 255;
	}  
	
	// 4�� �� 
	_tileMap[35][39].index = 1;
	_tileMap[35][39].tileNum[1] = 1;
	_tileMap[35][39].tilePos[1] = { 4,1 };
	_tileMap[35][39].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[35][39].alpha[1] = 255;

	_tileMap[35][41].index = 1;
	_tileMap[35][41].tileNum[1] = 1;
	_tileMap[35][41].tilePos[1] = { 5,1 };
	_tileMap[35][41].alpha[1] = 255;

	// 4�� �Ա� Ÿ��
	_tileMap[35][41].tileNum[0] = 2;
	_tileMap[35][41].tilePos[0] = { 0,0 };
	_tileMap[35][41].tileKind[0] = TILEKIND_TERRAIN;
	//

	_tileMap[35][33].index = 2;
	_tileMap[35][33].tileNum[1] = 1;
	_tileMap[35][33].tilePos[1] = { 3 ,1 };
	_tileMap[35][33].alpha[1] = 255;

	_tileMap[35][33].index = 2;
	_tileMap[35][33].tileNum[0] = 1;
	_tileMap[35][33].tilePos[0] = { 0 ,1 };
	_tileMap[35][33].alpha[0] = 255;


	for (int i = 15; i < 35; i += 2)
	{
		_tileMap[i][33].index = 1;
		_tileMap[i][33].tileNum[1] = 1;
		_tileMap[i][33].tilePos[1] = { RND->getFromIntTo(2,5),2 };
		_tileMap[i][33].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][33].alpha[1] = 255;
	}


	_tileMap[13][33].index = 2;
	_tileMap[13][33].tileNum[1] = 1;
	_tileMap[13][33].tilePos[1] = { 0,1 };
	_tileMap[13][33].alpha[1] = 255;

	_tileMap[13][33].index = 2;
	_tileMap[13][33].tileNum[0] = 1;
	_tileMap[13][33].tilePos[0] = { 3,1 };
	_tileMap[13][33].alpha[0] = 255;

	for (int i = 35; i < 52; i += 2)
	{
		_tileMap[13][i].index = 1;
		_tileMap[13][i].tileNum[1] = 1;
		_tileMap[13][i].tilePos[1] = { 3,3 };
		_tileMap[13][i].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[13][i].alpha[1] = 255;
	}

	_tileMap[13][53].index = 1;
	_tileMap[13][53].tileNum[1] = 1;
	_tileMap[13][53].tilePos[1] = { 0,2 };
	_tileMap[13][53].tileKind[0] = TILEKIND_OBJECT;
	_tileMap[13][53].alpha[1] = 255;

	for (int i = 15; i < 48; i += 2)
	{
		_tileMap[i][53].index = 1;
		_tileMap[i][53].tileNum[1] = 1;
		_tileMap[i][53].tilePos[1] = { RND->getFromIntTo(2,5),2 };
		_tileMap[i][53].tileKind[0] = TILEKIND_OBJECT;
		_tileMap[i][53].alpha[1] = 255;
	}

	// ��
	_tileMap[19][33].index = 2;
	_tileMap[19][33].tileNum[1] = 1;
	_tileMap[19][33].tilePos[1] = { 5 ,6 };
	_tileMap[19][33].alpha[1] = 255;

	_tileMap[21][33].tileNum[1] = 1;
	_tileMap[21][33].tilePos[1] = { 4 ,6 };
	_tileMap[21][33].alpha[1] = 255;
	 
	// �� �ٴ� Ÿ��  
	_tileMap[20][33].tileNum[0] = 2;
	_tileMap[20][33].tilePos[0] = { 0,10 };
	_tileMap[20][33].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[21][33].tileNum[0] = 2;
	_tileMap[21][33].tilePos[0] = { 0 ,10 }; 
	_tileMap[21][33].tileKind[0] = TILEKIND_TERRAIN;

	_tileMap[22][33].tileNum[0] = 2;
	_tileMap[22][33].tilePos[0] = { 0 ,10 };
	_tileMap[22][33].tileKind[0] = TILEKIND_TERRAIN;

	for (int i = 18; i < 28; i++)
	{
		_tileMap[i][32].tileNum[0] = 2;
		_tileMap[i][32].tilePos[0] = { 3 ,10 };
		_tileMap[i][32].tileKind[0] = TILEKIND_TERRAIN;
	}

	// �� �� ������Ʈ
	_tileMap[19][35].index = 1;
	_tileMap[19][35].tileNum[1] = 1;
	_tileMap[19][35].tilePos[1] = { 2,6 };
	_tileMap[19][35].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[23][35].index = 1;
	_tileMap[23][35].tileNum[1] = 1;
	_tileMap[23][35].tilePos[1] = { 2,6 };
	_tileMap[23][35].tileKind[0] = TILEKIND_OBJECT;

	// ������
	_tileMap[16][9].index = 1;
	_tileMap[16][9].tileNum[1] = 1;
	_tileMap[16][9].tilePos[1] = { 2,6 };
	_tileMap[16][9].tileKind[0] = TILEKIND_OBJECT;

	_tileMap[19][9].index = 1;
	_tileMap[19][9].tileNum[1] = 1;
	_tileMap[19][9].tilePos[1] = { 0,7 };
	_tileMap[19][9].tileKind[0] = TILEKIND_OBJECT;
	  

	_tileMap[23][9].index = 1;
	_tileMap[23][9].tileNum[1] = 1;
	_tileMap[23][9].tilePos[1] = { 2,6 };
	_tileMap[23][9].tileKind[0] = TILEKIND_OBJECT;
} 

void townScene::Astar()
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

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
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
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs���밪

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
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
				// ������ �׳� �ְ� �θ� ����
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

	// ���� ���� ������Ͽ��� ����
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

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
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

// ��
void townScene::golemAstar()
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

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
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
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs���밪

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
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
				// ������ �׳� �ְ� �θ� ����
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

	// ���� ���� ������Ͽ��� ����
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

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
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

	if (gIsFind)
	//if (gIsFind && !_golem->getIsFindMonster())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_vGolemAstar.push_back(endY* TILEX + endX);
		_tileMap[endX][endY].tileKind[0] = TILEKIND_GOLEMROAD;
		_golem->setIsFindRoad(true);
	}
	/*if (gIsFind && !_golem->getIsFindOgre())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_vGolemAstar.push_back(endY* TILEX + endX);
		_tileMap[endX][endY].tileKind[0] = TILEKIND_GOLEMROAD;
		_golem->setIsFindRoad(true);
	}
	if (gIsFind && !_golem->getIsFindAnda())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_vGolemAstar.push_back(endY* TILEX + endX);
		_tileMap[endX][endY].tileKind[0] = TILEKIND_GOLEMROAD;
		_golem->setIsFindRoad(true);
	}*/

	if (gIsFind && _golem->getIsFindAnda())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_golem->setIsFindAnda(false);
		_golem->setIsFindRoad(true);
	}

	if (gIsFind && _golem->getIsFindMonster())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_golem->setIsFindMonster(false);
		_golem->setIsFindRoad(true);
	}

	if (gIsFind && _golem->getIsFindOgre())
	{
		for (int i = 0; i < _vGolemAstar.size(); i++)
			_tileMap[_vGolemAstar.at(i) % TILEX][_vGolemAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vGolemAstar.clear();
		_golem->setIsFindOgre(false);
		_golem->setIsFindRoad(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startGolemTile && gIsFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_GOLEMROAD;
		_vGolemAstar.push_back(tempTile);
	}
	if (gIsFind && _vGolemAstar.size() > 0 && _golem->getIsFindMonster() ) _vGolemAstar.erase(_vGolemAstar.begin());
	if (gIsFind && _vGolemAstar.size() > 0 && _golem->getIsFindOgre() ) _vGolemAstar.erase(_vGolemAstar.begin());
	if (gIsFind && _vGolemAstar.size() > 0 && _golem->getIsFindAnda() ) _vGolemAstar.erase(_vGolemAstar.begin());

	_golem->setAstar(_vGolemAstar);
}

//�߷� ������ 
void townScene::balogMove()
{
	for (int i = 0; i < _vBalog.size(); i++)
	{ 
		//��ã��
		if (m_player->getIsSummon() && !_vBalog.at(i)->getIsDeath() && !_golem->getIsDeath() && !(_vBalog.at(i)->getCurrentX() == _golem->getCurrentX() && _vBalog.at(i)->getCurrentY() == _golem->getCurrentY()))
		{
			int _tempDis;
			_tempDis = abs(_vBalog.at(i)->getCurrentX() - _golem->getCurrentX()) + abs(_vBalog.at(i)->getCurrentY() - _golem->getCurrentY());

			if (_tempDis < 6 )
			{
				_vBalog.at(i)->setIsFindGolem(true);
				//_vBalog.at(i)->setIsAtkTimeCheck(false);
				_vBalog.at(i)->setIsFindTarget(true);
				_vBalog.at(i)->setEndX(_golem->getCurrentX());
				_vBalog.at(i)->setEndY(_golem->getCurrentY());
			}
		}
		if (_vBalog.at(i)->getIsFindTarget() && !_vBalog.at(i)->getIsEndPointCheck() && !_vBalog.at(i)->getIsDeath() && !( _vBalog.at(i)->getCurrentX() == m_player->getPlayerCurrentX() && _vBalog.at(i)->getCurrentY() == m_player->getPlayerCurrentY() )
			&& !(_vBalog.at(i)->getCurrentX() == _golem->getCurrentX() && _vBalog.at(i)->getCurrentY() == _golem->getCurrentY() ))
		{ 
			_monsterIdx = i;
			_isFindMonRoad = false;
			_vBalog.at(i)->setIsMoveEnd(false);
			mIsFind = false; 
			mNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int endX = _vBalog.at(i)->getEndX();
				int endY = _vBalog.at(i)->getEndY();
				endMonTile = endY * TILEX + endX;
				if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startMonTile = _vBalog.at(i)->getCurrentY() * TILEX + _vBalog.at(i)->getCurrentX();

			openList.clear();
			closeList.clear();
			startMonAstar = true;
			currentMonTile = startMonTile;
		}

		if (!mIsFind && !mNoPath &&startMonAstar)
		{
			while (!mIsFind && !mNoPath) balogAstar();
		}
	} 
}
  
// �����
void townScene::ogreMove()
{
	for (int i = 0; i < _vOgre.size(); i++)
	{
		//��ã��
		if (m_player->getIsSummon() && !_vOgre.at(i)->getIsDeath() && !_golem->getIsDeath() && !(_vOgre.at(i)->getCurrentX() == _golem->getCurrentX() && _vOgre.at(i)->getCurrentY() == _golem->getCurrentY()))
		{
			int _tempDis;
			_tempDis = abs(_vOgre.at(i)->getCurrentX() - _golem->getCurrentX()) + abs(_vOgre.at(i)->getCurrentY() - _golem->getCurrentY());

			if (_tempDis < 6)
			{
				_vOgre.at(i)->setIsFindGolem(true); 
				_vOgre.at(i)->setIsFindTarget(true);
				_vOgre.at(i)->setEndX(_golem->getCurrentX());
				_vOgre.at(i)->setEndY(_golem->getCurrentY());
			}
		}
		if (_vOgre.at(i)->getIsFindTarget() && !_vOgre.at(i)->getIsEndPointCheck() && !_vOgre.at(i)->getIsDeath() && !(_vOgre.at(i)->getCurrentX() == m_player->getPlayerCurrentX() && _vOgre.at(i)->getCurrentY() == m_player->getPlayerCurrentY())
			&& !(_vOgre.at(i)->getCurrentX() == _golem->getCurrentX() && _vOgre.at(i)->getCurrentY() == _golem->getCurrentY()))
		{
			_monsterIdx = i;
			_isFindOgreRoad = false;
			_vOgre.at(i)->setIsMoveEnd(false);
			oIsFind = false;
			oNoPath = false;
			bool checkEndPoint = true;
			while (checkEndPoint)
			{
				int endX = _vOgre.at(i)->getEndX();
				int endY = _vOgre.at(i)->getEndY();
				endOgreTile = endY * TILEX + endX;
				if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
			}

			startOgreTile = _vOgre.at(i)->getCurrentY() * TILEX + _vOgre.at(i)->getCurrentX();

			openList.clear();
			closeList.clear();
			startOgreAstar = true;
			currentOgreTile = startOgreTile;
		}

		if (!oIsFind && !oNoPath &&startOgreAstar)
		{
			while (!oIsFind && !oNoPath) ogreAstar();
		}
	}
}


// �ȴ�
void townScene::andaMove()
{ //��ã��
	if (m_player->getIsSummon() && !_anda->getIsDeath() && !_golem->getIsDeath() && !(_anda->getCurrentX() == _golem->getCurrentX() && _anda->getCurrentY() == _golem->getCurrentY()))
	{
		int _tempDis;
		_tempDis = abs(_anda->getCurrentX() - _golem->getCurrentX()) + abs(_anda->getCurrentY() - _golem->getCurrentY());

		if (_tempDis < 6)
		{
			_anda->setIsFindGolem(true);
			_anda->setIsFindTarget(true);
			_anda->setEndX(_golem->getCurrentX());
			_anda->setEndY(_golem->getCurrentY());
		}
	}
	// ������
	if (_anda->getIsFindTarget() && !_anda->getIsEndPointCheck() && !_anda->getIsDeath() && !(_anda->getCurrentX() == m_player->getPlayerCurrentX() && _anda->getCurrentY() == m_player->getPlayerCurrentY())
		&& !(_anda->getCurrentX() == _golem->getCurrentX() && _anda->getCurrentY() == _golem->getCurrentY()))
	{ 
		_isFindAndaRoad = false;
		_anda->setIsMoveEnd(false);
		aIsFind = false;
		aNoPath = false;
		bool checkEndPoint = true;
		while (checkEndPoint)
		{
			int endX = _anda->getEndX();
			int endY = _anda->getEndY();
			endAndaTile = endY * TILEX + endX;
			if (_tileMap[endX][endY].tileKind[0] != TILEKIND_OBJECT) checkEndPoint = false;
		}

		startAndaTile = _anda->getCurrentY() * TILEX + _anda->getCurrentX();

		openList.clear();
		closeList.clear();
		startAndaAstar = true;
		currentAndaTile = startAndaTile;
	}

	if (!aIsFind && !aNoPath &&startAndaAstar)
	{
		while (!aIsFind && !aNoPath) andaAstar();
	}
	
}
void townScene::skillCollisionCheck()
{
	vector<tagSkill> tempSkill;
	tempSkill = _playerManager->getVSkill();
	for (int i = 0; i < _playerManager->getVSkill().size(); i++)
	{ 
		int tempX = _playerManager->getVSkill().at(i).currentX;
		int tempY = _playerManager->getVSkill().at(i).currentY;

		if (_tileMap[tempX][tempY].tileKind[0] == TILEKIND_OBJECT)
		{ 
			tempSkill.erase(tempSkill.begin() + i );
			_playerManager->setVSkill(tempSkill);
		}
	} 
	for (int i = 0; i < _vBalog.size(); i++)
	{
		if (!_vBalog.at(i)->getIsDeath())
		{
			for (int j = 0; j < _playerManager->getVSkill().size(); j++)
			{
				RECT rc;
				if (IntersectRect(&rc, &_playerManager->getVSkill().at(j).skillRc, &_vBalog.at(i)->getBalogColRc()))
				{
					tempSkill.erase(tempSkill.begin() + j);
					_playerManager->setVSkill(tempSkill);
					_vBalog.at(i)->balogHit(m_player->getAttackDamage());
				}
			}
		}
	}
	for (int i = 0; i < _vOgre.size(); i++)
	{
		if (!_vOgre.at(i)->getIsDeath())
		{
			for (int j = 0; j < _playerManager->getVSkill().size(); j++)
			{
				RECT rc;
				if (IntersectRect(&rc, &_playerManager->getVSkill().at(j).skillRc, &_vOgre.at(i)->getOgreColRc()))
				{
					tempSkill.erase(tempSkill.begin() + j);
					_playerManager->setVSkill(tempSkill);
					_vOgre.at(i)->ogreHit(m_player->getAttackDamage());
				}
			}
		}
	}

	if (!_anda->getIsDeath())
	{
		for (int j = 0; j < _playerManager->getVSkill().size(); j++)
		{
			RECT rc;
			if (IntersectRect(&rc, &_playerManager->getVSkill().at(j).skillRc, &_anda->getAndaColRc()))
			{
				tempSkill.erase(tempSkill.begin() + j);
				_playerManager->setVSkill(tempSkill);
				_anda->andaHit(m_player->getAttackDamage());
			}
		}
	}
}

// ������ų
void townScene::bossSkillCollisionCheck()
{
	vector<bossSkill> tempSkill;
	tempSkill = _anda->getVSkill();
	for (int i = 0; i < _anda->getVSkill().size(); i++)
	{
		int tempX = _anda->getVSkill().at(i).currentX;
		int tempY = _anda->getVSkill().at(i).currentY;

		if (_tileMap[tempX][tempY].tileKind[0] == TILEKIND_OBJECT)
		{
			tempSkill.erase(tempSkill.begin() + i);
			_anda->setVSkill(tempSkill);
		}
	}
	if (!_golem->getIsDeath())
	{
		for (int j = 0; j < _anda->getVSkill().size(); j++)
		{
			RECT rc;
			if (IntersectRect(&rc, &_anda->getVSkill().at(j).rc, &_golem->getGolemColRc()))
			{
				tempSkill.erase(tempSkill.begin() + j);
				_anda->setVSkill(tempSkill);
				_golem->golemHit(_anda->getAtk(),_anda->getAndaDirection());
			}
		}
	} 
	for (int i = 0; i < _anda->getVSkill().size(); i++)
	{
		RECT rc;
		if (IntersectRect(&rc, &_anda->getVSkill().at(i).rc, &m_player->getPlayerColRc()))
		{
			tempSkill.erase(tempSkill.begin() + i);
			_anda->setVSkill(tempSkill);
			m_player->playerHit(_anda->getAtk(), _anda->getAndaDirection());
		}
	}  
}
 
void townScene::balogAstar()
{
	// A*
	int endX = endMonTile % TILEX;
	int endY = endMonTile / TILEX;


	int currentX = currentMonTile % TILEX;
	int currentY = currentMonTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
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
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs���밪

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
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
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].node = currentMonTile;
						}
					}
				}
				// ������ �׳� �ְ� �θ� ����
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tileMap[x][y].node = currentMonTile;
				}
				// find
				if (y * TILEX + x == endMonTile)
					mIsFind = true;
			}
		}
	}

	// ���� ���� ������Ͽ��� ����
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentMonTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		mNoPath = true;
	}

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
	closeList.push_back(currentMonTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		//int min = _tileMap[x][y][*openList.begin()].h;
		int min = _tileMap[*openList.begin() % TILEX][*openList.begin() / TILEX].h;
		currentMonTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tileMap[*iter % TILEX][*iter / TILEX].h)
			{
				min = _tileMap[*iter % TILEX][*iter / TILEX].h;
				currentMonTile = *iter;
			}
		}
	}
	int tempTile = endMonTile;

	if (mIsFind && _vBalog.at(_monsterIdx)->getIsFindTarget())
	{
		for (int i = 0; i < _vMonAstar.size(); i++)
			_tileMap[_vMonAstar.at(i) % TILEX][_vMonAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vMonAstar.clear();  
		_vBalog.at(_monsterIdx)->setIsFindRoad(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startMonTile && mIsFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_GOLEMROAD;
		_vMonAstar.push_back(tempTile);
	}

	//if (mIsFind && _vMonAstar.size() > 0 && _vBalog.at(_monsterIdx)->getIsFindGolem()) _vMonAstar.erase(_vMonAstar.begin());
	_vBalog.at(_monsterIdx)->setAstar(_vMonAstar);
}

// ����� ���̽�Ÿ
void townScene::ogreAstar()
{
	// A*
	int endX = endOgreTile % TILEX;
	int endY = endOgreTile / TILEX;


	int currentX = currentOgreTile % TILEX;
	int currentY = currentOgreTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
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
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs���밪

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
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
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].node = currentOgreTile;
						}
					}
				}
				// ������ �׳� �ְ� �θ� ����
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tileMap[x][y].node = currentOgreTile;
				}
				// find
				if (y * TILEX + x == endOgreTile)
					oIsFind = true;
			}
		}
	}

	// ���� ���� ������Ͽ��� ����
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentOgreTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		oNoPath = true;
	}

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
	closeList.push_back(currentOgreTile);

	if (openList.size() != 0)
	{
		// find minimum f cost in openList
		//int min = _tileMap[x][y][*openList.begin()].h;
		int min = _tileMap[*openList.begin() % TILEX][*openList.begin() / TILEX].h;
		currentOgreTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tileMap[*iter % TILEX][*iter / TILEX].h)
			{
				min = _tileMap[*iter % TILEX][*iter / TILEX].h;
				currentOgreTile = *iter;
			}
		}
	}
	int tempTile = endOgreTile;

	if (oIsFind && _vOgre.at(_monsterIdx)->getIsFindTarget())
	{
		for (int i = 0; i < _vOgreAstar.size(); i++)
			_tileMap[_vOgreAstar.at(i) % TILEX][_vOgreAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vOgreAstar.clear();
		_vOgre.at(_monsterIdx)->setIsFindRoad(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startOgreTile && oIsFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_GOLEMROAD;
		_vOgreAstar.push_back(tempTile);
	} 
	_vOgre.at(_monsterIdx)->setAstar(_vOgreAstar);
}

// �ȴ� ���̽�Ÿ
void townScene::andaAstar()
{
	// A*
	int endX = endAndaTile % TILEX;
	int endY = endAndaTile / TILEX;


	int currentX = currentAndaTile % TILEX;
	int currentY = currentAndaTile / TILEX;
	// left, right, up, down, leftup, rightdown, leftdown, rightup
	int dx[] = { -1, 1, 0, 0, -1, 1, -1, 1 };
	int dy[] = { 0, 0, -1, 1, -1, 1, 1, -1 };
	bool tempBlock[8];

	// ���� ã�� �ݺ���
	for (int i = 0; i < 8; i++)
	{
		int x = currentX + dx[i];
		int y = currentY + dy[i];
		tempBlock[i] = false;

		// �ش� �������� ������ Ÿ���� ��ȿ�� Ÿ������ Ȯ��
		if (0 <= x && x < TILEX && 0 <= y && y < TILEY)
		{
			bool isOpen;
			// �밢�� Ÿ���� �̵� ������ (�ֺ��� ��������� ����) �ӽ÷� ��� ���� ����
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
					// leftup�� ��� left�� up�� ��������� �ȵ�
					if (i == DIRECTION_LEFTUP &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_UP]) continue;
					// rightdown�� ��� right�� down�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTDOWN &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_DOWN]) continue;
					// rightup�� ��� right�� up�� ��������� �ȵ�
					if (i == DIRECTION_RIGHTUP &&
						tempBlock[DIRECTION_RIGHT] || tempBlock[DIRECTION_UP]) continue;
					// leftdown�� ��� left�� down�� ��������� �ȵ�
					if (i == DIRECTION_LEFTDOWN &&
						tempBlock[DIRECTION_LEFT] || tempBlock[DIRECTION_DOWN]) continue;
					_tileMap[x][y].g = 14;

				}
				//abs���밪

				_tileMap[x][y].h = (abs(endX - x) + abs(endY - y)) * 10;
				_tileMap[x][y].f = _tileMap[x][y].g + _tileMap[x][y].h;

				// ���¸���Ʈ�� ������ g ��� �� �� ó��
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
							_tileMap[openList[i] % TILEX][openList[i] / TILEX].node = currentAndaTile;
						}
					}
				}
				// ������ �׳� �ְ� �θ� ����
				if (!isOpen)
				{
					openList.push_back(y * TILEX + x);
					_tileMap[x][y].node = currentAndaTile;
				}
				// find
				if (y * TILEX + x == endAndaTile)
					aIsFind = true;
			}
		}
	}

	// ���� ���� ������Ͽ��� ����
	for (iter = openList.begin(); iter != openList.end(); ++iter)
	{
		if ((*iter) == currentAndaTile)
		{
			iter = openList.erase(iter);
			break;
		}
	}

	// not Find
	if (openList.size() == 0)
	{
		aNoPath = true;
	}

	// ���� Ÿ�� Ŭ�����Ʈ�� �ֱ�
	closeList.push_back(currentAndaTile);

	if (openList.size() != 0)
	{ 
		int min = _tileMap[*openList.begin() % TILEX][*openList.begin() / TILEX].h;
		currentAndaTile = *openList.begin();
		for (iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (min > _tileMap[*iter % TILEX][*iter / TILEX].h)
			{
				min = _tileMap[*iter % TILEX][*iter / TILEX].h;
				currentAndaTile = *iter;
			}
		}
	}
	int tempTile = endAndaTile;

	if (aIsFind && _anda->getIsFindTarget())
	{
		for (int i = 0; i < _vAndaAstar.size(); i++)
			_tileMap[_vAndaAstar.at(i) % TILEX][_vAndaAstar.at(i) / TILEX].tileKind[0] = TILEKIND_TERRAIN;
		_vAndaAstar.clear();
		_anda->setIsFindRoad(true);
	}

	while (_tileMap[tempTile % TILEX][tempTile / TILEY].node != startAndaTile && aIsFind)
	{
		tempTile = _tileMap[tempTile % TILEX][tempTile / TILEY].node;
		_tileMap[tempTile % TILEX][tempTile / TILEY].tileKind[0] = TILEKIND_GOLEMROAD;
		_vAndaAstar.push_back(tempTile);
	}
	_anda->setAstar(_vAndaAstar);
}

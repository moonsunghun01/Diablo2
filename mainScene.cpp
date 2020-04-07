#include "stdafx.h"
#include "mainScene.h"

HRESULT mainScene::init()
{
	_backGround = IMAGEMANAGER->findImage("main");
	_startButton = IMAGEMANAGER->findImage("startButton");
	_endButton = IMAGEMANAGER->findImage("endButton");
	_loading = IMAGEMANAGER->findImage("loading");
	_logo = IMAGEMANAGER->findImage("logo");
	return S_OK;
}

void mainScene::release()
{
}

void mainScene::update()
{
	_startButtonRc = RectMake(610, 400, _startButton->getFrameWidth(), _startButton->getFrameHeight());
	_endButtonRc = RectMake(610, 400, _endButton->getFrameWidth(), _endButton->getFrameHeight());

	if (PtInRect(&_startButtonRc, m_ptMouse) && KEYMANAGER->isOnceKeyDown(VK_LBUTTON)) _startButtonClick = !_startButtonClick; 
	if (PtInRect(&_startButtonRc, m_ptMouse) && KEYMANAGER->isOnceKeyUp(VK_LBUTTON))_isLoading = true;
}

void mainScene::render()
{
	if (!_isLoading)
	{
		_backGround->render(getMemDC(), 250, 0);
		_logo->frameRender(getMemDC(), 470, 10,0,_logoIdx);
		_startButton->frameRender(getMemDC(), 610, 400, 0, _startButtonClick);
		_endButton->frameRender(getMemDC(), 610, 500, 0,_endButtonClick);

		_logoCount++;
		if (_logoCount % 5 == 0)
		{
			_logoIdx++;
			if (_logoIdx > _loading->getMaxFrameX()) _logoIdx = _logoCount = 0;
		}
	}
	else if (_isLoading)
	{
		_loading->frameRender(getMemDC(), WINSIZEX / 2 - _loading->getFrameWidth() / 2, WINSIZEY / 2 - _loading->getFrameHeight()/2, _loadingIdx, 0);

		_loadingCount++;
		if (_loadingCount % 5 == 0)
		{
			_loadingIdx++;
			if (_loadingIdx > _loading->getMaxFrameX())
			{
				SCENEMANAGER->changeScene("campScene");
			}
		}
	}
}

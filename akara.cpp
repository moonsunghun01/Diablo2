#include "stdafx.h"
#include "akara.h"

HRESULT akara::init()
{
	_akara = IMAGEMANAGER->findImage("akara");
	_akaraShadow = IMAGEMANAGER->findImage("akaraShadow");
	_akaraRc = RectMake(986 ,2137 ,25,25);
	return S_OK;
}

void akara::update()
{
	_akaraColRc = RectMake(_akaraRc.left + m_camera->getCameraX(), _akaraRc.top + m_camera->getCameraY(),25,100);
}

void akara::render()
{
	_akaraShadow->frameRender(getMemDC(), _akaraRc.left + m_camera->getCameraX() - 25 , _akaraRc.top + m_camera->getCameraY(), _akaraIdx, 0);
	_akara->frameRender(getMemDC(), _akaraRc.left + m_camera->getCameraX() , _akaraRc.top + m_camera->getCameraY(), _akaraIdx, 0);

	_akaraCount++;
	if (_akaraCount % 3 == 0)
	{
		_akaraIdx++;
		if (_akaraIdx > _akara->getMaxFrameX()) _akaraIdx = 0;
	}
}

void akara::shadowRender()
{
}

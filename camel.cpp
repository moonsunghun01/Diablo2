#include "stdafx.h"
#include "camel.h"


camel::camel()
{
}


camel::~camel()
{
}

HRESULT camel::init()
{
	//

	////처음 부터 끝까지 돌리자
	//_ani1 = new animation;
	//_ani1->init(_camel->getWidth(), _camel->getHeight(), 75, 89);
	//_ani1->setDefPlayFrame(false, true);
	//_ani1->setFPS(1);
	////배열에 담아서 돌리자
	//int arrAni[] = { 0,1,2,3,4,5,6,7,8,9 };
	//_ani2 = new animation;
	//_ani2->init(_camel->getWidth(), _camel->getHeight(), 75, 89);
	//_ani2->setPlayFrame(arrAni, 10, true);
	//_ani2->setFPS(1);

	//_ani3 = new animation;
	//_ani3->init(_camel->getWidth(), _camel->getHeight(), 75, 89);
	//_ani3->setPlayFrame(1, 10, false, true);
	//_ani3->setFPS(1);

	_camel = IMAGEMANAGER->addFrameImage("camel", "images/camel.bmp", 300, 267,4,3, true, RGB(255, 0, 255));

	ANIMATIONMANAGER->addDefAnimation("ani1", "camel", 10, false, true);

	int arrlen[] = { 1,2,3,7,8 };
	ANIMATIONMANAGER->addAnimation("ani2", "camel", arrlen, 5, 5, true);

	ANIMATIONMANAGER->addAnimation("ani3", "camel",0, 5,10,false, true);

	//시작 애니메이션 설정
	_ani = ANIMATIONMANAGER->findAnimation("ani1");

	return S_OK;
}

void camel::release()
{/*
	SAFE_DELETE(_ani1);
	SAFE_DELETE(_ani2);
	SAFE_DELETE(_ani3);*/
}

void camel::update()
{
	if(KEYMANAGER->isOnceKeyDown('1'))
	{
		//_ani1->start();
		_ani = ANIMATIONMANAGER->findAnimation("ani1");
		ANIMATIONMANAGER->start("ani1");
	}
	if (KEYMANAGER->isOnceKeyDown('2'))
	{
		//_ani2->start();
		_ani = ANIMATIONMANAGER->findAnimation("ani2");
		ANIMATIONMANAGER->start("ani2");
	}
	if (KEYMANAGER->isOnceKeyDown('3'))
	{
		//_ani3->start();
		_ani = ANIMATIONMANAGER->findAnimation("ani3");
		ANIMATIONMANAGER->start("ani3");
	}
	//_ani1->frameUpdate(0.4f);
	//_ani2->frameUpdate(0.1f);
	//_ani3->frameUpdate(1.0f);

}

void camel::render()
{
	/*_camel->aniRender(getMemDC(), WINSIZEX /2 -200, WINSIZEY / 2, _ani1);
	_camel->aniRender(getMemDC(), WINSIZEX /2 , WINSIZEY / 2, _ani2);
	_camel->aniRender(getMemDC(), WINSIZEX /2 +200, WINSIZEY / 2, _ani3);*/
	_camel->aniRender(getMemDC(), WINSIZEX /2, WINSIZEY / 2, _ani);
}

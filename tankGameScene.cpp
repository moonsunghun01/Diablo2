#include "stdafx.h"
#include "tankGameScene.h"


tankGameScene::tankGameScene()
{
}


tankGameScene::~tankGameScene()
{
}

HRESULT tankGameScene::init()
{

	//¸Ê
	_tankMap = new tankMap;
	_tankMap->init();

	//ÅÊÅ©
	_tank = new tank;
	_tank->init("tank");
	_tank->setTankMapMemoryAdress(_tankMap);
	_tank->setTankPosition(_tankMap->getMap()[_tankMap->getPosFirst()].rcTile);

	return S_OK;
}

void tankGameScene::release()
{
	SAFE_DELETE(_tankMap);
	SAFE_DELETE(_tank);
}

void tankGameScene::update()
{
	_tank->update();
	_tankMap->update();
}

void tankGameScene::render()
{
	_tankMap->render();
	_tank -> render();
}

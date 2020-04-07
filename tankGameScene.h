#pragma once
#include"gameNode.h"
#include"tankMap.h"
#include"tank.h"
class tankGameScene : public gameNode
{
private:

	tankMap * _tankMap;
	tank* _tank;
public:
	tankGameScene();
	~tankGameScene();

	HRESULT init();
	void release();
	void update();
	void render();
};


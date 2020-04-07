#pragma once
#include"gameNode.h"  
#include"townScene.h"
#include"campScene.h"
#include"mainScene.h"

//#define SUBWINOPEN 1
class mainGame : public gameNode
{
private: 
	image* _mouse;
	int _idx;
	int _count;
public:
	mainGame();
	~mainGame();

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render(/*HDC hdc*/);
};


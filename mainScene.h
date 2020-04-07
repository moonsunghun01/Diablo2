#pragma once
#include "gameNode.h"
#include "townScene.h"
#include "campScene.h"
class mainScene : public gameNode
{
private:
	image* _backGround;
	image* _startButton;
	image* _endButton;
	image* _loading;
	image* _logo;

	RECT _startButtonRc;
	RECT _endButtonRc;

	bool _startButtonClick;
	bool _endButtonClick;

	int _logoIdx;
	int _logoCount;

	int _loadingIdx;
	int _loadingCount;


	bool _isLoading;
public:

	HRESULT init();
	void release();
	void update();
	void render();
};


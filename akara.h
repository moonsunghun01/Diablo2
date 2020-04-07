#pragma once
#include "gameNode.h"
class akara : gameNode
{
private :
	image* _akara;
	image* _akaraShadow;
	RECT _akaraRc;
	RECT _akaraColRc;

	int _akaraIdx;
	int _akaraCount;

public:
	HRESULT init();
	void update();
	void render();
	void shadowRender();

	RECT getAkaraRc() { return _akaraRc; }
	RECT getAkaraColRc() { return _akaraColRc; }
};


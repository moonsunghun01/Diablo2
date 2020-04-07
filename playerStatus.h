#pragma once
#include "gameNode.h"
class playerStatus : gameNode
{
private:
	image* _playerStatus;
	RECT _playerStatusRc;

	RECT _statusCloseButtonRc;
	
	image* _playerStatusPoint;

	image* _statusUp[4];
	RECT _statusUpRc[4];
	char str[256];
public:

	HRESULT init();
	void release();
	void update();
	void render();

	RECT getPlayerStatusRc() { return _playerStatusRc; }
	void setPlayerStatusRc(RECT rc) { _playerStatusRc = rc; }

	RECT getStatusCloseButtonRc() { return _statusCloseButtonRc; }
	RECT getStatusUpRc(int arrNum) { return _statusUpRc[arrNum]; }
};


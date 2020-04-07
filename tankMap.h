#pragma once
#include"gameNode.h"
class tankMap :  public gameNode
{
private:
	tagCurrentTile _currentTile;
	tagTile _map[TILEX* TILEY];
	tagOBJAttribute _OBJattribute[TILEX* TILEY];
	tagSampleTile _sampleMap[SAMPLETILEX* SAMPLETILEY];
	DWORD _attribute[TILEX* TILEY];
	int _pos[2];


public:
	tankMap();
	~tankMap();

	HRESULT init();
	void release();
	void update();
	void render();

	void load();


	DWORD* getAttribute() { return _attribute;}
	tagTile* getMap() { return _map; }
	int getPosFirst() { return _pos[0]; }
	int getPosSecond() { return _pos[1]; }
	void attackBlock(int tileN);
};


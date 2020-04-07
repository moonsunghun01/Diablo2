#include "stdafx.h"
#include "tankMap.h"


tankMap::tankMap()
{
}


tankMap::~tankMap()
{
}

HRESULT tankMap::init()
{
	//맵타일
	IMAGEMANAGER->addFrameImage("mapTiles", "images/mapTiles.bmp", 0, 0, 640, 288, 20, 9, true, RGB(255, 0, 255));

	//맵로드
	load();

	return S_OK;
}

void tankMap::release()
{
}

void tankMap::update()
{
}

void tankMap::render()
{
	//지형
	for (int i = 0; i < TILEX*TILEY; i++)
	{
		IMAGEMANAGER->frameRender("mapTiles", getMemDC(),
			_map[i].rcTile.left, _map[i].rcTile.top,
			_map[i].terrainFrameX, _map[i].terrainFrameY);
	}
	//오브젝트
	for (int i = 0; i < TILEX* TILEY; i++)
	{
		if (_map[i].obj == OBJ_NONE)continue;

		IMAGEMANAGER->frameRender("mapTiles", getMemDC(), _map[i].rcTile.left,
			_map[i].rcTile.top, _map[i].objFrameX, _map[i].objFrameY);
	}

}

void tankMap::load()
{
	HANDLE file;
	DWORD read;

	file = CreateFile("tileMap.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(file, _map, sizeof(tagTile)* TILEX*TILEY, &read, NULL);
	ReadFile(file, _pos, sizeof(int) * 2, &read, NULL);

	//땅크 위치에 해당하는 타일 이미지를 걸러낸다.
	for (int i = 0; i < TILEX* TILEY; i++)
	{
		_OBJattribute[i].strengh = 0;

		if (_map[i].obj == OBJ_TANK1 || _map[i].obj == OBJ_TANK2)
		{
			_map[i].obj = OBJ_NONE;
		}
	}
	//속성정의를 하자.
	memset(_attribute, 0, sizeof(DWORD)*TILEX*TILEY);

	for (int i = 0; i < TILEX*TILEY; i++)
	{
		if (_map[i].terrain == TR_WATER)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
		}
		if (_map[i].obj == OBJ_BLOCK)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
			_OBJattribute[i].strengh = 1;
		}
		if (_map[i].obj == OBJ_BLOCK3)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
			_OBJattribute[i].strengh = 3;
		}
		if (_map[i].obj == OBJ_BLOCKS)
		{
			_attribute[i] |= ATTR_UNMOVABLE;
		}
	}
}

void tankMap::attackBlock(int tileN)
{
	if (_map[tileN].obj == OBJ_BLOCKS)return;

	_OBJattribute[tileN].strengh--;

	if (_OBJattribute[tileN].strengh <= 0)
	{
		_OBJattribute[tileN].strengh = 0;
		_attribute[tileN] = 0;
		_map[tileN].obj = OBJ_NONE;
	}

}

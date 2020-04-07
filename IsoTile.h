#pragma once
//크기
#define CELL_WIDTH 100
#define CELL_HEIGHT 50

#define RADIUS_WIDTH (CELL_WIDTH / 2)
#define RADIUS_HEIGHT (CELL_HEIGHT / 2)

#define PLAYER_SPEED 5
//초기좌표
//#define INIT_X 400
//#define INIT_Y 20
#define INIT_X 425
#define INIT_Y -330
//타일 갯수
#define TILE_COUNT_X 500
#define TILE_COUNT_Y 500

//타일 이미지
#define TILE_SIZE_X 8
#define TILE_SIZE_Y 10


#define TILE_MAX 10
enum TILEKIND
{
	TILEKIND_OBJECT,
	TILEKIND_TERRAIN,
	TILEKIND_ROAD,
	TILEKIND_GOLEMROAD,
	TILEKIND_NONE
};

struct TagTile
{
	int left, top;

	bool block;
	TILEKIND kind;

	int node;
	POINT nodePt;

	int showState;

	int f, g, h;

	int index ;


	int tileNum[TILE_MAX];
	TILEKIND tileKind[TILE_MAX];
	POINT tilePos[TILE_MAX];

	int height;

	int alpha[TILE_MAX];
};
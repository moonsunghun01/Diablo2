#pragma once
#include"gameNode.h"
#include"IsoTile.h"
#include "playerManager.h"
#include"golem.h"  

#include "balog.h"
#include "ogre.h"
#include "anda.h"

#include "akara.h"
#include "shop.h"
enum CAMP_Direction
{
	CAMP_DIRECTION_LEFT,
	CAMP_DIRECTION_RIGHT,
	CAMP_DIRECTION_UP,
	CAMP_DIRECTION_DOWN,
	CAMP_DIRECTION_LEFTUP,
	CAMP_DIRECTION_RIGHTDOWN,
	CAMP_DIRECTION_LEFTDOWN,
	CAMP_DIRECTION_RIGHTUP
};
enum CAMP_State
{
	CAMP_STATE_NONE,
	CAMP_STATE_OPEN,
	CAMP_STATE_CLOSE,
	CAMP_STATE_PATH
};
enum CAMP_Select
{
	CAMP_SELECT_START,
	CAMP_SELECT_END,
	CAMP_SELECT_BLOCK
};
class campScene : public gameNode
{
private:

	//a* 케릭
	TagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];
	vector<int> openList;
	vector<int> closeList;
	vector<int>::iterator iter;
	 

	int startTile;
	int endTile;
	int currentTile;
	bool isFind;
	bool noPath;
	bool startAstar;

	vector<int> _vAStar;

	// a* 소환
	int startGolemTile;
	int endGolemTile;
	int currentGolemTile;
	bool startGolemAstar;
	bool gIsFind;
	bool gNoPath;

	vector<int> _vGolemAstar;
	bool _isFindGolemRoad;

	image* _golemIcon;
	image* _golemHp;
	RECT _golemHpRc; 

	int _isoX, _isoY;
	int _center;
	char str[128];

	int _startX;
	int _startY;

	POINT imageFrame;

	bool _isDebug;
	int _moveXMaxCount;
	int _moveXCount;

	int _moveYMaxCount;
	int _moveYCount;

	// 기본 이미지
	image* _panel;
	RECT _panelRc;
	image* _lifegauge;
	image* _managauge;

	// 스킬 
	image* _leftSkill;
	RECT   _leftSkillRc;
	bool _isLeftSkillCheck;
	int  _leftSkillIndex;

	image* _rightSkill;
	RECT   _rightSkillRc;
	bool _isRightSkillCheck;
	int _rightSkillIndex;

	image* _rightSkillList[5];
	RECT   _rightSkillListRc[5];


	// 타일 이미지
	image* _tilesImage;
	RECT _tilesImageRect;
	RECT _tilesSelectRect[TILE_COUNT_X][TILE_COUNT_Y];

	// 이동 관련
	bool _isMoving;		// 움직임 .
	int _moveXDistance;
	int _moveYDistance;

	// 플레이어
	playerManager* _playerManager;
	POINT _moveEndPoint;

	// 소환 골렘
	golem* _golem;
	 
	// 몬스터 발록
	balog* _balog;
	vector<balog*> _vBalog;

	ogre* _ogre;
	vector<ogre*> _vOgre;

	anda* _anda;
	//상점
	akara* _akara;
	
	shop* _shop;
	bool _isShopOpen;

	// 웨이
	RECT _wayRc;
	image* _potal;
	RECT _potalRc;
	RECT _potalButtonRc;
	RECT _potalCloseRc;
	bool _isOpenWay;

	bool _isChangeScene;
	image* _black;
	image* _loading;

	int _loadingIdx;
	int _loadingCount;
public: 

	HRESULT init();
	void release();
	void update();
	void render();

	//타일맵 그리자.
	void DrawTileMap();
	void DrawObjTile();
	void DrawAlphaObj();

	//마름모를 그리자
	void DrawRhombus(int left, int top);
	//코너검출(1~4분면)
	int GetCornerIndex(int isoX, int isoY);

	//마름모 안에 있음?
	bool IsInRhombus(int corner, int isoX, int isoY);

	void TlieInit();
	void objInit();

	void Astar();
	void golemAstar(); 

	void skillCollisionCheck();
};


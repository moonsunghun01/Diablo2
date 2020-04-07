#pragma once
#include"gameNode.h"
#include"IsoTile.h"
#include "playerManager.h"
#include"golem.h"
#include "balog.h"
#include "ogre.h"
#include "anda.h"
enum CTRL
{
	CTRL_DRAW,
	CTRL_ERASER,
	CTRL_INIT,
	CTRL_SAVE,
	CTRL_LOAD, 
	CTRL_END
};
enum Direction
{
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFTUP,
	DIRECTION_RIGHTDOWN,
	DIRECTION_LEFTDOWN,
	DIRECTION_RIGHTUP
};
enum State
{
	STATE_NONE,
	STATE_OPEN,
	STATE_CLOSE,
	STATE_PATH
};
enum Select
{
	SELECT_START,
	SELECT_END,
	SELECT_BLOCK
};
class townScene : public gameNode
{
private:

	//a* 케릭
	TagTile _tileMap[TILE_COUNT_X][TILE_COUNT_Y];
	vector<int> openList;
	vector<int> closeList;
	vector<int>::iterator iter;

	Select currentSelect;

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
	//////////////////////
	
	// a* 몬스터
	int startMonTile;
	int endMonTile;
	int currentMonTile;
	bool startMonAstar;
	bool mIsFind;
	bool mNoPath;

	vector<int> _vMonAstar;
	bool _isFindMonRoad;

	int _monsterIdx;
	//////////////
	 
	// a* 오우거
	int startOgreTile;
	int endOgreTile;
	int currentOgreTile;
	bool startOgreAstar;
	bool oIsFind;
	bool oNoPath;

	vector<int> _vOgreAstar;
	bool _isFindOgreRoad;

	int _ogreIdx;
	//////////////

	// a* 오우거
	int startAndaTile;
	int endAndaTile;
	int currentAndaTile;
	bool startAndaAstar;
	bool aIsFind;
	bool aNoPath;

	vector<int> _vAndaAstar;
	bool _isFindAndaRoad;

	//////////////

	int _isoX, _isoY;
	int _center;
	char str[128];

	int _startX;
	int _startY;

	POINT imageFrame;
	CTRL _currentCTRL;

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

	
	// 그릴것인지
	bool _isDraw;
	bool _isErase;
	bool _isMove;		// 그리기 x 움직임 버튼

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

	image* _backHp;
	image* _frontHp;

public:
	townScene();
	~townScene();

	HRESULT init();
	void release();
	void update();
	void render();
	void renderOrder();
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

	void balogAstar();
	void balogMove(); 

	void ogreAstar();
	void ogreMove(); 

	void andaAstar();
	void andaMove();

	void skillCollisionCheck();
	void bossSkillCollisionCheck();
};


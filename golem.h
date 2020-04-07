#pragma once
#include "gameNode.h" 
#include "balog.h"
#include "ogre.h"
#include "anda.h"

#define CELL_HEIGHT 50 
#define CELL_WIDTH 100 
#define TILEX 500
#define TILEY 500

enum GOLEM_DIRECTION
{
	GOLEM_DOWN,
	GOLEM_LEFTDOWN,
	GOLEM_LEFT,
	GOLEM_LEFTUP,
	GOLEM_UP,
	GOLEM_RIGHTUP,
	GOLEM_RIGHT,
	GOLEM_RIGHTDOWN, 
	GOLEM_IDLE
};

class golem : gameNode
{
private:
	vector<balog*> _vBalog;
	vector<ogre*> _vOgre;
	anda* _anda;
	// 이동 .. 

	int _moveXDistance;
	int _moveXCount;
	int _moveXMaxCount;

	int _moveYDistance;
	int _moveYCount;
	int _moveYMaxCount;

	bool _isEndPointCheck;

	vector<int> _vAStar;

	bool _isFindRoad;

	// 애니매이션
	GOLEM_DIRECTION _golemDirection;

	int _hp;
	int _atk;
	int _speed;

	int _frameX;
	int _frameY; 
	int _frameCount;

	bool _isSummonComplete;
	
	bool _isMove;
	bool _isMoveEnd;
	float _moveCoolTime;

	int _startX;
	int _startY;

	int _currentX;
	int _currentY;

	int _endX;
	int _endY;

	bool _isAtk;
	float _atkCoolTime;

	image* _golem;
	image* _golemShadow;

	RECT _golemRc;
	RECT _golemColRc;
	RECT _golemAtkRc;

	// 몬스터 찾기
	bool _isFindMoster;
	int _findMonsterIndex;

	bool _isFindOgre;
	int _findOgreIndex;
	
	bool _isFindAnda;
	int _findAndaIndex;

	bool _isHit;
	int _hitIndex;

	bool _isDeath;
	int _deathIndex;

	bool _deathCheck;
public:
	HRESULT init();
	void update();
	void render();
	void shadowRender();

	void animation();

	void rndMove(); 
	void waitMove();

	void findMonster();
	void atkGolem();
	void setIdle();
	void golemHit(int atk, int dir);
	void golemDeath();


	bool getIsEndPointCheck() { return _isEndPointCheck; }
	void setIsEndPointCheck(bool isEndPointCheck) { _isEndPointCheck = isEndPointCheck; }

	vector<int> getAStar() { return _vAStar; }
	void setAstar(vector<int> aStar) { _vAStar = aStar; }

	bool getIsFindRoad() { return _isFindRoad; }
	void setIsFindRoad(bool isFindRoad) { _isFindRoad = isFindRoad; }

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	int getFrameX() { return _frameX; }
	void setFrameX(int frameX) { _frameX = frameX; }

	int getFrameY() { return _frameY; }
	void setFrameY(int frameY) { _frameY = frameY; }

	int getFrameCount() { return _frameCount; }
	void setFrameCount(int frameCount) { _frameCount = frameCount; }

	bool getIsSummonComplte() { return _isSummonComplete; }
	void setIsSummonComplte(bool isSummonComplte) { _isSummonComplete = isSummonComplte; }

	bool getIsMove() { return _isMove; }
	void setIsMove(bool isMove) { _isMove = isMove; }

	bool getIsMoveEnd() { return _isMoveEnd; }
	void setIsMoveEnd(bool isMoveEnd) { _isMoveEnd = isMoveEnd; }

	float getMoveCoolTime() { return _moveCoolTime; }
	void setMoveCoolTime(float moveCoolTime) { _moveCoolTime = moveCoolTime; }

	int getCurrentX() { return _currentX; }
	void setCurrentX(int currentX) { _currentX = currentX; }

	int getCurrentY() { return _currentY; }
	void setCurrentY(int currentY) { _currentY = currentY; }

	int getEndX() { return _endX; }
	void setendX(int endX) { _endX = endX; }

	int getEndY() { return _endY; }
	void setendY(int endY) { _endY = endY; }

	bool getIsAtk() { return _isAtk; }
	void setIsAtk(bool isAtk) { _isAtk = isAtk; }

	float atkCoolTime() { return _atkCoolTime; }
	void setAtkCoolTime(float atkCoolTime) { _atkCoolTime = atkCoolTime; }

	image* getGolem() { return _golem; }
	void setGolem(image* golem) { _golem = golem; }

	image* getGolemShadow() { return _golemShadow; }
	void setGolemShadow(image* golemShadow) { _golemShadow = golemShadow; }

	RECT getGolemRc() { return _golemRc; }
	void setGolemRc(RECT golemRc) { _golemRc = golemRc; }
	
	RECT getGolemColRc() { return _golemColRc; }
	void setGolemColRc(RECT golemColRc) { _golemColRc = golemColRc; }

	bool getIsFindMonster() { return _isFindMoster; }
	void setIsFindMonster(bool isFindMonster) { _isFindMoster = isFindMonster; }

	bool getIsFindOgre() { return _isFindOgre; }
	void setIsFindOgre(bool isFindOgre) { _isFindOgre = isFindOgre; }

	bool getIsFindAnda() { return _isFindAnda; }
	void setIsFindAnda(bool isFindAnda) { _isFindAnda = isFindAnda; }

	void setBalogLink(vector<balog*> vBalog) { _vBalog = vBalog; }
	void setOgreLink(vector<ogre*> vOgre) { _vOgre = vOgre; }
	void setAndaLink(anda* anda) { _anda = anda; }

	int getFindMonsterIndex() { return _findMonsterIndex; }
	void setFindMonsterIndex(int index) { _findMonsterIndex = index; }

	int getFindOgreIndex() { return _findOgreIndex; }
	void setFindOgreIndex(int index) { _findOgreIndex = index; }

	bool getIsDeath() { return _isDeath; }
	void setIsDeath(bool isDeath) { _isDeath = isDeath; }
};


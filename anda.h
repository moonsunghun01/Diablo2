#pragma once
#include "gameNode.h" 

#define CELL_HEIGHT 50 
#define CELL_WIDTH 100 
#define TILEX 500
#define TILEY 500
struct bossSkill
{
	image* image;
	RECT rc;
	float angle;
	int speed;

	int currentX;
	int currentY;
};
enum ANDA_DIRECTION
{
	ANDA_DOWN,
	ANDA_LEFTDOWN,
	ANDA_LEFT,
	ANDA_LEFTUP,
	ANDA_UP,
	ANDA_RIGHTUP,
	ANDA_RIGHT,
	ANDA_RIGHTDOWN,
	ANDA_IDLE
};

class anda : gameNode
{
private:
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
	ANDA_DIRECTION _andaDirection;

	int _hp;
	int _atk;
	int _speed;

	int _frameX;
	int _frameY;
	int _frameCount;
	bool _frameMaxCheck;

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
	bool _isAtkTimeCheck;

	int _atkIdx;
	int _atkCount;

	bool _isIdle;

	bool _isStart;
	image* _anda;
	image* _andaShadow;

	RECT _andaRc;
	RECT _andaColRc;

	bool _isFindTarget;

	// 골렘찾기
	bool _isFindGolem;

	// 맞음
	bool _isHit;
	bool _isDeath;
	int _deathIndex;
	int _deathDir;

	bool _golemHit;

	bool _golemDeathCheck;

	int _skillCount;
	bool _isActSkill;
	int _skillIdx;

	int a;
	bossSkill _bossSkill;
	vector<bossSkill> _vSkill;

	float _skillAngle;
public:
	HRESULT init(int x, int y);
	void update();
	void render();
	void shadowRender();

	void animation();

	void findTarget();
	void andaMove();
	void andaAtk();
	void andaSkill();
	void skillMove();
	
	void checkAtkCoolTime();
	void golemDeath();

	bool getIsEndPointCheck() { return _isEndPointCheck; }
	void setIsEndPointCheck(bool isEndPointCheck) { _isEndPointCheck = isEndPointCheck; }

	void andaHit(int damage);

	vector<bossSkill> getVSkill() { return _vSkill; }
	void setVSkill(vector<bossSkill> vSkill) { _vSkill = vSkill; }

	vector<int> getAStar() { return _vAStar; }
	void setAstar(vector<int> aStar) { _vAStar = aStar; }

	bool getIsFindRoad() { return _isFindRoad; }
	void setIsFindRoad(bool isFindRoad) { _isFindRoad = isFindRoad; }

	int getAndaDirection() { return _andaDirection; }


	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	int getAtk() { return _atk; }
	void setAtk(int atk) { _atk = atk; }

	int getFrameX() { return _frameX; }
	void setFrameX(int frameX) { _frameX = frameX; }

	int getFrameY() { return _frameY; }
	void setFrameY(int frameY) { _frameY = frameY; }

	int getFrameCount() { return _frameCount; }
	void setFrameCount(int frameCount) { _frameCount = frameCount; }

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
	void setEndX(int endX) { _endX = endX; }

	int getEndY() { return _endY; }
	void setEndY(int endY) { _endY = endY; }

	bool getIsAtk() { return _isAtk; }
	void setIsAtk(bool isAtk) { _isAtk = isAtk; }

	float atkCoolTime() { return _atkCoolTime; }
	void setAtkCoolTime(float atkCoolTime) { _atkCoolTime = atkCoolTime; }

	bool getIsAtkTimeCheck() { return _isAtkTimeCheck; }
	void setIsAtkTimeCheck(bool isAtkTimeCheck) { _isAtkTimeCheck = isAtkTimeCheck; }

	bool getIsIdle() { return _isIdle; }
	void setIsIdle(bool isIdle) { _isIdle = isIdle; }

	image* getAnda() { return _anda; }
	void setAnda(image* anda) { _anda = anda; }

	image* getAndaShadow() { return _andaShadow; }
	void setAndaShadow(image* andaShadow) { _andaShadow = andaShadow; }

	RECT getAndaRc() { return _andaRc; }
	void setAndamRc(RECT andaRc) { _andaRc = andaRc; }

	RECT getAndaColRc() { return _andaColRc; }
	void setAndaColRc(RECT andaColRc) { _andaColRc = andaColRc; }

	bool getIsFindTarget() { return _isFindTarget; }
	void setIsFindTarget(bool isFindTarget) { _isFindTarget = isFindTarget; }

	bool getIsFindGolem() { return _isFindGolem; }
	void setIsFindGolem(bool isFindGolem) { _isFindGolem = isFindGolem; }

	bool getIsDeath() { return _isDeath; }
	void setIsDeath(bool isDeath) { _isDeath = isDeath; }

	bool getGolemHit() { return _golemHit; }
	void setGolemHit(bool golemHit) { _golemHit = golemHit; }

	bool getGolemDeathCheck() { return _golemDeathCheck; }
	void setGolemDeathCheck(bool golemDeathCheck) { _golemDeathCheck = golemDeathCheck; }
};


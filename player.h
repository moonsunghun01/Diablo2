#pragma once 
#include "item.h"
class player
{
private:
	// 플레이어 기본정보 
	int _strength;
	int _dex;
	int _life;
	int _mana;

	int _attackDamage;
	int _skillDamage;
	int _attackGrade;

	int _def;
	int _stamina;

	int _hp;
	int _currentHp;
	int _mp;
	int _currentMp;

	int _money;


	bool _isMove;
	int _moveSpeed;

	bool _isHit;
	int _hitDir;

	image* _playerImage;		
	image* _playerShadowImage;	
	RECT _playerRc;				
	RECT _playerColRc;

	POINT _playerRcCenter;

	int _playerStartX;
	int _playerStartY;

	int _playerCurrentX;
	int _playerCurrentY;

	int _playerEndX;
	int _playerEndY;

	//플레이어 스킬
	int _rightSkill;
	int _leftSkill;
	bool _isSummon;
	bool _isSummonAlive;
	int _summonDir;

	int _skillPoint;
	int _statusPoint;
	int _playerLevel;

	// 플레이어 장비
	tagItem _playerEqSd;
	tagItem _playerEqWp;
	tagItem _playerEqHm;

	tagItem _button1Potion;
	tagItem _button2Potion;
	tagItem _button3Potion;
	tagItem _button4Potion;


public:
	HRESULT init();
	void update();
	void playerHit(int atk,int dir);

	int getStrength() { return _strength; }
	void setStrength(int strength) { _strength = strength; }

	int getDex() { return _dex; }
	void setDex(int dex) { _dex = dex; }

	int getLife() { return _life; }
	void setLife(int life) { _life = life; }

	int getMana() { return _mana; }
	void setMana(int mana) { _mana = mana; }

	int getAttackDamage() { return _attackDamage; }
	void setAttackDamage(int attackDamage) { _attackDamage = attackDamage; }

	int getSkillDamage() { return _skillDamage; }
	void setSkillDamange(int SkillDamage) { _skillDamage = SkillDamage; }

	int getAttackGrade() { return _attackGrade; }
	void setAttackGrade(int attackGrade) { _attackGrade = attackGrade; }

	int getDef() { return _def; }
	void setDef(int def) { _def = def; }

	int getStamina() { return _stamina; }
	void setStamina(int stamina) { _stamina = stamina; }

	int getHp() { return _hp; }
	void setHp(int hp) { _hp = hp; }

	int getCurrentHp() { return _currentHp; }
	void setCurrentHp(int currentHp) { _currentHp = currentHp; }

	int getMp() { return _mp; }
	void setMp(int mp) { _mp = mp; }

	int getCurrentMp() { return _currentMp; }
	void setCurrentMp(int currentMp) { _currentMp = currentMp; }

	int getMoney() { return _money; }
	void setMoney(int money) { _money = money; }

	bool getIsMove() { return _isMove; }
	void setIsMove(bool isMove) { _isMove = isMove; }

	int getMoveSpeed() { return _moveSpeed; }
	void setMoveSpeed(int moveSpeed) { _moveSpeed = moveSpeed; }

	bool getIsHit() { return _isHit; }
	void setIsHit(bool isHit) { _isHit = isHit; }

	bool getHitDir() { return _hitDir; }
	void setHitDir(int hitDir) { _hitDir = hitDir; }

	image* getPlayerImage() { return _playerImage; }
	void setPlayerImage(image* playerImage) { _playerImage = playerImage; }
	
	image* getPlayerShadowImage() { return _playerShadowImage; }
	void setPlayerShadowImage(image* playerShadowImage) { _playerShadowImage = playerShadowImage; }

	RECT getPlayerRc() { return _playerRc; }
	void setPlayerRc(RECT playerRc) { _playerRc = playerRc; }

	RECT getPlayerColRc() { return _playerColRc; }
	void setPlayerColRc(RECT playerColRc) { _playerColRc = playerColRc; }

	POINT getPlayerRcCenter()
	{
		_playerRcCenter = { _playerRc.left + (_playerRc.right - _playerRc.left) / 2 ,_playerRc.top + (_playerRc.bottom - _playerRc.top) / 2 };
		return _playerRcCenter;
	}

	int getPlayerStartX() { return _playerStartX; }
	void setPlayerStartX(int playerStartX) { _playerStartX = playerStartX; }

	int getPlayerStartY() { return _playerStartY; }
	void setPlayerStartY(int playerStartY) { _playerStartY = playerStartY; }

	int getPlayerCurrentX() { return _playerCurrentX; }
	void setPlayerCurrentX(int playerCurrentX) { _playerCurrentX = playerCurrentX; }

	int getPlayerCurrentY() { return _playerCurrentY; }
	void setPlayerCurrentY(int playerCurrentY) { _playerCurrentY = playerCurrentY; }

	int getPlayerEndX() { return _playerEndX; }
	void setPlayerEndX(int playerEndX) { _playerEndX = playerEndX; }

	int getPlayerEndY() { return _playerEndY; }
	void setPlayerEndY(int playerEndY) { _playerEndY = playerEndY; }

	int getRightSkill() { return _rightSkill; }
	void setRightSkill(int rightSkill) { _rightSkill = rightSkill; }

	int getLeftSkill() { return _leftSkill; }
	void setLeftSkill(int leftSkill) { _leftSkill = leftSkill; }

	bool getIsSummon() { return _isSummon; }
	void setIsSummon(bool isSummon) { _isSummon = isSummon; }

	bool getIsSummonAlive() { return _isSummonAlive; }
	void setIsSummonAlive(bool isSummonAlive) { _isSummonAlive = isSummonAlive; }

	int getSummonDir() { return _summonDir; }
	void setSummonDir(int summonDir) { _summonDir = summonDir; }

	int getSkillPoint() { return _skillPoint; }
	void setSkillPoint(int skillPoint) { _skillPoint = skillPoint; }

	int getStatusPoint() { return _statusPoint; }
	void setStatusPoint(int statusPoint) { _statusPoint = statusPoint; }

	int getPlayerLevel() { return _playerLevel; }
	void setPlayerLevel(int playerLevel) { _playerLevel = playerLevel; }

	tagItem getPlayerEqSd() { return _playerEqSd; }
	void setPlayerEqSd(tagItem playerEqSd) { _playerEqSd = playerEqSd; }

	tagItem getPlayerEqWp() { return _playerEqWp; }
	void setPlayerEqWp(tagItem playerEqWp) { _playerEqWp = playerEqWp; }

	tagItem getPlayerEqHm() { return _playerEqHm; }
	void setPlayerEqHm(tagItem playerEqHm) { _playerEqHm = playerEqHm; }
	
	tagItem getButton1Potion() {return _button1Potion;}
	void setButton1Potion(tagItem potion) { _button1Potion = potion; }

	tagItem getButton2Potion() { return _button2Potion; }
	void setButton2Potion(tagItem potion) { _button2Potion = potion; }

	tagItem getButton3Potion() { return _button3Potion; }
	void setButton3Potion(tagItem potion) { _button3Potion = potion; }

	tagItem getButton4Potion() { return _button4Potion; }
	void setButton4Potion(tagItem potion) { _button4Potion = potion; }
};


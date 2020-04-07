#pragma once
#include "gameNode.h" 
#include "playerSkillTree.h"
#include "playerStatus.h"
#include "playerInventory.h"
#include "item.h" 
#define CELL_HEIGHT 50 
#define CELL_WIDTH 100 
#define TILEX 500
#define TILEY 500
enum PLAYER_DIRECTION
{
	PLAYER_UP = 0,
	PLAYER_RIGHTUP,
	PLAYER_RIGHT,
	PLAYER_RIGHTDOWN,
	PLAYER_DOWN,
	PLAYER_LEFTDOWN,
	PLAYER_LEFT,
	PLAYER_LEFTUP,
	PLAYER_IDLE
};
enum PLAYER_SKILL_ANGLE
{
	SKILL_ANGLE_DOWN,
	SKILL_ANGLE_LEFTDOWN1,
	SKILL_ANGLE_LEFTDOWN2,
	SKILL_ANGLE_LEFTDOWN3, 
	SKILL_ANGLE_LEFT,
	SKILL_ANGLE_LEFTUP1,
	SKILL_ANGLE_LEFTUP2,
	SKILL_ANGLE_LEFTUP3,
	SKILL_ANGLE_UP,
	SKILL_ANGLE_RIGHTUP1,
	SKILL_ANGLE_RIGHTUP2,
	SKILL_ANGLE_RIGHTUP3,
	SKILL_ANGLE_RIGHT, 
	SKILL_ANGLE_RIGHTDOWN1,
	SKILL_ANGLE_RIGHTDOWN2,
	SKILL_ANGLE_RIGHTDOWN3
};
struct tagSkill
{
	image* skillImage;
	RECT skillRc;
	RECT skillColRc;

	int currentX;
	int currentY;

	float skillAnlgeValue;
	int skillAngle;
	int skillAniCount = 0 ;
	int skillAniIndex = 0;
	
	float skillTime;
};
class playerManager : public gameNode
{
private:
	// �̵� ..
	POINT _moveEndPoint;

	int _moveXDistance;
	int _moveXCount;
	int _moveXMaxCount;

	int _moveYDistance;
	int _moveYCount;
	int _moveYMaxCount;
 
	bool _isEndPointCheck;

	//��ų
	float _skillAngleValue;

	// �ִϸ��̼�
	PLAYER_DIRECTION _playerDirection;
	int _aniCount;
	int _aniIndex;
	int _hitIndex;

	// ��ų
	PLAYER_SKILL_ANGLE _skillAngle;
	bool _isSKillActiveMotion;
	// ��Ƽ��
	tagSkill _tagSkill;
	vector<tagSkill> _vSkill;
	vector<tagSkill> _vSkillTrail;

	// ����
	vector<tagSkill> _vAuraSkill;

	// ��ȯ 
	bool _isGolemSummon;

	//��ųƮ��
	playerSkillTree* _playerSkillTree;
	bool _isOpenSkillTree;
	bool _isClickSkillTreeIcon;
	image* _skillPanelButton;
	RECT _skillPanelButtonRc;

	//���� â
	playerStatus* _playerStatus;
	bool _isOpenStatus;
	bool _isClickStatusUpButton;
	image* _statusPanelButton;
	RECT _statusPanelButtonRc;

	// �κ��丮
	playerInventory* _playerInventory;
	bool _isOpenInven;

	item* _item;
	// ���� �̵�
	bool _isShieldClick;
	bool _isShieldChange;

	// ���� �̵�
	bool _isWeaponClick;
	bool _isWeaponChange;

	// ���� �̵�
	bool _isPositionClick;
	bool _isPositionChange;
	int _potionNum;
	RECT _potionPanelRc[4];

	// A*
	vector<int> _vAStar;
	bool _isLeftCamera;
	bool _isTopCamera;
	// ����� ���. �ɸ��� rect ǥ��
	bool _isDebug;

	char str[256];
	 
	bool _isBuyWeapon;
	bool _isBuyShield;

	bool _isBuyHpPotion;
	bool _isBuyMpPotion;

	image* _mana;
	float _manaCount;
	bool _isManaLcak;


public:
	HRESULT init();
	void release();
	void update();
	void render();
	void shadowRender();

	void playerMove();
	// Ű���� ��ü ����
	void keyManager();
	// �÷��̾� �⺻ �ִϸ��̼� ������
	void animation(); 

	void skillMove(int skillNum);

	void buyWeapon();
	void buyShield();
	void buyMpPotion();
	void buyHpPotiion();

	bool getIsEndPointCheck() { return _isEndPointCheck; }
	void setIsEndPointCheck(bool isEndPointCheck) { _isEndPointCheck = isEndPointCheck; }

	playerSkillTree* getPlayerSkillTree() { return _playerSkillTree; }
	playerStatus* getPlayerStatus() { return _playerStatus; }
	playerInventory* getPlayerInventory() { return _playerInventory; }
	bool getIsOpenInven() { return _isOpenInven; }
	void setIsOpenInven(bool isOpenInven) { _isOpenInven = isOpenInven; }

	vector<tagSkill> getVSkill() { return _vSkill; }
	void setVSkill(vector<tagSkill> vSkill) { _vSkill = vSkill; }

	void setVSkillRc(int idx, RECT rc) { _vSkill.at(idx).skillRc = rc; }

	vector<int> getAStar() { return _vAStar; }
	void setAstar(vector<int> aStar) { _vAStar = aStar; }

	bool getIsBuyWeapon() { return _isBuyWeapon; }
	void setIsBuyWeapon(bool isBuyWeapon) { _isBuyWeapon = isBuyWeapon; }

	bool getIsBuyShield() { return _isBuyShield; }
	void setIsBuyShield(bool isBuyShield) { _isBuyShield = isBuyShield; }

	bool getIsBuyHpPotion() { return _isBuyHpPotion; } 

	bool getIsBuyMpPotion() { return _isBuyMpPotion; } 

	bool getIsWeaponClick() { return _isWeaponClick; }
	void setIsWeaponClick(bool isWeaponClick) { _isWeaponClick = isWeaponClick; }
	
	bool getIsShieldClick() { return _isShieldClick; }
	void setIsShieldClick(bool isShieldClick) { _isShieldClick = isShieldClick; }

	item* getItem() { return _item; }
};


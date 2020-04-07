#pragma once
#include "gameNode.h"
#include "item.h"
struct tagInven
{
	RECT invenRc;
	bool isExist;
};
class playerInventory : gameNode
{
private:
	image* _playerInven;
	RECT _playerInvenRc;

	RECT _invenCloseButtonRc;

	RECT _weaponRc;
	RECT _armorRc;
	RECT _shieldRc;
	RECT _beltRc;
	RECT _gloveRc;
	RECT _shoesRc;
	RECT _helmetRc;

	tagInven _tagInven[40];

	bool _isDebug;

	item* _item;
	char str[256];
	//945,395
public:
	HRESULT init(item* item);
	void release();
	void update(item* item);
	void render();

	RECT getPlayerInvenRc() { return _playerInvenRc; }
	void setPlayerInvenRc(RECT rc) { _playerInvenRc = rc; }

	RECT getPlayerInvenCloseButtonRc() { return _invenCloseButtonRc; }

	RECT getTagInvenRc(int arrNum) { return _tagInven[arrNum].invenRc; }

	RECT getShieldRc() { return _shieldRc; }

	RECT getWeaponRc() { return _weaponRc; }

	bool getTagInvenIsExist(int arrNum) { return _tagInven[arrNum].isExist; }
	void setTagInvenIsExist(int arrNum) { _tagInven[arrNum].isExist = !_tagInven[arrNum].isExist; }

};


#pragma once
#include "gameNode.h"
#include "item.h"
class shop : gameNode
{
private:
	image* _shop;
	RECT _shopRc;
	item* _item;

	RECT _shopCloseButtonRc;

	RECT _weaponRc;
	RECT _shieldRc;
	RECT _mpPotionRc;
	RECT _hpPotionRc;

public:
	HRESULT init();
	void update();
	void render();

	image* getShop() { return _shop; }

	RECT getShopRc() { return _shopRc; }
	void setShopRc(RECT rc) { _shopRc = rc; }

	RECT getShopCloseButtonRc() { return _shopCloseButtonRc; }

	RECT getWeaponRc() { return _weaponRc; }
	RECT getShieldRc() { return _shieldRc; }
	RECT getMpPotionRc() { return _mpPotionRc; }
	RECT getHpPotionRc() { return _hpPotionRc; } 

	void sellWepon();
	void sellShield(); 

	void buyWeapon();
	void buyShield();
};


#pragma once
enum itemKind
{
	WEAPON,
	ARMOR,
	SHIELD,
	BELT,
	POTION,
	EMPTY
};
struct tagItem
{
	image* itemImg;
	RECT itemRc;
	int atk;
	int def;
	int str;
	int dex;
	int life;
	int mana;

	int height;
	int width;
	int index;

	itemKind kind;

	int hp;
	int mp;

	int cost;
}; 
class item
{

private: 
	vector<tagItem> _vPotion; 
	
	tagItem _shield;
	tagItem _weapon;

	tagItem _shopShield;
	tagItem _shopWeapon;
	tagItem _shopHpPotion;
	tagItem _shopMpPotion;

	itemKind _itemKind;
public:
	HRESULT init();
	void update();

	tagItem getShield() { return _shield; }
	void setShield(tagItem shield) { _shield = shield; }
	void setShieldIndex(int index) { _shield.index = index; }

	tagItem getShopShield() { return _shopShield; }
	void setShopShieldIndex(int index) { _shopShield.index = index; }

	tagItem getShopWeapon() { return _shopWeapon; }
	void setShopWeaponIndex(int index) { _shopWeapon.index = index; }
	
	tagItem getShopHpPoton() { return _shopHpPotion; }

	tagItem getShopMpPoton() { return _shopMpPotion; }

	RECT getShieldRc() { return _shield.itemRc; }
	void setShieldRc(POINT mouse) { _shield.itemRc = RectMake(mouse.x -15, mouse.y -15 ,_shield.width * 36, _shield.height * 33); }

	tagItem getWeapon() { return _weapon; }
	RECT getWeaponRc() { return _weapon.itemRc; }
	void setWeapon(tagItem weapon) { _weapon = weapon; }
	void setWeaponIndex(int index) { _weapon.index = index; }
	void setWeaponRc(POINT mouse) { _weapon.itemRc = RectMake(mouse.x -15, mouse.y -15 , _weapon.width * 36, _weapon.height * 33); }

	vector<tagItem> getVPotion() { return _vPotion; }
	void setVPotion(vector<tagItem> vPotion) { _vPotion = vPotion; } 

	void setVPotionRc(int arrNum, POINT mouse) { _vPotion.at(arrNum).itemRc = RectMake(mouse.x - 15, mouse.y - 15, _vPotion.at(arrNum).width * 36, _vPotion.at(arrNum).height * 33); }
	void setVPotionIndex(int arrNum, int index) { _vPotion.at(arrNum).index = index; }

	void deleteItem(int arrNum);

	tagItem getShopHpPotion() { return _shopHpPotion; }

	tagItem getShopMpPotion() { return _shopMpPotion; }
};


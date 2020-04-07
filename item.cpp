#include "stdafx.h"
#include "item.h"

HRESULT item::init()
{
	tagItem _tempItem;
	for (int i = 0; i < 4; i++)
	{ 
		_tempItem.itemImg = IMAGEMANAGER->findImage("hpPotion");
		_tempItem.hp = 100;
		_tempItem.mp = 0;
		_tempItem.width = 1;
		_tempItem.height = 1;
		_tempItem.index = 9 + i * 10;
		_tempItem.kind = POTION;

		_vPotion.push_back(_tempItem); 
	}
	for (int i = 0; i < 4; i++)
	{
		_tempItem.itemImg = IMAGEMANAGER->findImage("mpPotion");
		_tempItem.hp = 0;
		_tempItem.mp = 100;
		_tempItem.width = 1;
		_tempItem.height = 1;
		_tempItem.index = 8 + i * 10;
		_tempItem.kind = POTION;

		_vPotion.push_back(_tempItem);
	}
	_shield.itemImg = IMAGEMANAGER->findImage("shield");
	_shield.atk = 0;
	_shield.def = 10;
	_shield.str = 5;
	_shield.dex = 10;
	_shield.life = 10;
	_shield.mana = 0;
	_shield.height = 3;
	_shield.width = 2;
	_shield.index = -1;
	_shield.kind = SHIELD;
	_shield.cost = 2000;
	
	_weapon.itemImg = IMAGEMANAGER->findImage("weapon");
	_weapon.atk = 10;
	_weapon.def = 0;
	_weapon.str = 5;
	_weapon.dex = 0;
	_weapon.life = 0;
	_weapon.mana = 10;
	_weapon.height = 2;
	_weapon.width = 1;
	_weapon.index = -1;
	_weapon.kind = WEAPON;
	_weapon.cost = 2500;


	_shopShield.itemImg = IMAGEMANAGER->findImage("shield");
	_shopShield.atk = 0;
	_shopShield.def = 10;
	_shopShield.str = 5;
	_shopShield.dex = 10;
	_shopShield.life = 10;
	_shopShield.mana = 0;
	_shopShield.height = 3;
	_shopShield.width = 2;
	_shopShield.index = 0;
	_shopShield.kind = SHIELD;
	_shopShield.cost = 4000;

	_shopWeapon.itemImg = IMAGEMANAGER->findImage("weapon");
	_shopWeapon.atk = 10;
	_shopWeapon.def = 0;
	_shopWeapon.str = 5;
	_shopWeapon.dex = 0;
	_shopWeapon.life = 0;
	_shopWeapon.mana = 10;
	_shopWeapon.height = 2;
	_shopWeapon.width = 1;
	_shopWeapon.index = 0;
	_shopWeapon.kind = WEAPON;
	_shopWeapon.cost = 5000;

	_shopHpPotion.itemImg = IMAGEMANAGER->findImage("hpPotion");
	_shopHpPotion.hp = 100;
	_shopHpPotion.mp = 0;
	_shopHpPotion.width = 1;
	_shopHpPotion.height = 1;
	_shopHpPotion.index = 0;
	_shopHpPotion.cost = 100;

	_shopMpPotion.itemImg = IMAGEMANAGER->findImage("mpPotion");
	_shopMpPotion.hp = 0;
	_shopMpPotion.mp = 100;
	_shopMpPotion.width = 1;
	_shopMpPotion.height = 1;
	_shopMpPotion.index = 0;
	_shopMpPotion.cost = 150;

	return S_OK;
}

void item::update()
{
	if (_shield.index >= 0) _shield.itemRc = RectMake(942 + _shield.index % 10 * 36, 395 + _shield.index / 10 * 33
		, _shield.width * 36, _shield.height * 33);
	if (m_player->getPlayerEqSd().kind == SHIELD) 
		_shield.itemRc = RectMake(1233,170 , _shield.width * 36, _shield.height * 33);

	if (_weapon.index >= 0) _weapon.itemRc = RectMake(942 + _weapon.index % 10 * 36, 395 + _weapon.index / 10 * 33
		, _weapon.width * 36, _weapon.height * 33);
	if (m_player->getPlayerEqWp().kind == WEAPON)
		_weapon.itemRc = RectMake(960, 180, _weapon.width * 36, _weapon.height * 33);

	for (int i = 0; i < _vPotion.size(); i++)
	{ 
		if(_vPotion.at(i).index >= 0)
			_vPotion.at(i).itemRc = RectMake(942 + _vPotion.at(i).index % 10 * 36,
				395 + _vPotion.at(i).index / 10 * 33
				, _vPotion.at(i).width * 36, _vPotion.at(i).height * 33);
	}
}

void item::deleteItem(int arrNum)
{
	_vPotion.erase(_vPotion.begin() + arrNum);
}

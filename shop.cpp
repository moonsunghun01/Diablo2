#include "stdafx.h"
#include "shop.h"

HRESULT shop::init()
{
	_shop = IMAGEMANAGER->findImage("shop");
	_shopCloseButtonRc = RectMake(540, 545, 40, 40);
	 
	_item = new item;
	_item->init();
	  
	return S_OK;
}

void shop::update()
{
	if (_item->getShopWeapon().index == 0) _weaponRc = RectMake(220, 276, 30, 60);
	else _weaponRc = RectMake(0,0,0,0);
	if (_item->getShopShield().index == 0) _shieldRc = RectMake(220, 173, 70, 100);
	else _shieldRc = RectMake(0, 0, 0, 0);
	_hpPotionRc = RectMake(543, 171, 40, 40);
	_mpPotionRc = RectMake(543, 206, 40, 40);
}

void shop::render()
{ 
	_shopRc = RectMake(200, 100, _shop->getWidth(), _shop->getHeight());
	_shop->render(getMemDC(),200,100);
	if (_item->getShopShield().index == 0) _item->getShopShield().itemImg->render(getMemDC(),220,173); 
	if (_item->getShopWeapon().index == 0) _item->getShopWeapon().itemImg->render(getMemDC(),220,276);  

	_item->getShopHpPoton().itemImg->render(getMemDC(), 543, 171);
	_item->getShopMpPoton().itemImg->render(getMemDC(), 543, 206);
	 
}

void shop::sellWepon()
{
	_item->setShopWeaponIndex(-1);
}

void shop::sellShield()
{
	_item->setShopShieldIndex(-1);
}

void shop::buyWeapon()
{
	_item->setShopWeaponIndex(0);
}

void shop::buyShield()
{
	_item->setShopShieldIndex(0);
}
 

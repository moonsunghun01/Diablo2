#include "stdafx.h"
#include "playerInventory.h"

HRESULT playerInventory::init(item* item)
{
	_item = item;

	_playerInven = IMAGEMANAGER->findImage("inven");
	// 기본 설정
	for (int i = 0; i < 40; i++)
	{
		_tagInven[i].invenRc = RectMake(942 + i%10 *36 , 395 + (i/10) * 33, 36, 33);
		_tagInven[i].isExist = false;
	}

	// 물약 
	for (int i = 0; i < _item->getVPotion().size(); i++)
	{
		_tagInven[_item->getVPotion().at(i).index].isExist = true;
	}
	
	_invenCloseButtonRc = RectMake(954, 550, 40, 40);
	return S_OK;
}

void playerInventory::release()
{
}

void playerInventory::update(item* item)
{
	_item = item;
	if (KEYMANAGER->isOnceKeyDown('M')) _isDebug = !_isDebug;
}

void playerInventory::render()
{
	_playerInvenRc = RectMake(920, 100, _playerInven->getWidth(), _playerInven->getHeight());
	_playerInven->render(getMemDC(), 920, 100);
	_weaponRc = RectMake(940,150,80,135);
	_armorRc = RectMake(1083,185,80,105);
	_shieldRc = RectMake(1225,150,80,135);
	_helmetRc = RectMake(1083,100,75,75);
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	sprintf_s(str, "%d",m_player->getMoney()); TextOut(getMemDC(), 1110, 556, str, strlen(str));
	// 방패 그리기
	if(_item->getShield().index >= 0)_item->getShield().itemImg->render(getMemDC(), _item->getShield().itemRc.left , _item->getShield().itemRc.top);
	// 무기 그리기
	if (_item->getWeapon().index >= 0)_item->getWeapon().itemImg->render(getMemDC(), _item->getWeapon().itemRc.left , _item->getWeapon().itemRc.top);
	// 물약 그리기
	for (int i = 0; i < _item->getVPotion().size(); i++)
	{
		 _item->getVPotion().at(i).itemImg->render(getMemDC(), _item->getVPotion().at(i).itemRc.left, _item->getVPotion().at(i).itemRc.top);	
	}

	if (_isDebug)
	{
		for (int i = 0; i < 40; i++) Rectangle(getMemDC(), _tagInven[i].invenRc.left, _tagInven[i].invenRc.top, _tagInven[i].invenRc.right, _tagInven[i].invenRc.bottom);
		Rectangle(getMemDC(), _weaponRc.left, _weaponRc.top, _weaponRc.right, _weaponRc.bottom);
		Rectangle(getMemDC(), _armorRc.left, _armorRc.top, _armorRc.right, _armorRc.bottom);
		Rectangle(getMemDC(), _shieldRc.left, _shieldRc.top, _shieldRc.right, _shieldRc.bottom);
		Rectangle(getMemDC(), _helmetRc.left, _helmetRc.top, _helmetRc.right, _helmetRc.bottom);
		Rectangle(getMemDC(), _item->getShield().itemRc.left, _item->getShield().itemRc.top, _item->getShield().itemRc.right, _item->getShield().itemRc.bottom);
		Rectangle(getMemDC(), _item->getWeapon().itemRc.left, _item->getWeapon().itemRc.top, _item->getWeapon().itemRc.right, _item->getWeapon().itemRc.bottom);
	}
}

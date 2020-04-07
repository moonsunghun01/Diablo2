#include "stdafx.h"
#include "playerStatus.h"

HRESULT playerStatus::init()
{
	_playerStatus = IMAGEMANAGER->findImage("status");
	
	_playerStatusPoint = IMAGEMANAGER->findImage("statusPoint");
	_statusCloseButtonRc = RectMake(360, 550, 40, 40);

	for (int i = 0; i < 4; i ++ )
	{
		_statusUp[i] = IMAGEMANAGER->findImage("statusUpButton");
	}
	_statusUpRc[0] = RectMake(358,188,40,40);
	_statusUpRc[1] = RectMake(358,258,40,40);
	_statusUpRc[2] = RectMake(358,358,40,40);
	_statusUpRc[3] = RectMake(358,432,40,40);
	return S_OK;
}

void playerStatus::release()
{
}

void playerStatus::update()
{
}

void playerStatus::render()
{
	_playerStatusRc = RectMake(200, 100, _playerStatus->getWidth(), _playerStatus->getHeight());

	_playerStatus->render(getMemDC(),200,100);
	 SetTextColor(getMemDC(), RGB(255, 255, 255));
	if (m_player->getStatusPoint() > 0)
	{
		_playerStatusPoint->render(getMemDC(), 200, 500); 
		sprintf_s(str, "남은 능력치"); TextOut(getMemDC(), 215,508, str, strlen(str));
		sprintf_s(str, "%d",m_player->getStatusPoint()); TextOut(getMemDC(), 332,508, str, strlen(str));
		for (int i = 0; i < 4; i++) _statusUp[i]->render(getMemDC(), _statusUpRc[i].left, _statusUpRc[i].top);
	}

	sprintf_s(str, " 문 성 훈"); TextOut(getMemDC(), 300, 113, str, strlen(str));
	sprintf_s(str, " 네크로멘서"); TextOut(getMemDC(), 475, 113, str, strlen(str));
	sprintf_s(str, " 레 벨"); TextOut(getMemDC(), 220, 140, str, strlen(str));
	sprintf_s(str, " %d",m_player->getPlayerLevel()); TextOut(getMemDC(), 235, 157, str, strlen(str));
	sprintf_s(str, " 경 험 치"); TextOut(getMemDC(), 320, 140, str, strlen(str));
	sprintf_s(str, " 500"); TextOut(getMemDC(), 335, 157, str, strlen(str));
	sprintf_s(str, " 다 음 레 벨 "); TextOut(getMemDC(), 470, 140, str, strlen(str));
	sprintf_s(str, " 700"); TextOut(getMemDC(), 500, 157, str, strlen(str));
	sprintf_s(str, " 힘 "); TextOut(getMemDC(), 240, 200, str, strlen(str));
	sprintf_s(str, " %d ",m_player->getStrength()); TextOut(getMemDC(), 308, 198, str, strlen(str));
	sprintf_s(str, " 민첩성 "); TextOut(getMemDC(), 226, 270, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getDex()); TextOut(getMemDC(), 308, 270, str, strlen(str));
	sprintf_s(str, " 생명력 "); TextOut(getMemDC(), 226, 370, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getLife()); TextOut(getMemDC(), 308, 370, str, strlen(str));
	sprintf_s(str, " 에너지 "); TextOut(getMemDC(), 226, 442, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getMana()); TextOut(getMemDC(), 308, 442, str, strlen(str));
	sprintf_s(str, " 공격 데미지"); TextOut(getMemDC(), 420, 200, str, strlen(str));
	sprintf_s(str, " 스킬 데미지"); TextOut(getMemDC(), 420, 226, str, strlen(str));
	sprintf_s(str, " 공격 등급 "); TextOut(getMemDC(), 427, 270, str, strlen(str));
	sprintf_s(str, " %d",m_player->getAttackGrade()); TextOut(getMemDC(), 550, 270, str, strlen(str));
	sprintf_s(str, " 방 어 "); TextOut(getMemDC(), 445, 325, str, strlen(str));
	sprintf_s(str, " %d",m_player->getDef()); TextOut(getMemDC(), 547, 325, str, strlen(str));
	sprintf_s(str, " 스테미나 "); TextOut(getMemDC(), 408, 370, str, strlen(str));
	sprintf_s(str, " %d " , m_player->getStamina()); TextOut(getMemDC(), 500, 370, str, strlen(str));
	sprintf_s(str, " %d " , m_player->getStamina()); TextOut(getMemDC(), 550, 370, str, strlen(str));
	sprintf_s(str, " 라이프 "); TextOut(getMemDC(), 418, 397, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getHp()); TextOut(getMemDC(), 500, 397, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getCurrentHp()); TextOut(getMemDC(), 550, 397, str, strlen(str));
	sprintf_s(str, " 마 나 "); TextOut(getMemDC(), 423, 442, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getMp()); TextOut(getMemDC(), 500, 442, str, strlen(str));
	sprintf_s(str, " %d ", m_player->getCurrentMp()); TextOut(getMemDC(), 550, 442, str, strlen(str));
	sprintf_s(str, " 파이어 저항력 "); TextOut(getMemDC(), 422, 486, str, strlen(str));
	sprintf_s(str, " 0 "); TextOut(getMemDC(), 557, 486, str, strlen(str));
	sprintf_s(str, " 콜드 저항력 "); TextOut(getMemDC(), 430, 515, str, strlen(str));
	sprintf_s(str, " 0 "); TextOut(getMemDC(), 557, 515, str, strlen(str));
	sprintf_s(str, " 라이트닝 저항력 "); TextOut(getMemDC(), 418, 542, str, strlen(str));
	sprintf_s(str, " 0 "); TextOut(getMemDC(), 557, 542, str, strlen(str));
	sprintf_s(str, " 포이즌 저항력 "); TextOut(getMemDC(), 422, 570, str, strlen(str));
	sprintf_s(str, " 0 "); TextOut(getMemDC(), 557, 570, str, strlen(str));
}

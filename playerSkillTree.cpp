#include "stdafx.h"
#include "playerSkillTree.h"

HRESULT playerSkillTree::init()
{
	_defaultSkillTree = IMAGEMANAGER->findImage("default_skill_tree");
	_defaultSkillFont = IMAGEMANAGER->findImage("default_skill_font");

	_boneSkillTreeBackImage = IMAGEMANAGER->findImage("bone_skill_tree");
	_boneSkillTreeFont = IMAGEMANAGER->findImage("poison_bone_skill_font");

	_summonSkillTreeBackImage = IMAGEMANAGER->findImage("summon_skill_tree");
	_summonSkillTreeFont = IMAGEMANAGER->findImage("summon_skill_font");

	_curseSkillTreeBackImage = IMAGEMANAGER->findImage("curse_skill_tree");
	_curseSkillTreeFont = IMAGEMANAGER->findImage("curse_skill_font");

	_skillTreeNum = 1;
	for(int i =0 ; i<3; i++)_skillTreeNumRc[i] = RectMake(1220,230 + i * 120,100,120);
	skillIconSetting();
	if (_skillTreeNum == 1) _skillTreeCloseButton = RectMake(1150,544,45,45);
	return S_OK;
}

void playerSkillTree::release()
{
}

void playerSkillTree::update()
{
	skillIconSetting();
}

void playerSkillTree::render()
{
	_defaultSkillTreeRc = RectMake(920, 100, _defaultSkillTree->getWidth(), _defaultSkillTree->getHeight());
	SetTextColor(getMemDC(), RGB(255, 255, 255));
	//Rectangle(getMemDC(),_defaultSkillTreeRc.left, _defaultSkillTreeRc.top, _defaultSkillTreeRc.right, _defaultSkillTreeRc.bottom);
	_defaultSkillTree->render(getMemDC(), _defaultSkillTreeRc.left, _defaultSkillTreeRc.top);
	if(_skillTreeNum == 0)_summonSkillTreeBackImage->render(getMemDC(), _defaultSkillTreeRc.left, _defaultSkillTreeRc.top);
	if (_skillTreeNum == 1)
	{
		_boneSkillTreeBackImage->render(getMemDC(), _defaultSkillTreeRc.left, _defaultSkillTreeRc.top);
		for (int i = 0; i < 10; i++)
		{
			_boneSkillTreeIcon[i].skillIcon->render(getMemDC(), _boneSkillTreeIcon[i].skillIconRc.left, _boneSkillTreeIcon[i].skillIconRc.top);
			// ¸ø¹è¿ò ?
			if (m_player->getPlayerLevel() < _boneSkillTreeIcon[i].learnLevel || m_player->getSkillPoint() <= 0 || _boneSkillTreeIcon[_boneSkillTreeIcon[i].preSkillNum].skillLevel <= 0 || _boneSkillTreeIcon[i].skillLevel >= 20)
			{
				_boneSkillTreeIcon[i].skillIconAlpha->alphaRender(getMemDC(), _boneSkillTreeIcon[i].skillIconRc.left, _boneSkillTreeIcon[i].skillIconRc.top, 100);
			}
			
			sprintf_s(str, " %d ", _boneSkillTreeIcon[i].skillLevel);
			if(_boneSkillTreeIcon[i].skillLevel >0)
			TextOut(getMemDC(), _boneSkillTreeIcon[i].skillIconRc.left + 62, _boneSkillTreeIcon[i].skillIconRc.top + 52, str, strlen(str));
		}
	}
	if(_skillTreeNum == 2)_curseSkillTreeBackImage->render(getMemDC(), _defaultSkillTreeRc.left, _defaultSkillTreeRc.top);
	//for (int i = 0; i < 3; i++) Rectangle(getMemDC(), _skillTreeNumRc[i].left, _skillTreeNumRc[i].top, _skillTreeNumRc[i].right, _skillTreeNumRc[i].bottom);
	sprintf_s(str, " %d ", m_player->getSkillPoint());
	TextOut(getMemDC(), 1255, 177, str, strlen(str));

	_defaultSkillFont->render(getMemDC(), 1248,115);
	_summonSkillTreeFont->render(getMemDC(), 1252,265);
	_boneSkillTreeFont->render(getMemDC(), 1255,380);
	_curseSkillTreeFont->render(getMemDC(), 1255,530);
}

void playerSkillTree::skillIconSetting()
{
	_boneSkillTreeIcon[0].skillIcon = IMAGEMANAGER->findImage("1_icon");
	_boneSkillTreeIcon[0].skillIconRc = RectMake(1033, 117, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[0].learnLevel = 0;
	_boneSkillTreeIcon[0].skillLevel = 1;
	_boneSkillTreeIcon[0].preSkillNum = 0;

	_boneSkillTreeIcon[1].skillIcon = IMAGEMANAGER->findImage("2_icon");
	_boneSkillTreeIcon[1].skillIconRc = RectMake(1125, 117, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[1].learnLevel = 0;
	_boneSkillTreeIcon[1].preSkillNum = 0;

	_boneSkillTreeIcon[2].skillIcon = IMAGEMANAGER->findImage("3_icon");
	_boneSkillTreeIcon[2].skillIconRc = RectMake(940, 195, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[2].learnLevel = 6;
	_boneSkillTreeIcon[2].preSkillNum = 0;

	_boneSkillTreeIcon[3].skillIcon = IMAGEMANAGER->findImage("4_icon");
	_boneSkillTreeIcon[3].skillIconRc = RectMake(1033, 195, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[3].learnLevel = 6;
	_boneSkillTreeIcon[3].preSkillNum = 0;

	_boneSkillTreeIcon[4].skillIcon = IMAGEMANAGER->findImage("5_icon");
	_boneSkillTreeIcon[4].skillIconRc = RectMake(1125, 274, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[4].learnLevel = 12;
	_boneSkillTreeIcon[4].preSkillNum = 1;

	_boneSkillTreeIcon[5].skillIcon = IMAGEMANAGER->findImage("6_icon");
	_boneSkillTreeIcon[5].skillIconRc = RectMake(940, 353, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[5].learnLevel = 18;
	_boneSkillTreeIcon[5].preSkillNum = 2;

	_boneSkillTreeIcon[6].skillIcon = IMAGEMANAGER->findImage("7_icon");
	_boneSkillTreeIcon[6].skillIconRc = RectMake(1033, 353, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[6].learnLevel = 18;
	_boneSkillTreeIcon[6].preSkillNum = 3;

	_boneSkillTreeIcon[7].skillIcon = IMAGEMANAGER->findImage("8_icon");
	_boneSkillTreeIcon[7].skillIconRc = RectMake(1125, 433, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[7].learnLevel = 24;
	_boneSkillTreeIcon[7].preSkillNum = 4;

	_boneSkillTreeIcon[8].skillIcon = IMAGEMANAGER->findImage("9_icon");
	_boneSkillTreeIcon[8].skillIconRc = RectMake(940, 510, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[8].learnLevel = 30;
	_boneSkillTreeIcon[8].preSkillNum = 5;

	_boneSkillTreeIcon[9].skillIcon = IMAGEMANAGER->findImage("10_icon");
	_boneSkillTreeIcon[9].skillIconRc = RectMake(1033, 510, _boneSkillTreeIcon[0].skillIcon->getWidth(), _boneSkillTreeIcon[0].skillIcon->getHeight());
	_boneSkillTreeIcon[9].learnLevel = 30;
	_boneSkillTreeIcon[9].preSkillNum = 6;

	for (int i = 0; i < 10; i++)
	{
		_boneSkillTreeIcon[i].skillIconAlpha= IMAGEMANAGER->findImage("blackIcon");
	}
}

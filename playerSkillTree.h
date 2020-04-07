#pragma once
#include "gameNode.h"
struct skillTreeIcon
{
	image* skillIcon;
	RECT skillIconRc;
	image* skillIconAlpha;
	bool isLearn = false;
	int preSkillNum;
	int learnLevel;				// �����ִ� �÷��̾� ����
	int skillLevel = 0;			// ��ų ���� ���� 
	
};
class playerSkillTree : gameNode
{
private:
	// ���
	image* _defaultSkillTree;
	RECT _defaultSkillTreeRc;
	image* _defaultSkillFont;

	// �� ��ų ( ���� )
	image* _boneSkillTreeBackImage;
	RECT _boneSkillTreeRc;
	image* _boneSkillTreeFont;

	// ��ȯ ��ų (�̹�����)
	image* _summonSkillTreeBackImage;
	RECT _summonSkillTreeRc;
	image* _summonSkillTreeFont;

	// ���� ��ų (�̹�����)
	image* _curseSkillTreeBackImage;
	RECT _curseSkillTreeRc;
	image* _curseSkillTreeFont; 

	// ��ų ���� 
	RECT _skillTreeNumRc[3];
	int _skillTreeNum;

	skillTreeIcon _summonSkillTreeIcon[9];
	skillTreeIcon _boneSkillTreeIcon[10];
	skillTreeIcon _curseSkillTreeIcon[10];

	RECT _skillTreeCloseButton;

	char str[256];
public:
	HRESULT init();
	void release();
	void update();
	void render();
	
	void skillIconSetting();

	RECT getDefaultSkillTreeRc() { return _defaultSkillTreeRc; }
	void setDefaultSkillTreeRc(RECT rc) { _defaultSkillTreeRc = rc; }

	RECT getSkillTreeNumRc(int num) { return _skillTreeNumRc[num]; }

	int getSkillTreeNum() { return _skillTreeNum; }
	void setSkillTreeNum(int skillTreeNum) { _skillTreeNum = skillTreeNum; }

	RECT getBoneSkillTreeIconRc(int arrNum) { return _boneSkillTreeIcon[arrNum].skillIconRc; }
	
	int getBoneSkillTreeIconLearnLevel(int arrNum) { return _boneSkillTreeIcon[arrNum].learnLevel; }
	
	int getBoneSkillTreeIconPreSkillNum(int arrNum) { return _boneSkillTreeIcon[arrNum].preSkillNum; }

	int getBoneSkillTreeIconSkillLevel(int arrNum) { return _boneSkillTreeIcon[arrNum].skillLevel; }
	void setBoneSkillTreeIconSkillLevel(int arrNum,int skillLevel) { _boneSkillTreeIcon[arrNum].skillLevel = skillLevel; }
	
	RECT getSkillTreeCloseButton() { return _skillTreeCloseButton; }
};


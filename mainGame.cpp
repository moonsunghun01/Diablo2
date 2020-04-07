#include "stdafx.h"
#include "mainGame.h"


mainGame::mainGame()
{
}


mainGame::~mainGame()
{
}

HRESULT mainGame::init()
{
	gameNode::init(true);  
	IMAGEMANAGER->addFrameImage("cursor", "images/diablo/cursor.bmp", 248 , 27 , 8, 1, true, RGB(0, 0, 0));
	_mouse = IMAGEMANAGER->findImage("cursor");
	//나는 바보 멍청이다
	// 씬
	IMAGEMANAGER->addImage	   ("main",			"images/diablo/scene/main.bmp",1000, WINSIZEY, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("startButton", "images/diablo/scene/playButton.bmp", 270 , 70 , 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("endButton", "images/diablo/scene/exitButton.bmp", 270 , 70 , 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("loading", "images/diablo/scene/portal.bmp", 2560 , 256 , 10, 1, true, RGB(255, 0, 255)); 
	IMAGEMANAGER->addFrameImage("logo", "images/diablo/scene/logo.bmp", 550 , 3240 , 1, 15, true, RGB(0, 255, 0)); 

	// 케릭터
	IMAGEMANAGER->addFrameImage("necro_idle", "images/diablo/char/necro_idle_8.bmp", 688 , 632 , 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_idle_shadow", "images/diablo/char/necro_idle_shadow.bmp", 688 , 632, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_run", "images/diablo/char/necro_run_8.bmp", 608 , 664 , 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_run_shadow", "images/diablo/char/necro_run_shadow.bmp", 608 , 664, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_skill", "images/diablo/char/necro_skill.bmp", 1488 , 1392 , 16, 16, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_skill_shadow", "images/diablo/char/necro_skill_shadow.bmp", 1488 , 1392 , 16, 16, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_hit", "images/diablo/char/necro_hit.bmp", 602, 640, 7, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("necro_hit_shadow", "images/diablo/char/necro_hit_shadow.bmp", 602, 640, 7, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("mana", "images/diablo/char/mana.bmp", 100, 25, true, RGB(255, 0, 255));

	// 몬스터
	// 발록 
	IMAGEMANAGER->addFrameImage("balog_idle", "images/diablo/monster/balog/Balog_Idle.bmp", 2415 , 1615 , 8 ,8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_idle_shadow", "images/diablo/monster/balog/Balog_Idle_shadow.bmp", 2912 , 2720 , 8 ,8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_move", "images/diablo/monster/balog/Balog_Walk.bmp", 2415, 1616, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_move_shadow", "images/diablo/monster/balog/Balog_Walk_shadow.bmp", 2912, 2720, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_atk", "images/diablo/monster/balog/Balog_Attack.bmp", 4813, 1616, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_atk_shadow", "images/diablo/monster/balog/Balog_Attack_shadow.bmp", 5824, 2720, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_hit", "images/diablo/monster/balog/Balog_GetHit.bmp", 1811, 1616, 6, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_hit_shadow", "images/diablo/monster/balog/Balog_GetHit_shadow.bmp", 2184, 2720, 6, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_die", "images/diablo/monster/balog/Balog_Dying.bmp", 6040, 1616, 20, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("balog_die_shadow", "images/diablo/monster/balog/Balog_Dying_shadow.bmp", 7280, 2720, 20, 8, true, RGB(255, 0, 255));
	
	//오우거
	IMAGEMANAGER->addFrameImage("ogre_idle",		 "images/diablo/monster/ogre/Ogre_Idle.bmp", 1064, 1040, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_idle_shadow", "images/diablo/monster/ogre/Ogre_Idle_shadow.bmp", 1464, 1472, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_move",		 "images/diablo/monster/ogre/Ogre_Walk.bmp", 1256, 1128, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_move_shadow", "images/diablo/monster/ogre/Ogre_Walk_shadow.bmp", 1672, 1656, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_atk",		 "images/diablo/monster/ogre/Ogre_Attack.bmp", 3300, 1728, 15, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_atk_shadow",  "images/diablo/monster/ogre/Ogre_Attack_shadow.bmp", 4560, 2440, 15, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_hit",		 "images/diablo/monster/ogre/Ogre_GetHit.bmp", 1239, 1304, 7, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_hit_shadow",  "images/diablo/monster/ogre/Ogre_GetHit_shadow.bmp", 1673, 1896, 7, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_die",		 "images/diablo/monster/ogre/Ogre_Dying.bmp", 7475, 1644, 23, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("ogre_die_shadow",  "images/diablo/monster/ogre/Ogre_Dying_shadow.bmp", 8855, 2848, 23, 8, true, RGB(255, 0, 255));

	// 안다리엘
	IMAGEMANAGER->addFrameImage("anda_idle",  "images/diablo/monster/anda/anda_idle.bmp", 2847, 1430, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_idle_shadow",  "images/diablo/monster/anda/anda_idle_shadow.bmp", 2847, 1430, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_move",  "images/diablo/monster/anda/anda_move.bmp", 1776, 1471, 12, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_move_shadow",  "images/diablo/monster/anda/anda_move_shadow.bmp", 1776, 1471, 12, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_atk",  "images/diablo/monster/anda/anda_atk.bmp", 3952, 1980, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_atk_shadow",  "images/diablo/monster/anda/anda_atk_shadow.bmp", 3952, 1980, 16, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_hit",  "images/diablo/monster/anda/anda_hit.bmp", 1038, 1440, 6, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_hit_shadow",  "images/diablo/monster/anda/anda_hit_shadow.bmp", 1038, 1440, 6, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_die",  "images/diablo/monster/anda/anda_die.bmp", 4669, 1855, 23, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_die_fire",  "images/diablo/monster/anda/anda_die_fire.bmp", 2346, 1296, 23, 8, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("anda_skill",  "images/diablo/monster/anda/anda_skill.bmp", 3816, 1455, 18, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("anda_skill_shadow",  "images/diablo/monster/anda/anda_skill_shadow.bmp", 3816, 1455, 18, 8, true, RGB(255, 0, 255));

	IMAGEMANAGER->addFrameImage("anda_active_skill",  "images/diablo/monster/anda/skill.bmp", 1466, 863, 9, 8, true, RGB(0, 0, 0));

	IMAGEMANAGER->addImage("backHp", "images/diablo/monster/backHp.bmp", 200, 30, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("frontHp", "images/diablo/monster/frontHp.bmp", 200, 30, true, RGB(255, 0, 255));


	//스킬
	IMAGEMANAGER->addFrameImage("bone_spear", "images/diablo/skill/bone.bmp", 480 *1.2, 912 *1.2, 6, 16, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("bone_trail", "images/diablo/skill/bone_trail.bmp", 648 * 1.2, 1232 * 1.2, 9, 16, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("bone_shield", "images/diablo/skill/bone_shield.bmp", 1224 * 1.2, 126 * 1.2, 12, 2, true, RGB(0, 0, 0));
	
	// 소환 스킬 (골렘)
	IMAGEMANAGER->addImage	   ("fire_golem_icon",			"images/diablo/monster/fire_golem/fire_golem_icon.bmp", 46*1.5, 41*1.5, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage	   ("fire_golem_hp",			"images/diablo/monster/fire_golem/fire_golem_hp.bmp", 46*1.5, 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage	   ("fire_golem_hp2",			"images/diablo/monster/fire_golem/fire_golem_hp2.bmp", 46*1.5, 10, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("fire_golem_summon",		"images/diablo/monster/fire_golem/fire_golem_summon.bmp", 3344 * 1.2, 135 * 1.2, 16, 1, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("fire_golem_idle",			"images/diablo/monster/fire_golem/fire_golem_idle.bmp", 448 * 1.2, 735 * 1.2, 8, 8, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("fire_golem_idle_shadow",	"images/diablo/monster/fire_golem/fire_golem_idle_shadow.bmp", 448 * 1.2, 735 * 1.2, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("fire_golem_move",			"images/diablo/monster/fire_golem/fire_golem_move.bmp", 584 * 1.2, 784 * 1.2, 8, 8, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("fire_golem_move_shadow",	"images/diablo/monster/fire_golem/fire_golem_move_shadow.bmp", 584 * 1.2, 784 * 1.2, 8, 8, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("fire_golem_atk",			"images/diablo/monster/fire_golem/fire_golem_atk.bmp", 2278 * 1.2, 847 * 1.2, 17, 8, true, RGB(0, 0, 0));
	IMAGEMANAGER->addFrameImage("fire_golem_atk_shadow",	"images/diablo/monster/fire_golem/fire_golem_atk_shadow.bmp", 2278 * 1.2, 847 * 1.2, 17, 8, true, RGB(255, 0,255));
	IMAGEMANAGER->addFrameImage("fire_golem_death",			"images/diablo/monster/fire_golem/fire_golem_death.bmp", 135 * 19 * 1.2, 135 *1.2, 19, 1, true, RGB(0, 0,0));


	// 기본 이미지
	IMAGEMANAGER->addImage("panel", "images/diablo/panel/Panel.BMP", 1200, 153, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("lifegauge", "images/diablo/panel/Lifegauge.BMP", 115, 115, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("managauge", "images/diablo/panel/Managauge.BMP", 125, 125, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("levelup", "images/diablo/panel/Levelup.BMP", 90, 30,3,1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("skill_panel", "images/diablo/panel/skill_panel.BMP", 365, 70,5,1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("skillPanelButton", "images/diablo/button/statusPointButton.BMP", 40, 40, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("statusPanelButton", "images/diablo/button/statusPointButton.BMP", 40, 40, true, RGB(0, 0, 0));

	// 타일
	IMAGEMANAGER->addFrameImage("act1Tiles2", "images/diablo/tiles/act1Tiles2.bmp", 400, 400, 100 * 5, (50) * 12, 5, 12, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("act1Wall3", "images/diablo/tiles/act1Wall5.bmp", 400, 400, 200 * 6, (400) * 8, 6, 8, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addFrameImage("townTile", "images/diablo/tiles/townTiles.bmp", 400, 400, 100 * 5, (50) * 45, 5, 45, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("townWall", "images/diablo/tiles/townWall.bmp", 400, 400, 200 * 31, (400) * 3, 31, 3, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("townWall2", "images/diablo/tiles/townWall2.bmp", 400, 400, 200 * 20, (400) * 1, 20, 1, true, RGB(255, 0, 255));
	 
	// 스킬 트리 창 
	IMAGEMANAGER->addImage("default_skill_tree", "images/diablo/panel/default_skill_tree.BMP", 400, 500, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("bone_skill_tree", "images/diablo/panel/bone_skill_tree.BMP", 400, 500, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("summon_skill_tree", "images/diablo/panel/summon_skill_tree.BMP", 400, 500, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("curse_skill_tree", "images/diablo/panel/curse_skill_tree.BMP", 400, 500, true, RGB(0, 0, 0));

	// 스킬 트리 아이콘
	IMAGEMANAGER->addImage("1_icon", "images/diablo/skill/icon/1_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("2_icon", "images/diablo/skill/icon/2_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("3_icon", "images/diablo/skill/icon/3_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("4_icon", "images/diablo/skill/icon/4_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("5_icon", "images/diablo/skill/icon/5_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("6_icon", "images/diablo/skill/icon/6_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("7_icon", "images/diablo/skill/icon/7_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("8_icon", "images/diablo/skill/icon/8_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("9_icon", "images/diablo/skill/icon/9_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("10_icon", "images/diablo/skill/icon/10_icon.BMP", 63, 55, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("blackIcon", "images/diablo/skill/icon/blackIcon.BMP", 63, 55, true, RGB(255, 0, 255));

	// 스텟 창 
	IMAGEMANAGER->addImage("status", "images/diablo/panel/status.BMP", 400, 500, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("statusPoint", "images/diablo/panel/statusPoint.BMP", 170, 30, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("statusUpButton", "images/diablo/button/statusPointButton.BMP", 40, 40, true, RGB(0, 0, 0));

	// 인벤토리 창 
	IMAGEMANAGER->addImage("inven", "images/diablo/panel/inventory.BMP", 400, 500, true, RGB(0, 0, 0));

	IMAGEMANAGER->addImage("way", "images/diablo/panel/potal.BMP", 400, 500, true, RGB(0, 0, 0));
	// 아이템
	IMAGEMANAGER->addImage("hpPotion", "images/diablo/item/hpPotion.bmp", 36, 33, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("mpPotion", "images/diablo/item/mpPotion.bmp", 36, 33, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("shield", "images/diablo/item/shield.bmp", 36 * 2, 33 * 3, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("weapon", "images/diablo/item/weapon.bmp", 36, 33 * 2, true, RGB(0, 0, 0));
	IMAGEMANAGER->addImage("weapon2", "images/diablo/item/weapon2.bmp", 36, 33 * 2, true, RGB(0, 0, 0));

	IMAGEMANAGER->addImage("mpPotionInfo", "images/diablo/item/mpPotionInfo.bmp", 150, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("hpPotionInfo", "images/diablo/item/hpPotionInfo.bmp", 150, 50, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("shieldInfo", "images/diablo/item/shieldInfo.bmp", 200, 200, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("weaponInfo", "images/diablo/item/weaponInfo.bmp", 200, 200, true, RGB(255, 0, 255));
	
	IMAGEMANAGER->addImage("shopShieldInfo", "images/diablo/item/shopShieldInfo.bmp", 200, 200, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("shopWeaponInfo", "images/diablo/item/shopWeaponInfo.bmp", 200, 200, true, RGB(255, 0, 255));
	 
	//npc
	IMAGEMANAGER->addFrameImage("akara", "images/diablo/npc/akara.bmp", 400, 400, 352, 73, 13, 1, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("akaraShadow", "images/diablo/npc/akara_shadow.bmp", 400, 400, 710, 73, 13, 1, true, RGB(255, 0, 255));
	 
	IMAGEMANAGER->addImage("shop", "images/diablo/panel/shop.bmp", 400, 500, true, RGB(255, 0, 255));

	// 글씨
	IMAGEMANAGER->addImage("default_skill_font", "images/diablo/font/default_skill_font.BMP", 36, 45, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("poison_bone_skill_font", "images/diablo/font/poison_bone_skill_font.BMP", 36, 58, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("summon_skill_font", "images/diablo/font/summon_skill_font.BMP", 37, 35, true, RGB(255, 0, 255));
	IMAGEMANAGER->addImage("curse_skill_font", "images/diablo/font/curse_skill_font.BMP", 37, 14, true, RGB(255, 0, 255));
	
	mainScene* _mainScene = new mainScene;

	SCENEMANAGER->addScene("mainScene", _mainScene);

	townScene* _townScene = new townScene; 
	SCENEMANAGER->addScene("townScene", _townScene);

	campScene* _campScene = new campScene;
	SCENEMANAGER->addScene("campScene", _campScene);

	//SCENEMANAGER->changeScene("campScene");
	SCENEMANAGER->changeScene("mainScene");
	//SCENEMANAGER->changeScene("townScene");
 
	return S_OK;
}

void mainGame::release()
{
	gameNode::release();

}

void mainGame::update()
{
	gameNode::update();
	SCENEMANAGER->update(); 
	ShowCursor(false);
	_count++;
	if (_count % 3 == 0)
	{
		_idx++;
		if (_idx > _mouse->getMaxFrameX()) _idx = 0;
	}
}

void mainGame::render(/*HDC hdc*/)
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	SCENEMANAGER->render(); 
	TIMEMANAGER->render(getMemDC());
	_mouse->frameRender(getMemDC(), m_ptMouse.x, m_ptMouse.y, _idx , 0);
	//=====================================================
	//백버퍼의 내용을 HDC에 그린다.(지우지마!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}










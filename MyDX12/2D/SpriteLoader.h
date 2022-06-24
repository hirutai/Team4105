#pragma once

namespace XIIlib
{
	enum SpriteNum
	{
		DEBUG_TEXT_TEX = 0, DEBUG_JISTEXT_TEX,
		DIGITALNUMBER_TEX, EFEECT1_TEX,
		// ゲームで使ってる部分
		SPACE_TEX, 
		GAMEOVER_TEX, 
		STAGEBG1_TEX,
		EASY_BUTTON_TEX, NORMAL_BUTTON_TEX, HARD_BUTTON_TEX,
		EDGE_TEX, 
		TITLEBG_TEX, TITLELOG_TEX,
		GAMECLEAR_TEX, 
		OPERATORGUIDE_TEX, MENU_TEX,CURSOR_TEX,
		BUTTON_PLAY_TEX,BUTTON_SELECT_TEX,BUTTON_TITLE_TEX,
		BUTTON_PLAYER_TEX,BUTTON_ENEMY_TEX,
		PLAYERGUIDES_TEX, ENEMYGUIDES_TEX,
		BG_TEX,STAGESELECT_TEX,BAN_TEX,
		STORY0_SP,STORY1_SP,STORY2_SP,
	};
	class SpriteLoader
	{	
	public:
		static void Load();
	private:
		static unsigned int spriteCount;
	};
}

#pragma once

namespace XIIlib
{
	enum SpriteNum
	{
		DEBUG_TEXT_TEX = 0, DEBUG_JISTEXT_TEX,
		DIGITALNUMBER_TEX, EFEECT1_TEX,
		// ÉQÅ[ÉÄÇ≈égÇ¡ÇƒÇÈïîï™
		SPACE_TEX, GAMEOVER_TEX, STAGEBG1_TEX,
		EASY_BUTTON_TEX, NORMAL_BUTTON_TEX, HARD_BUTTON_TEX,
		EDGE_TEX, TITLEBG_TEX, TITLELOG_TEX,
		GAMECLEAR_TEX, PLAYERGUIDE_TEX, MENU_TEX,
		ENEMYGUIDES_TEX, BG_TEX, STAGESELECT_TEX,
		BAN_TEX,
	};
	class SpriteLoader
	{	
	public:
		static void Load();
	private:
		static unsigned int spriteCount;
	};
}

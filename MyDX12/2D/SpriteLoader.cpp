#include "SpriteLoader.h"
#include "../2D/Sprite.h"
using namespace XIIlib;
unsigned int SpriteLoader::spriteCount = 0;

void XIIlib::SpriteLoader::Load()
{
	Sprite::LoadTexture(DEBUG_TEXT_TEX,    L"Resources/ASCIItex_ver2.png");
	Sprite::LoadTexture(DEBUG_JISTEXT_TEX, L"Resources/JIS_Code_ver.1.02.png");
	Sprite::LoadTexture(DIGITALNUMBER_TEX, L"Resources/digitalNum.png");
	Sprite::LoadTexture(EFEECT1_TEX,       L"Resources/effect1.png");
	// ÉQÅ[ÉÄ
	Sprite::LoadTexture(SPACE_TEX,         L"Resources/Space.png");
	Sprite::LoadTexture(GAMEOVER_TEX,      L"Resources/GameOver.png");
	Sprite::LoadTexture(STAGEBG1_TEX,      L"Resources/stage_BG1.png");
	Sprite::LoadTexture(EASY_BUTTON_TEX,   L"Resources/Easy.png");
	Sprite::LoadTexture(NORMAL_BUTTON_TEX, L"Resources/Normal.png");
	Sprite::LoadTexture(HARD_BUTTON_TEX,   L"Resources/coming soon.png");
	Sprite::LoadTexture(EDGE_TEX,          L"Resources/waku.png");
	Sprite::LoadTexture(TITLEBG_TEX,       L"Resources/TitleBG.png");
	Sprite::LoadTexture(TITLELOG_TEX,      L"Resources/TitleLog.png");
	Sprite::LoadTexture(GAMECLEAR_TEX,     L"Resources/GameClear.png");
	Sprite::LoadTexture(OPERATORGUIDE_TEX, L"Resources/explanation.png");
	Sprite::LoadTexture(MENU_TEX,          L"Resources/tabkey.png");
	Sprite::LoadTexture(CURSOR_TEX,        L"Resources/cursor.png");
	Sprite::LoadTexture(BUTTON_PLAY_TEX,   L"Resources/test.png");
	Sprite::LoadTexture(BUTTON_SELECT_TEX, L"Resources/sute.png");
	Sprite::LoadTexture(BUTTON_TITLE_TEX,  L"Resources/taitleni.png");
	Sprite::LoadTexture(BUTTON_PLAYER_TEX, L"Resources/test.png");
	Sprite::LoadTexture(BUTTON_ENEMY_TEX,  L"Resources/test.png");
	Sprite::LoadTexture(PLAYERGUIDES_TEX,  L"Resources/Menu.png");
	Sprite::LoadTexture(ENEMYGUIDES_TEX,   L"Resources/Menu.png");
	Sprite::LoadTexture(BG_TEX,            L"Resources/BG.png");
	Sprite::LoadTexture(STAGESELECT_TEX,   L"Resources/StageSelect.png");
	Sprite::LoadTexture(BAN_TEX,           L"Resources/banTex.png");
	Sprite::LoadTexture(STORY0_SP,         L"Resources/story0.png");
	//Sprite::LoadTexture(STORY1_SP,         L"Resources/.png");
	//Sprite::LoadTexture(STORY2_SP,         L"Resources/.png");
	Sprite::LoadTexture(WHITEOUT,          L"Resources/white.png");
}

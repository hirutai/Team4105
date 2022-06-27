#include "Title.h"
#include "GameScene.h"
#include "Story.h"
#include "Menu.h"
#include "Select.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

XIIlib::Title::Title()
{
	// Initializeとほぼ同じ

}

XIIlib::Title::~Title()
{
	// ポインタ使ったやつの埋葬場
	delete spTitleLog;
	delete spTitleBG;
	delete space;
}

void XIIlib::Title::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	spTitleBG = Sprite::Create(TITLEBG_TEX, { 0.0f,0.0f });
	spTitleLog = Sprite::Create(TITLELOG_TEX, { 320.0f,0.0f });
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Title::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		trigSpace = true;
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
	}

	if (trigSpace) {
		if (p_game_scene->DrawScreen(false)) {
			p_game_scene->ChangeState(new Story);
		}
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Select);
	}

	if (gamePad_->Button_Down(X_Y)) {
		stageNum = StageNumber::EASY;
	}

	if (gamePad_->Button_Down(X_X)) {
		stageNum = StageNumber::NORMAL;
	}
}

void XIIlib::Title::Draw()
{
	// 3D描画

}

void XIIlib::Title::DrawTex()
{
	// スプライト描画
	space->Draw();
}

void XIIlib::Title::DrawBackground()
{
	spTitleBG->Draw();
	spTitleLog->Draw();
}

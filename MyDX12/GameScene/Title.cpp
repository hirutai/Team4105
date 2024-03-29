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

void XIIlib::Title::Initialize()
{
	// Scene切り替え時に一度通る処理
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	spTitleBG = Sprite::Create(TITLEBG_TEX, { 0.0f,0.0f });
	spTitleLog = Sprite::Create(TITLELOG_TEX, { 320.0f,0.0f });
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Title::Update()
{
	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			//p_game_scene->ChangeState(new Story);
			p_game_scene->ChangeState(new Select);
		}
	}

	if (trigSpace)return;

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
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

#include "Title.h"
#include "GameScene.h"
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
	delete spPushA;
}

void XIIlib::Title::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	spPushA = Sprite::Create((UINT)SpriteName::PUSHA_SP, {0.0f,0.0f});
	spTitleBG = Sprite::Create((UINT)SpriteName::TITLEBG_SP, { 0.0f,0.0f });
	spTitleLog = Sprite::Create((UINT)SpriteName::TITLELOG_SP, { 320.0f,0.0f });
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Title::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Select);
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
	//spPushA->Draw();
}

void XIIlib::Title::DrawBackground()
{
	spTitleBG->Draw();
	spTitleLog->Draw();
}

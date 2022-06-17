#include "Select.h"
#include "GameScene.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"

XIIlib::Select::Select()
{
	// Initializeとほぼ同じ
}

XIIlib::Select::~Select()
{
	// ポインタ使ったやつの埋葬場
}

void XIIlib::Select::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	easyButton.reset(Sprite::Create((UINT)SpriteName::EASY_BUTTON_SP, {0.0f,0.0f}));
	normalButton.reset(Sprite::Create((UINT)SpriteName::NORMAL_BUTTON_SP, { 0.0f,0.0f }));
	hardButton.reset(Sprite::Create((UINT)SpriteName::HARD_BUTTON_SP, { 0.0f,0.0f }));
	edge.reset(Sprite::Create((UINT)SpriteName::EDGE_SP, { 0.0f,0.0f }));
}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Play);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}
}

void XIIlib::Select::Draw()
{
	// 3D描画
}

void XIIlib::Select::DrawTex()
{
	// スプライト描画
	easyButton->Draw();
	normalButton->Draw();
	hardButton->Draw();
	edge->Draw();
}

void XIIlib::Select::DrawBackground()
{
}

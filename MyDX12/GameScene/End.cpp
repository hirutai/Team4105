#include "End.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"


XIIlib::End::End()
{
	// Initializeとほぼ同じ

}

XIIlib::End::~End()
{
	// ポインタ使ったやつの埋葬場
	delete spGameOver;
}

void XIIlib::End::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	UnitManager::GetInstance()->AllDestroy();
	spGameOver = Sprite::Create(GAMEOVER_SP, {0.0f,0.0f});
}

void XIIlib::End::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Title);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Title);
	}

	DebugJISText::GetInstance()->Print("エンド", 0, 0, 1);
}

void XIIlib::End::Draw()
{
	// 3D描画

}

void XIIlib::End::DrawTex()
{
	// スプライト描画
	spGameOver->Draw();
}

void XIIlib::End::DrawBackground()
{

}

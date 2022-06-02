#include "Title.h"
#include "GameScene.h"
#include "Menu.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"

XIIlib::Title::Title()
{
	// Initializeとほぼ同じ

}

XIIlib::Title::~Title()
{
	// ポインタ使ったやつの埋葬場

}

void XIIlib::Title::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理

}

void XIIlib::Title::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Menu);
	}

	DebugJISText::GetInstance()->Print("タイトル",0,0,1);
}

void XIIlib::Title::Draw()
{
	// 3D描画

}

void XIIlib::Title::DrawTex()
{
	// スプライト描画

}

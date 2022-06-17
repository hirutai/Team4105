#include "Menu.h"
#include "GameScene.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"

XIIlib::Menu::Menu()
{
	// Initializeとほぼ同じ

}

XIIlib::Menu::~Menu()
{
	// ポインタ使ったやつの埋葬場

}

void XIIlib::Menu::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Play);
		p_game_scene->GetAudio()->StopBGM();
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
		p_game_scene->GetAudio()->StopBGM();
	}

}

void XIIlib::Menu::Draw()
{
	// 3D描画

}

void XIIlib::Menu::DrawTex()
{
	// スプライト描画

}

void XIIlib::Menu::DrawBackground()
{

}

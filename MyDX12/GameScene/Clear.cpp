#include "Clear.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

using namespace XIIlib;

Clear::Clear()
{
}

Clear::~Clear()
{
	delete gameClear;
}

void Clear::Initialize(GameScene* p_game_scene)
{
	// Scene切り替え時に一度通る処理
	UnitManager::GetInstance()->AllDestroy();
	gameClear = Sprite::Create((UINT)SpriteName::GAMECLEAR_SP, { 0.0f,0.0f });
}

void Clear::Update(GameScene* p_game_scene)
{
	// 更新
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Title);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Title);
	}

}

void Clear::Draw()
{
	// 3D描画

}

void Clear::DrawTex()
{
	// スプライト描画
	gameClear->Draw();
}

void Clear::DrawBackground()
{

}
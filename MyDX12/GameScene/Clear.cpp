#include "Clear.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../GameObject/ChainSprite.h"

using namespace XIIlib;

Clear::Clear()
{
}

Clear::~Clear()
{
	delete pchStr;
	delete gameClear;
	delete space;
}

void Clear::Initialize()
{
	// Scene切り替え時に一度通る処理
	UnitManager::GetInstance()->AllDestroy();
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	gameClear = Sprite::Create(GAMECLEAR_TEX, { 0.0f,0.0f });

	pchStr = ChainSprite::Create();
	pchStr->AddMoji(360,360,60, MOJI_KEN);
	pchStr->AddMoji(360,420,60, MOJI_KA);
	pchStr->AddMoji(360,480,60, MOJI_ZYOU);
	pchStr->AddMoji(360,520,60, MOJI_TOU);
}

void Clear::Update()
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("clear.wav", 0.5f);
		oneThrough = true;
	}
	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			p_game_scene->ChangeState(new Title);
			easyCount = 0;
		}
	}

	pchStr->Update();

	if (trigSpace)return;

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
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
	space->Draw();
	pchStr->Draw();
}

void Clear::DrawBackground()
{

}
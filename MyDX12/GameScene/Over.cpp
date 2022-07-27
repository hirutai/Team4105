#include "Over.h"
#include "GameScene.h"
#include "Title.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

XIIlib::Over::Over()
{
	// Initializeとほぼ同じ

}

XIIlib::Over::~Over()
{
	// ポインタ使ったやつの埋葬場
	delete textTitle;
	delete textRetry;
	delete spGameOver;
}

void XIIlib::Over::Initialize()
{
	// Scene切り替え時に一度通る処理
	UnitManager::GetInstance()->AllDestroy();
	spGameOver = Sprite::Create(GAMEOVER_TEX, {0.0f,0.0f});

	textRetry = Sprite::Create(TEXT_RETRY, { 1280 * xMullValue,768.0f * yMullValue1 });
	textRetry->SetSize(textRetry->GetDefault() * mulXY * pickSize);
	textTitle = Sprite::Create(TEXT_GO_TITLE, { 1280 * xMullValue,768.0f * yMullValue2 });
	textTitle->SetSize(textTitle->GetDefault() * mulXY);
	textRetry->SetColor(1, 1, 1, 1);
	textTitle->SetColor(0.5f, 0.5f, 0.5f, 1);

	//p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Over::Update()
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("sakebi.wav", 0.1f);
		oneThrough = true;
	}

	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			if (!selectT_R) {
				// RETRY
				SceneState::phase = Phase::CameraDirecting;
				SceneState::CreateUnitsPosition(SceneState::stageNum, "stage0");
				p_game_scene->ChangeState(new Play());
				easyCount = 0;
			}
			else {
				// タイトルに遷移
				p_game_scene->ChangeState(new Title);
				easyCount = 0;
			}
			
		}
	}

	if (trigSpace)return;

	static Math::Vector2 dfaultSize = textRetry->GetDefault() * mulXY;
	if (KeyInput::GetInstance()->Trigger(DIK_W) || gamePad_->LStickDownFront()) {
		selectT_R = false;
		textRetry->SetColor(1,1,1,1);
		textTitle->SetColor(0.5f, 0.5f, 0.5f, 1);
		textRetry->SetSize(dfaultSize * pickSize);
		textTitle->SetSize(dfaultSize);
	}
	if (KeyInput::GetInstance()->Trigger(DIK_S) || gamePad_->LStickDownBack()) {
		selectT_R = true;
		textRetry->SetColor(0.5f, 0.5f, 0.5f, 1);
		textTitle->SetColor(1, 1, 1, 1);
		textRetry->SetSize(dfaultSize);
		textTitle->SetSize(dfaultSize * pickSize);
	}

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->StopSE();
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
	}
}

void XIIlib::Over::Draw()
{
	// 3D描画
}

void XIIlib::Over::DrawTex()
{
	// スプライト描画
	spGameOver->Draw();
	textRetry->Draw();
	textTitle->Draw();
}

void XIIlib::Over::DrawBackground()
{

}

#include "Clear.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../GameObject/ChainSprite.h"
#include "../Camera/DebugCamera.h"
#include "Select.h"

using namespace XIIlib;

Clear::Clear()
{
}

Clear::~Clear()
{
	delete pchStr;
	delete textTitle;
	delete textSelect;
	delete gameClear;
}

void Clear::Initialize()
{
	SetDebugCamera(p_game_scene->GetCamera());
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
 	pchStr = ChainSprite::Create();
	
	UnitManager::GetInstance()->AllDestroy();
	if (stageNum == StageNumber::HARD)
	{
		pchStr->AddMoji(180, 90, 360, MOJI_CLEAR1);
		pchStr->AddMoji(1100, 90, 360, MOJI_CLEAR2);
		pchStr->AddMoji(180, 630, 360, MOJI_CLEAR3);
		pchStr->AddMoji(1100, 630, 360, MOJI_CLEAR4);
		gameClear = Sprite::Create(BOSSCLEAR_TEX, { 0.0f,0.0f });
		textSelect = Sprite::Create(TEXT_SELECT, { 1280 * xMullValueHard,768.0f * yMullValue1 });
		textSelect->SetSize(textSelect->GetDefault() * mulXY * pickSize);
		textTitle = Sprite::Create(CLEAR_GO_TITLE, { 1280 * xMullValueHard,768.0f * yMullValue2 });
		textTitle->SetSize(textTitle->GetDefault() * mulXY);
	}
	else
	{
		pchStr->AddMoji(180, 120, 180, MOJI_KEN);
		pchStr->AddMoji(180, 240, 180, MOJI_KA);
		pchStr->AddMoji(180, 360, 180, MOJI_ZYOU);
		pchStr->AddMoji(180, 480, 180, MOJI_TOU);
		gameClear = Sprite::Create(GAMECLEAR_TEX, { 0.0f,0.0f });
		textSelect = Sprite::Create(TEXT_SELECT, { 1280 * xMullValue,768.0f * yMullValue1 });
		textSelect->SetSize(textSelect->GetDefault() * mulXY * pickSize);
		textTitle = Sprite::Create(CLEAR_GO_TITLE, { 1280 * xMullValue,768.0f * yMullValue2 });
		textTitle->SetSize(textTitle->GetDefault() * mulXY);
	}
	textSelect->SetColor(1, 1, 1, 1);
	textTitle->SetColor(0.5f, 0.5f, 0.5f, 1);
	gameClear->SetSize({ 1280, 768 });
}

void Clear::Update()
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("clear.wav", 0.5f);
		oneThrough = true;
	}

	pchStr->Update();

	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			if (!selectT_R) {
				// RETRY
				SceneState::phase = Phase::CameraDirecting;
				debugCamera->SetPosition(0, 10, -30); // 視点座標の設定
				debugCamera->_Update();
				UnitManager::GetInstance()->ObjectUpdate();
				p_game_scene->ChangeState(new Select());
				easyCount = 0;
				return;
			}
			else {
				// タイトルに遷移
				p_game_scene->ChangeState(new Title);
				easyCount = 0;
				return;
			}
		}
	}

	if (trigSpace)return;

	static Math::Vector2 dfaultSize = textSelect->GetDefault() * mulXY;
	if (KeyInput::GetInstance()->Trigger(DIK_W) || gamePad_->LStickDownFront()) {
		p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
		selectT_R = false;
		textSelect->SetColor(1, 1, 1, 1);
		textTitle->SetColor(0.5f, 0.5f, 0.5f, 1);
		textSelect->SetSize(dfaultSize * pickSize);
		textTitle->SetSize(dfaultSize);
	}
	if (KeyInput::GetInstance()->Trigger(DIK_S) || gamePad_->LStickDownBack()) {
		p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
		selectT_R = true;
		textSelect->SetColor(0.5f, 0.5f, 0.5f, 1);
		textTitle->SetColor(1, 1, 1, 1);
		textSelect->SetSize(dfaultSize);
		textTitle->SetSize(dfaultSize * pickSize);
	}

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->StopSE();
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
	}
}

void Clear::Draw()
{
	// 3D�`��

}

void Clear::DrawTex()
{
	// �X�v���C�g�`��
	gameClear->Draw();
	textSelect->Draw();
	textTitle->Draw();
	pchStr->Draw();
}

void Clear::DrawBackground()
{

}
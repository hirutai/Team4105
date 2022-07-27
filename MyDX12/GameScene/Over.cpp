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
	// Initialize�Ƃقړ���

}

XIIlib::Over::~Over()
{
	// �|�C���^�g������̖�����
	delete textTitle;
	delete textRetry;
	delete spGameOver;
}

void XIIlib::Over::Initialize()
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	UnitManager::GetInstance()->AllDestroy();
	spGameOver = Sprite::Create(GAMEOVER_TEX, {0.0f,0.0f});

	const float yMullValue1 = 0.5f, yMullValue2 = 0.7f,
		xMullValue = 0.53f,mulXY = 2.0f;

	textRetry = Sprite::Create(TEXT_RETRY, { 1280 * xMullValue,768.0f * yMullValue1 });
	textRetry->SetSize(textRetry->GetDefault() * mulXY);
	textTitle = Sprite::Create(TEXT_GO_TITLE, { 1280 * xMullValue,768.0f * yMullValue2 });
	textTitle->SetSize(textTitle->GetDefault() * mulXY);
	textRetry->SetColor(0.5f, 0.5f, 0.5f, 1);
	textTitle->SetColor(1, 1, 1, 1);
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
			}
			else {
				// �^�C�g���ɑJ��
				p_game_scene->ChangeState(new Title);
			}
			
		}
	}

	if (trigSpace)return;

	if (KeyInput::GetInstance()->Trigger(DIK_W) || gamePad_->LStickDownFront()) {
		selectT_R = true;
		textRetry->SetColor(1,1,1,1);
		textTitle->SetColor(0.5f, 0.5f, 0.5f, 1);
	}
	if (KeyInput::GetInstance()->Trigger(DIK_S) || gamePad_->LStickDownBack()) {
		selectT_R = false;
		textRetry->SetColor(0.5f, 0.5f, 0.5f, 1);
		textTitle->SetColor(1, 1, 1, 1);
	}

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->StopSE();
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
	}
}

void XIIlib::Over::Draw()
{
	// 3D�`��
}

void XIIlib::Over::DrawTex()
{
	// �X�v���C�g�`��
	spGameOver->Draw();
	textRetry->Draw();
	textTitle->Draw();
}

void XIIlib::Over::DrawBackground()
{

}

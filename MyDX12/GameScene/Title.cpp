#include "Title.h"
#include "GameScene.h"
#include "Menu.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

XIIlib::Title::Title()
{
	// Initialize�Ƃقړ���

}

XIIlib::Title::~Title()
{
	// �|�C���^�g������̖�����
	delete spPushA;
}

void XIIlib::Title::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	spPushA = Sprite::Create(PUSHA_SP, {0.0f,0.0f});
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Title::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Play);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}

	if (gamePad_->Button_Down(X_Y)) {
		stageNum = EASY;
	}

	if (gamePad_->Button_Down(X_X)) {
		stageNum = NORMAL;
	}

}

void XIIlib::Title::Draw()
{
	// 3D�`��

}

void XIIlib::Title::DrawTex()
{
	// �X�v���C�g�`��
	spPushA->Draw();
}

void XIIlib::Title::DrawBackground()
{

}

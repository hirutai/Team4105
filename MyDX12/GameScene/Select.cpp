#include "Select.h"
#include "GameScene.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"

XIIlib::Select::Select()
{
	// Initialize�Ƃقړ���
}

XIIlib::Select::~Select()
{
	// �|�C���^�g������̖�����
}

void XIIlib::Select::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	easyButton.reset(Sprite::Create((UINT)SpriteName::EASY_BUTTON_SP, {0.0f,0.0f}));
	normalButton.reset(Sprite::Create((UINT)SpriteName::NORMAL_BUTTON_SP, { 0.0f,0.0f }));
	hardButton.reset(Sprite::Create((UINT)SpriteName::HARD_BUTTON_SP, { 0.0f,0.0f }));
	edge.reset(Sprite::Create((UINT)SpriteName::EDGE_SP, { 0.0f,0.0f }));
}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Play);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}
}

void XIIlib::Select::Draw()
{
	// 3D�`��
}

void XIIlib::Select::DrawTex()
{
	// �X�v���C�g�`��
	easyButton->Draw();
	normalButton->Draw();
	hardButton->Draw();
	edge->Draw();
}

void XIIlib::Select::DrawBackground()
{
}

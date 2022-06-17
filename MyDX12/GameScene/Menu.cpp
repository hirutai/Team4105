#include "Menu.h"
#include "GameScene.h"
#include "Play.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"

XIIlib::Menu::Menu()
{
	// Initialize�Ƃقړ���

}

XIIlib::Menu::~Menu()
{
	// �|�C���^�g������̖�����

}

void XIIlib::Menu::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��

}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Play);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}

	DebugJISText::GetInstance()->Print("���j���[", 0, 0, 1);
}

void XIIlib::Menu::Draw()
{
	// 3D�`��

}

void XIIlib::Menu::DrawTex()
{
	// �X�v���C�g�`��

}

void XIIlib::Menu::DrawBackground()
{

}

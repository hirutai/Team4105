#include "Menu.h"
#include "GameScene.h"
#include "Select.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"
#include "Play.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"

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
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_TAB)) {
		p_game_scene->ChangeState(new Play);
	}


	// �e�X�g�p
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Select);
	}

}

void XIIlib::Menu::Draw()
{
	// 3D�`��
	AttackAreaManager::GetInstance()->Draw();
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Menu::DrawTex()
{
	// �X�v���C�g�`��

}

void XIIlib::Menu::DrawBackground()
{

}

#include "Title.h"
#include "GameScene.h"
#include "Menu.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"

XIIlib::Title::Title()
{
	// Initialize�Ƃقړ���

}

XIIlib::Title::~Title()
{
	// �|�C���^�g������̖�����

}

void XIIlib::Title::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��

}

void XIIlib::Title::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Menu);
	}

	DebugJISText::GetInstance()->Print("�^�C�g��",0,0,1);
}

void XIIlib::Title::Draw()
{
	// 3D�`��

}

void XIIlib::Title::DrawTex()
{
	// �X�v���C�g�`��

}

#include "End.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"


XIIlib::End::End()
{
	// Initialize�Ƃقړ���

}

XIIlib::End::~End()
{
	// �|�C���^�g������̖�����
	delete spGameOver;
}

void XIIlib::End::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	UnitManager::GetInstance()->AllDestroy();
	spGameOver = Sprite::Create(GAMEOVER_SP, {0.0f,0.0f});
}

void XIIlib::End::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new Title);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Title);
	}

	DebugJISText::GetInstance()->Print("�G���h", 0, 0, 1);
}

void XIIlib::End::Draw()
{
	// 3D�`��

}

void XIIlib::End::DrawTex()
{
	// �X�v���C�g�`��
	spGameOver->Draw();
}

void XIIlib::End::DrawBackground()
{

}

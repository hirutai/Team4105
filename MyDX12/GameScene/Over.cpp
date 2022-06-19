#include "Over.h"
#include "GameScene.h"
#include "Title.h"
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
	delete spGameOver;
}

void XIIlib::Over::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	UnitManager::GetInstance()->AllDestroy();
	spGameOver = Sprite::Create((UINT)SpriteName::GAMEOVER_SP, {0.0f,0.0f});
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Over::Update(GameScene* p_game_scene)
{
	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
		p_game_scene->ChangeState(new Title);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Title);
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
}

void XIIlib::Over::DrawBackground()
{

}

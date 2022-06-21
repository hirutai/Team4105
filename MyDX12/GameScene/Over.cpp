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
	delete space;
}

void XIIlib::Over::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	//UnitManager::GetInstance()->AllDestroy();
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	spGameOver = Sprite::Create(GAMEOVER_TEX, {0.0f,0.0f});
	//p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Over::Update(GameScene* p_game_scene)
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("sakebi.wav", 0.5f);
		oneThrough = true;
	}

	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		trigSpace = true;
		p_game_scene->GetAudio()->StopSE();
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
	}

	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Title);
	}

	if (trigSpace) {
		if (p_game_scene->DrawScreen(false)) {
			p_game_scene->ChangeState(new Title);
		}
	}
}

void XIIlib::Over::Draw()
{
	// 3D�`��
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Over::DrawTex()
{
	// �X�v���C�g�`��
	//spGameOver->Draw();
	//space->Draw();
}

void XIIlib::Over::DrawBackground()
{

}

#include "Clear.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

using namespace XIIlib;

Clear::Clear()
{
}

Clear::~Clear()
{
	delete gameClear;
	delete space;
}

void Clear::Initialize(GameScene* p_game_scene)
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	UnitManager::GetInstance()->AllDestroy();
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	gameClear = Sprite::Create(GAMECLEAR_TEX, { 0.0f,0.0f });
}

void Clear::Update(GameScene* p_game_scene)
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("clear.wav", 0.5f);
		oneThrough = true;
	}

	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		trigSpace = true;
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

void Clear::Draw()
{
	// 3D�`��

}

void Clear::DrawTex()
{
	// �X�v���C�g�`��
	gameClear->Draw();
	space->Draw();
}

void Clear::DrawBackground()
{

}
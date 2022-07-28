#include "Clear.h"
#include "GameScene.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/UnitManager.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"
#include "../GameObject/ChainSprite.h"

using namespace XIIlib;

Clear::Clear()
{
}

Clear::~Clear()
{
	delete pchStr;
	delete gameClear;
	delete space;
}

void Clear::Initialize()
{
	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	UnitManager::GetInstance()->AllDestroy();
	space = Sprite::Create(SPACE_TEX, { 1280 / 2 - 300, 768 / 2 + 100 });
	if (stageNum == StageNumber::HARD)
	{
		gameClear = Sprite::Create(BOSSCLEAR_TEX, { 0.0f,0.0f });
	}
	else
	{
		gameClear = Sprite::Create(GAMECLEAR_TEX, { 0.0f,0.0f });
	}
	gameClear->SetSize({ 1280, 768 });

	pchStr = ChainSprite::Create();
	pchStr->AddMoji(180,120,180, MOJI_KEN);
	pchStr->AddMoji(180,240,180, MOJI_KA);
	pchStr->AddMoji(180,360,180, MOJI_ZYOU);
	pchStr->AddMoji(180,480,180, MOJI_TOU);
}

void Clear::Update()
{
	if (!oneThrough) {
		p_game_scene->GetAudio()->PlaySE("clear.wav", 0.5f);
		oneThrough = true;
	}
	if (trigSpace) {
		if (p_game_scene->DrawScreen(TransitionType::CLOSE)) {
			p_game_scene->ChangeState(new Title);
			easyCount = 0;
		}
	}


	if (trigSpace)return;
	pchStr->Update();

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE) || gamePad_->Button_Down(X_A)) {
		trigSpace = true;
		p_game_scene->GetAudio()->PlaySE("kettei.wav", 0.3f);
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
	pchStr->Draw();
}

void Clear::DrawBackground()
{

}
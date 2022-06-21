#include "Select.h"
#include "GameScene.h"
#include "Play.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

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
	easyButton.reset(Sprite::Create(EASY_BUTTON_TEX, HOMEL_POS));
	normalButton.reset(Sprite::Create(NORMAL_BUTTON_TEX, HOMEC_POS));
	hardButton.reset(Sprite::Create(HARD_BUTTON_TEX, HOMER_POS));
	edge.reset(Sprite::Create(EDGE_TEX, HOMEL_POS));
	bg_sp.reset(Sprite::Create(BG_TEX, { 0, 0}));
	stageSelect_sp.reset(Sprite::Create(STAGESELECT_TEX, { 250, 0}));

	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}

	if (!trigSpace) {
		// A��D�������Ă���
		if (KeyInput::GetInstance()->Trigger(DIK_A)
			|| KeyInput::GetInstance()->Trigger(DIK_D))
		{
			const float moveX = BASE_SIZE.x + GAP;

			if (KeyInput::GetInstance()->Trigger(DIK_A))
			{
				edgePos.x -= moveX;
				if (edgePos.x < HOMEL_POS.x)
				{
					edgePos.x = HOMEL_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
			else if (KeyInput::GetInstance()->Trigger(DIK_D))
			{
				edgePos.x += moveX;
				if (edgePos.x > HOMER_POS.x)
				{
					edgePos.x = HOMER_POS.x;
				}
				else
				{
					p_game_scene->GetAudio()->PlaySE("select.wav", 0.1f);
				}
			}
		}
	}

	// ��������߂�
	if (edgePos.x <= HOMEL_POS.x)
	{
		edgePos.x = HOMEL_POS.x;
	}
	else if (edgePos.x >= HOMER_POS.x)
	{
		edgePos.x = HOMER_POS.x;
	}

	// ���̍X�V
	edge->SetPosition(edgePos);

	// �X�V
	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		trigSpace = true;
		// �|�W�V�����ɂ���ăX�e�[�W�i���o�[����
		if (edgePos.x == HOMEL_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			stageNum = StageNumber::EASY;
		}
		else if (edgePos.x == HOMEC_POS.x)
		{
			p_game_scene->GetAudio()->PlaySE("stageSelect.wav", 0.3f);
			stageNum = StageNumber::NORMAL;
		}
		else if (edgePos.x == HOMER_POS.x)
		{
			//p_game_scene->GetAudio()->PlaySE("stageSelect.wav",0.3f);
			return;
			//stageNum = StageNumber::HARD;
		}
	}

	if (trigSpace) {
		if (p_game_scene->DrawScreen(false)) {
			p_game_scene->ChangeState(new Play);
		}
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
	stageSelect_sp->Draw();
	edge->Draw();

}

void XIIlib::Select::DrawBackground()
{
	bg_sp->Draw();
}

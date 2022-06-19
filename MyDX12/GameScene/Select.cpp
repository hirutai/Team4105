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
	easyButton.reset(Sprite::Create((UINT)SpriteName::EASY_BUTTON_SP, HOMEL_POS));
	normalButton.reset(Sprite::Create((UINT)SpriteName::NORMAL_BUTTON_SP, HOMEC_POS));
	hardButton.reset(Sprite::Create((UINT)SpriteName::HARD_BUTTON_SP, HOMER_POS));
	edge.reset(Sprite::Create((UINT)SpriteName::EDGE_SP, HOMEL_POS));
	bg_sp.reset(Sprite::Create((UINT)SpriteName::BG_SP, { 0, 0}));
	stageSelect_sp.reset(Sprite::Create((UINT)SpriteName::STAGESELECT_SP, { 250, 0}));

	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	if (movingScene) {

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
	else {
		// �V�[���̑J�ڂ��������Ă��邩�H
		if (p_game_scene->DrawScreen(true)) {
			movingScene = true;
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

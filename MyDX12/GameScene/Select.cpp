#include "Select.h"
#include "GameScene.h"
#include "Play.h"
#include "Title.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../2D/Sprite.h"

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
}

void XIIlib::Select::Update(GameScene* p_game_scene)
{
	
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
	}
	// A��D�������Ă���
	if (KeyInput::GetInstance()->Trigger(DIK_A) 
		|| KeyInput::GetInstance()->Trigger(DIK_D)) 
	{
		const float moveX = BASE_SIZE.x + GAP;
		
		if (KeyInput::GetInstance()->Trigger(DIK_A))
		{
			edgePos.x -= moveX;
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_D))
		{
			edgePos.x += moveX;
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
		// �|�W�V�����ɂ���ăX�e�[�W�i���o�[����
		if (edgePos.x == HOMEL_POS.x)
		{
			stageNum = StageNumber::EASY;
		}
		else if (edgePos.x == HOMEC_POS.x)
		{
			stageNum = StageNumber::NORMAL;
		}
		else if (edgePos.x == HOMER_POS.x)
		{
			return;
			//stageNum = StageNumber::HARD;
		}
		p_game_scene->ChangeState(new Play);
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
	edge->Draw();
}

void XIIlib::Select::DrawBackground()
{
}

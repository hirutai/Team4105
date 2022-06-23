#include "Menu.h"
#include "GameScene.h"
#include "Select.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../Audio/Audio.h"
#include "Play.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../2D/Sprite.h"
#include "../Tool/Easing.h"

XIIlib::Menu::Menu()
{
	// Initialize�Ƃقړ���

}

XIIlib::Menu::~Menu()
{
	// �|�C���^�g������̖�����
	delete enemyGuides;
	delete playerGuide;
	for (int i = MAX_BUTTON -1; i >= 0; --i)
	{
		delete buttons[i];
	}
	delete cursor;
	delete spStageBG1;
}

void XIIlib::Menu::Initialize(GameScene* p_game_scene)
{
	// �萔
	const DirectX::XMFLOAT4 color = {1,1,1,1};
	const Math::Vector2 anchorPoint = {0.5f,0.5f};

	// ����
	spStageBG1   = Sprite::Create(STAGEBG1_TEX, { 0.0f,0.0f }); // �w�i
	cursor       = Sprite::Create(CURSOR_TEX, { winCenter.x - 80.0f, winCenter.y - SPACE * 2 }, color, anchorPoint);

	buttons[(int)MenuState::CONTINUE] 
		= Sprite::Create(BUTTON_PLAY_TEX,  { winCenter.x, winCenter.y - SPACE * 2}, color, anchorPoint);
	buttons[(int)MenuState::PLAYER_GUIDS] 
		= Sprite::Create(BUTTON_PLAYER_TEX,{ winCenter.x, winCenter.y - SPACE}, color, anchorPoint);
	buttons[(int)MenuState::ENEMY_GUIDS] 
		= Sprite::Create(BUTTON_ENEMY_TEX, { winCenter.x, winCenter.y }, color, anchorPoint);
	buttons[(int)MenuState::NEXT_SLECT] 
		= Sprite::Create(BUTTON_SELECT_TEX,{ winCenter.x, winCenter.y + SPACE}, color, anchorPoint);
	buttons[(int)MenuState::NEXT_TITLE] 
		= Sprite::Create(BUTTON_TITLE_TEX, { winCenter.x, winCenter.y + SPACE * 2 }, color, anchorPoint);

	// �T�C�Y��0��
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->SetSize({0,0});
	}

	playerGuide = Sprite::Create(PLAYERGUIDES_TEX, winCenter, color, anchorPoint); // �v���C���[�̐���
	enemyGuides = Sprite::Create(ENEMYGUIDES_TEX, winCenter, color, anchorPoint); // �G�̐���
}

void XIIlib::Menu::Update(GameScene* p_game_scene)
{
	// �Ō�̃J�E���^�[��MAX�ɓ��B����܂ŁAEasing����
	if (easingCounts[MAX_BUTTON -1] < MAX_EASING_COUNT)
	{
		for (int i = 0; i < MAX_BUTTON; ++i)
		{
			EasingMove(i);
		}
		CountsUpdate();
	}
	else
	{
		// �J�[�\���̕\��
		if (!cursorDisp) { cursorDisp = true; }
		// �X�V
		if (KeyInput::GetInstance()->Trigger(DIK_TAB)) {
			p_game_scene->ChangeState(new Play);
			menuExists = false;
		}
	}
	
	// ����key����������
	if (KeyInput::GetInstance()->Trigger(DIK_W) || KeyInput::GetInstance()->Trigger(DIK_S))
	{
		Math::Vector2 move = {0.0f,0.0f};
		Math::Vector2 pos = cursor->GetPosition();
		// ���ꂼ��̏���
		if (KeyInput::GetInstance()->Trigger(DIK_W))
		{
			move.y = -SPACE;
		}
		else if (KeyInput::GetInstance()->Trigger(DIK_S))
		{
			move.y = SPACE;
		}
		pos.y += move.y;
		// �g�𒴂�����~�߂�
		if (pos.y < buttons[0]->GetPosition().y) 
		{
			pos.y = buttons[0]->GetPosition().y;
		}
		else if (pos.y > buttons[MAX_BUTTON - 1]->GetPosition().y)
		{
			pos.y = buttons[MAX_BUTTON - 1]->GetPosition().y;
		}

		cursor->SetPosition(pos);
	}

	// �e�X�g�p
	if (gamePad_->Button_Down(X_A)) {
		p_game_scene->ChangeState(new Play);
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
	if (cursorDisp) {
		cursor->Draw();
	}
	for (int i = 0; i < MAX_BUTTON; ++i)
	{
		buttons[i]->Draw();
	}
	//playerGuide->Draw();
	//enemyGuides->Draw();
}

void XIIlib::Menu::DrawBackground()
{
	spStageBG1->Draw();
}

void XIIlib::Menu::EasingMove(int i)
{
	// count���}�b�N�X�ɓ��B����܂�
	if (easingCounts[i] > MAX_EASING_COUNT) return;

	Math::Vector2 size = { 0,0 };
	float alpha = 0;
	size.x = Easing::EaseInOutCubic(easingCounts[i], 0, 94, MAX_EASING_COUNT);
	size.y = Easing::EaseInOutCubic(easingCounts[i], 0, 34, MAX_EASING_COUNT);
	alpha = Easing::EaseInOutCubic(easingCounts[i], 0, 1, MAX_EASING_COUNT);
	buttons[i]->SetSize(size);
	buttons[i]->SetAlpha(alpha);
}

void XIIlib::Menu::CountsUpdate()
{
	// �S�ẴJ�E���g�C���N�������g
	int count = 0;
	CountUpdate(count);
}

void XIIlib::Menu::CountUpdate(int& count)
{
	// �J�E���g���C���N�������g�A�x�����̃J�E���g���傫���Ȃ����玟�ɍs��
	easingCounts[count]++;
	if (easingCounts[count] < EASING_SPACE)return;
	count++;
	if (count >= MAX_BUTTON)return;
	CountUpdate(count);
}


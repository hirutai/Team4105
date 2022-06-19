#include "Play.h"
#include "GameScene.h"
#include "Clear.h"
#include "Over.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../GameObject/Yankee.h"
#include "../GameObject/Stone.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../GameObject/IntervalTimer.h"
#include "../2D/Sprite.h"
#include "../Audio/Audio.h"

XIIlib::Play::Play()
{
	// Initialize�Ƃقړ���

}

XIIlib::Play::~Play()
{
	// �|�C���^�g������̖�����
	delete enemyGuides; // �G�̐���
	delete menu; // ���j���[
	delete playerGuide; // �������
	delete spStageBG1;
	delete intervalTimter;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	
	// IntervalTimer new�Ə�����
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(4, 5);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);
	
	if (stageNum == StageNumber::EASY)
	{
		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));

		// �w�i�摜����
		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::NORMAL)
	{
		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		//std::shared_ptr<Knight> knight = std::move(Knight::Create(7, 7));
		//std::shared_ptr<Knight> knight2 = std::move(Knight::Create(6, 2));
		//std::shared_ptr<Knight> knight3 = std::move(Knight::Create(7, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		//std::shared_ptr<Bishop> bishop2 = std::move(Bishop::Create(3, 5));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(3, 6));
		//std::shared_ptr<Rook> rook2 = std::move(Rook::Create(6, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 3));
		std::shared_ptr<Yankee> yankee2 = std::move(Yankee::Create(5, 1));
		std::shared_ptr<Yankee> yankee3 = std::move(Yankee::Create(5, 6));
		std::shared_ptr<Yankee> yankee4 = std::move(Yankee::Create(5, 7));
		//std::shared_ptr<Stone> stone = std::move(Stone::Create(6, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		//UnitManager::GetInstance()->AddUnit(std::move(knight));
		//UnitManager::GetInstance()->AddUnit(std::move(knight2));
		//UnitManager::GetInstance()->AddUnit(std::move(knight3));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		//UnitManager::GetInstance()->AddUnit(std::move(bishop2));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		//UnitManager::GetInstance()->AddUnit(std::move(rook2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));
		UnitManager::GetInstance()->AddUnit(std::move(yankee2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee3));
		UnitManager::GetInstance()->AddUnit(std::move(yankee4));
		//UnitManager::GetInstance()->AddUnit(std::move(stone));

		// �w�i�摜����
		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
	}

	playerGuide = Sprite::Create((UINT)SpriteName::PLAYERGUIDE_SP, { 1000.0f,600.0f }); // �������
	menu = Sprite::Create((UINT)SpriteName::MENU_SP, { 0.0f,0.0f }); // ���j���[
	enemyGuides = Sprite::Create((UINT)SpriteName::ENEMYGUIDES_SP, { 0.0f,0.0f });; // �G�̐���
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	// �X�V
	UnitManager::GetInstance()->Update();
	intervalTimter->Timer();
	
	// ���j���[��ʂ�W�J
	if (KeyInput::GetInstance()->Trigger(DIK_TAB))
	{
		
	}

	// �V�[���ړ�
	if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // �v���C�������݂��Ă���ꍇ
	{
		if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // �G��S�ł�������
		{
			p_game_scene->GetAudio()->PlaySE("clear.wav", 0.1f);
			p_game_scene->ChangeState(new Clear); // �N���A�V�[����
		}
	}
	else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // �v���C�������݂��Ă��Ȃ��ꍇ
	{
		p_game_scene->GetAudio()->PlaySE("sakebi.wav",0.5f);
		p_game_scene->ChangeState(new Over); // �I�[�o�[�V�[����
	}
}

void XIIlib::Play::Draw()
{
	AttackAreaManager::GetInstance()->Draw();
	// 3D�`��
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// �X�v���C�g�`��
	//intervalTimter->Draw();
	playerGuide->Draw();
	menu->Draw();
	//enemyGuides->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// �w�i�X�v���C�g�`��
	spStageBG1->Draw();
}

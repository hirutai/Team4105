#include "Play.h"
#include "GameScene.h"
#include "End.h"
#include "../Input/KeyInput.h"
#include "../Tool/DebugJISText.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../GameObject/IntervalTimer.h"

XIIlib::Play::Play()
{
	// Initialize�Ƃقړ���

}

XIIlib::Play::~Play()
{
	// �|�C���^�g������̖�����
	delete intervalTimter;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	// IntervalTimer new�Ə�����
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(4, 5);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);

	// Scene�؂�ւ����Ɉ�x�ʂ鏈��
	std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
	std::shared_ptr<Knight> knight = std::move(Knight::Create(7, 7));
	std::shared_ptr<Knight> knight2 = std::move(Knight::Create(6, 2));
	std::shared_ptr<Knight> knight3 = std::move(Knight::Create(7, 0));
	std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(7, 1));
	std::shared_ptr<Bishop> bishop2 = std::move(Bishop::Create(3, 5));
	std::shared_ptr<Rook> rook = std::move(Rook::Create(3, 6));
	std::shared_ptr<Rook> rook2 = std::move(Rook::Create(6, 6));

	UnitManager::GetInstance()->AddUnit(std::move(king));
	UnitManager::GetInstance()->AddUnit(std::move(knight));
	UnitManager::GetInstance()->AddUnit(std::move(knight2));
	UnitManager::GetInstance()->AddUnit(std::move(knight3));
	UnitManager::GetInstance()->AddUnit(std::move(bishop));
	UnitManager::GetInstance()->AddUnit(std::move(bishop2));
	UnitManager::GetInstance()->AddUnit(std::move(rook));
	UnitManager::GetInstance()->AddUnit(std::move(rook2));

}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	// �X�V
	UnitManager::GetInstance()->Update();
	AttackAreaManager::GetInstance()->Draw();

	intervalTimter->Timer();

	if (KeyInput::GetInstance()->Trigger(DIK_SPACE)) {
		p_game_scene->ChangeState(new End);
	}

	DebugJISText::GetInstance()->Print("�v���C", 0, 0, 1);
}

void XIIlib::Play::Draw()
{
	// 3D�`��
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// �X�v���C�g�`��
	intervalTimter->Draw();
}

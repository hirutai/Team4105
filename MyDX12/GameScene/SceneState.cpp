#include "SceneState.h"
#include "GameScene.h"
#include "../GameObject/King.h"
#include "../GameObject/Rook.h"
#include "../GameObject/Bishop.h"
#include "../GameObject/Knight.h"
#include "../GameObject/Yankee.h"
#include "../GameObject/Boss.h"
#include "../GameObject/BossCollision.h"
#include "../GameObject/Stone.h"
#include "../GameObject/UnitManager.h"
#include"../GameObject/AttackAreaManager.h"
#include "../Tool/CSVLoader.h"
#include "../3D/Object3D.h"

using namespace XIIlib;
StageNumber SceneState::stageNum = StageNumber::DEBUG;
GamePAD_XInput* SceneState::gamePad_ = nullptr;
Phase SceneState::phase = Phase::CameraDirecting;
std::unique_ptr<Object3D> SceneState::backStages[3] = {};

void XIIlib::SceneState::CommonUpdate(GameScene* p_game_scene)
{
	if (movingScene) {
		Update(p_game_scene);
	}
	else {
		// �V�[���̑J�ڂ��������Ă��邩�H
		if (p_game_scene->DrawScreen(TransitionType::OPEN)) {
			movingScene = true;
		}
	}

}

void XIIlib::SceneState::CreateUnitsPosition(StageNumber stageNum, std::string fileName)
{
	// �X�e�[�W�ԍ����i�[
	this->stageNum = stageNum;

	// �X�e�[�W�ԍ��ɂ���Đ���
	if (stageNum == StageNumber::EASY)
	{

		//CSVLoader::CSVLoad("stage1");

		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 6));
		std::shared_ptr<Yankee> yankee1 = std::move(Yankee::Create(1, 6));
		std::shared_ptr<Yankee> yankee2 = std::move(Yankee::Create(6, 1));


		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(yankee1));
		UnitManager::GetInstance()->AddUnit(std::move(yankee2));

	}
	else if (stageNum == StageNumber::NORMAL)
	{
		//CSVLoader::CSVLoad("stage2");
		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(6, 3));
		std::shared_ptr<Bishop> bishop2 = std::move(Bishop::Create(4, 5));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(3, 6));
		std::shared_ptr<Rook> rook2 = std::move(Rook::Create(1, 5));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 3));
		std::shared_ptr<Yankee> yankee2 = std::move(Yankee::Create(5, 1));
		std::shared_ptr<Yankee> yankee3 = std::move(Yankee::Create(7, 4));
		std::shared_ptr<Yankee> yankee4 = std::move(Yankee::Create(2, 4));
		//std::shared_ptr<Stone> stone = std::move(Stone::Create(6, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(bishop2));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(rook2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));
		UnitManager::GetInstance()->AddUnit(std::move(yankee2));
		UnitManager::GetInstance()->AddUnit(std::move(yankee3));
		UnitManager::GetInstance()->AddUnit(std::move(yankee4));
		//UnitManager::GetInstance()->AddUnit(std::move(stone));
	}
	else if (stageNum == StageNumber::HARD)
	{
		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 6));
		std::shared_ptr<Yankee> yankee1 = std::move(Yankee::Create(1, 6));
		std::shared_ptr<Boss> boss = std::move(Boss::Create(4, 7));
		std::shared_ptr<BossCollision> bossCollision1 = std::move(BossCollision::Create(2, 7));
		std::shared_ptr<BossCollision> bossCollision2 = std::move(BossCollision::Create(3, 7));
		std::shared_ptr<BossCollision> bossCollision3 = std::move(BossCollision::Create(5, 7));


		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));
		UnitManager::GetInstance()->AddUnit(std::move(yankee1));
		UnitManager::GetInstance()->AddUnit(std::move(boss));
		UnitManager::GetInstance()->AddUnit(std::move(bossCollision1));
		UnitManager::GetInstance()->AddUnit(std::move(bossCollision2));
		UnitManager::GetInstance()->AddUnit(std::move(bossCollision3));
	}
	else
	{
		CSVLoader::CSVLoad(fileName);
		std::vector<std::vector<int>>unitsPos(CSVLoader::BOARD_NUM,std::vector<int>(CSVLoader::BOARD_NUM));
		unitsPos = CSVLoader::GetMapVector();

		// Scene�؂�ւ����Ɉ�x�ʂ鏈��
		for (int y = 0; y < unitsPos.size(); y++)
		{
			for (int x = 0; x < unitsPos.size(); x++)
			{
				if (unitsPos[y][x] == 1)
				{
					std::shared_ptr<King> king = std::move(King::CreateKing(x, y));
					UnitManager::GetInstance()->AddUnit(std::move(king));
				}
				else if (unitsPos[y][x] == 2)
				{
					std::shared_ptr<Rook> rook = std::move(Rook::Create(x, y));
					
					UnitManager::GetInstance()->AddUnit(std::move(rook));
					
				}
				else if (unitsPos[y][x] == 3)
				{
					std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(x, y));
					UnitManager::GetInstance()->AddUnit(std::move(bishop));
				}
				else if (unitsPos[y][x] == 4)
				{
					std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(x, y));
					UnitManager::GetInstance()->AddUnit(std::move(yankee));
					
				}
				else if (unitsPos[y][x] == 5)
				{
					std::shared_ptr<Boss> boss = std::move(Boss::Create(x, y));
					UnitManager::GetInstance()->AddUnit(std::move(boss));
				}
			}
		}
	}

	// ��x�����X�V
	UnitManager::GetInstance()->Update();
}

void XIIlib::SceneState::BackStagesInit()
{
	/*for (int i = 0; i < 3; ++i)
	{
		backStages[i].reset(Object3D::Create(Model::CreateFromOBJ("stage1_all")));
		backStages[i]->scale = Math::Vector3({ 3.0f,3.0f,3.0f });
		backStages[i]->position = Math::Vector3({ 0.0f,-1.0f,0.0f });
		backStages[i]->Update();
	}*/

	backStages[0].reset(Object3D::Create(Model::CreateFromOBJ("stage1_all")));
	backStages[0]->scale = Math::Vector3({ 3.0f,3.0f,3.0f });
	backStages[0]->position = Math::Vector3({ 0.0f,-1.0f,0.0f });
	backStages[0]->Update();
}

void XIIlib::SceneState::BackStagesDraw()
{
	backStages[0]->Update();
	// ���f���̕`��(.obj)
	Object3D::PreDraw();
	backStages[0]->Draw();
	Object3D::PostDraw();
}

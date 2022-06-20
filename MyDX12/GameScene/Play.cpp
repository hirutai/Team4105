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
#include "../Tool/Easing.h"

XIIlib::Play::Play()
{
	// Initialize‚Æ‚Ù‚Ú“¯‚¶

}

XIIlib::Play::~Play()
{
	// ƒ|ƒCƒ“ƒ^g‚Á‚½‚â‚Â‚Ì–„‘’ê
	delete enemyGuides; // “G‚Ìà–¾
	delete menu; // ƒƒjƒ…[
	delete playerGuide; // ‘€ìà–¾
	delete spStageBG1;
	delete intervalTimter;
}

void XIIlib::Play::Initialize(GameScene* p_game_scene)
{
	
	// IntervalTimer new‚Æ‰Šú‰»
	intervalTimter = new IntervalTimer();
	intervalTimter->Initialize(4, 5);
	UnitManager::GetInstance()->SetIntervalTimer(intervalTimter);
	
	if (stageNum == StageNumber::EASY)
	{
		// SceneØ‚è‘Ö‚¦‚Éˆê“x’Ê‚éˆ—
		std::shared_ptr<King> king = std::move(King::CreateKing(1, 0));
		std::shared_ptr<Bishop> bishop = std::move(Bishop::Create(2, 3));
		std::shared_ptr<Rook> rook = std::move(Rook::Create(4, 6));
		std::shared_ptr<Yankee> yankee = std::move(Yankee::Create(3, 6));

		UnitManager::GetInstance()->AddUnit(std::move(king));
		UnitManager::GetInstance()->AddUnit(std::move(bishop));
		UnitManager::GetInstance()->AddUnit(std::move(rook));
		UnitManager::GetInstance()->AddUnit(std::move(yankee));

		// ”wŒi‰æ‘œ¶¬
		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
	}
	else if (stageNum == StageNumber::NORMAL)
	{
		// SceneØ‚è‘Ö‚¦‚Éˆê“x’Ê‚éˆ—
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

		// ”wŒi‰æ‘œ¶¬
		spStageBG1 = Sprite::Create((UINT)SpriteName::STAGEBG1_SP, { 0.0f,0.0f });
	}

	playerGuide = Sprite::Create((UINT)SpriteName::PLAYERGUIDE_SP, { 1000.0f,600.0f }); // ‘€ìà–¾
	menu = Sprite::Create((UINT)SpriteName::MENU_SP, { 0.0f,10.0f }); // ƒƒjƒ…[
	enemyGuides = Sprite::Create((UINT)SpriteName::ENEMYGUIDES_SP, {0.0f,0.0f});; // “G‚Ìà–¾
	p_game_scene->GetAudio()->PlayBGM("yankeeBGM.wav");
}

void XIIlib::Play::Update(GameScene* p_game_scene)
{
	if (movingScene) {
#pragma region ƒƒjƒ…[ˆ—
<<<<<<< HEAD
=======

>>>>>>> parent of 27be801 (ã‚·ãƒ¼ãƒ³é·ç§»ã‚¢ãƒ‹ãƒ¡ãƒ¼ã‚·ãƒ§ãƒ³ã®ã‚¹ãƒ”ãƒ¼ãƒ‰èª¿æ•´ã€æ›´æ–°å‡¦ç†ã®æ”¹å–„)
		// ƒƒjƒ…[‰æ–Ê‚ğ“WŠJA•Â‚¶‚é
		if (KeyInput::GetInstance()->Trigger(DIK_TAB))
		{
			if (menuExists && easingCount >= MAX_EASING_COUNT)
			{
				// ƒ[ƒClear
				easingCount = 0;
				exitFlag = true;
				menuExists = false;
			}
			else
			{
				menuExists = true;
			}
<<<<<<< HEAD
		}
		
		if (menuExists)
=======
		}

		if (menuExists)
		{
			float posX = 0;
			float posY = 0;
			// count‚ªƒ}ƒbƒNƒX‚É“’B‚·‚é‚Ü‚Å
			if (easingCount <= MAX_EASING_COUNT)
			{
				posX = Easing::EaseInOutElastic(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
				posY = Easing::EaseInOutElastic(easingCount, -winSize.y, winSize.y, MAX_EASING_COUNT);
				easingCount++;
			}
			enemyGuides->SetPosition({ posX,posY });
		}

		if (exitFlag)
>>>>>>> parent of 27be801 (ã‚·ãƒ¼ãƒ³é·ç§»ã‚¢ãƒ‹ãƒ¡ãƒ¼ã‚·ãƒ§ãƒ³ã®ã‚¹ãƒ”ãƒ¼ãƒ‰èª¿æ•´ã€æ›´æ–°å‡¦ç†ã®æ”¹å–„)
		{
			float posX = 0;
			float posY = 0;
			// count‚ªƒ}ƒbƒNƒX‚É“’B‚·‚é‚Ü‚Å
			if (easingCount <= MAX_EASING_COUNT)
			{
<<<<<<< HEAD
				posX = Easing::EaseInOutBack(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
				posY = Easing::EaseInOutBack(easingCount, -winSize.y, winSize.y, MAX_EASING_COUNT);
				easingCount++;
				enemyGuides->SetPosition({ posX,posY });
			}
		}
		

		if (exitFlag)
		{
			float posX = 0;
			float posY = 0;
			// count‚ªƒ}ƒbƒNƒX‚É“’B‚·‚é‚Ü‚Å
			if (easingCount <= MAX_EASING_COUNT)
			{
				posX = Easing::EaseInOutBounce(easingCount, winSize.x, -winSize.x, MAX_EASING_COUNT);
				posY = Easing::EaseInOutBounce(easingCount, winSize.y, -winSize.y, MAX_EASING_COUNT);
				easingCount++;
				enemyGuides->SetPosition({ posX,posY });
			}
			else {
				easingCount = 0;
				exitFlag = false;
			}
		}
=======
				posX = Easing::EaseInOutBounce(easingCount, enemyGuides->GetPosition().x, -winSize.x, MAX_EASING_COUNT);
				posY = Easing::EaseInOutBounce(easingCount, enemyGuides->GetPosition().y, -winSize.y, MAX_EASING_COUNT);
				easingCount++;
				enemyGuides->SetPosition({ posX,posY });
			}
			else {
				easingCount = 0;
				exitFlag = false;
				menuExists = false;
			}

		}

		if (menuExists)
		{
			float posX = 0;
			// count‚ªƒ}ƒbƒNƒX‚É“’B‚·‚é‚Ü‚Å
			if (easingCount <= MAX_EASING_COUNT)
			{
				posX = Easing::EaseInOutCubic(easingCount, -winSize.x, winSize.x, MAX_EASING_COUNT);
			}
			enemyGuides->SetPosition({ posX,0 });
			easingCount++;
		}

>>>>>>> parent of 27be801 (ã‚·ãƒ¼ãƒ³é·ç§»ã‚¢ãƒ‹ãƒ¡ãƒ¼ã‚·ãƒ§ãƒ³ã®ã‚¹ãƒ”ãƒ¼ãƒ‰èª¿æ•´ã€æ›´æ–°å‡¦ç†ã®æ”¹å–„)
		// ƒƒjƒ…[‚ª“WŠJ‚³‚ê‚Ä‚¢‚é‚È‚çreturn
		if (menuExists)return;
#pragma endregion 

#pragma region Game Updateˆ—
		// XV
		UnitManager::GetInstance()->Update();
		intervalTimter->Timer();
		// ƒV[ƒ“ˆÚ“®
		if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // ƒvƒŒƒCƒ„‚ª‘¶İ‚µ‚Ä‚¢‚éê‡
		{
			if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // “G‚ğ‘S–Å‚³‚¹‚½
			{
				trigSpace = true;
			}
		}
		else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // ƒvƒŒƒCƒ„‚ª‘¶İ‚µ‚Ä‚¢‚È‚¢ê‡
		{
			trigSpace = true;
		}
#pragma endregion
		if (trigSpace) {
			if (p_game_scene->DrawScreen(false)) {
				if (UnitManager::GetInstance()->GetUnitIDElements("King") >= 0) // ƒvƒŒƒCƒ„‚ª‘¶İ‚µ‚Ä‚¢‚éê‡
				{
					if (UnitManager::GetInstance()->GetAllUnitCount() - 1 == 0) // “G‚ğ‘S–Å‚³‚¹‚½
					{
						p_game_scene->ChangeState(new Clear); // ƒNƒŠƒAƒV[ƒ“‚Ö
					}
				}
				else if (UnitManager::GetInstance()->GetUnitIDElements("King") < 0) // ƒvƒŒƒCƒ„‚ª‘¶İ‚µ‚Ä‚¢‚È‚¢ê‡
				{
					p_game_scene->ChangeState(new Over); // ƒI[ƒo[ƒV[ƒ“‚Ö
				}
			}
		}
	}
	else {
		// ƒV[ƒ“‚Ì‘JˆÚ‚ªŠ®—¹‚µ‚Ä‚¢‚é‚©H
		if (p_game_scene->DrawScreen(true)) {
			movingScene = true;
		}
	}
}

void XIIlib::Play::Draw()
{
	AttackAreaManager::GetInstance()->Draw();
	// 3D•`‰æ
	UnitManager::GetInstance()->Draw();
}

void XIIlib::Play::DrawTex()
{
	// ƒXƒvƒ‰ƒCƒg•`‰æ
	//intervalTimter->Draw();
	playerGuide->Draw();
	menu->Draw();
	// ƒƒjƒ…[‚ª“WŠJ‚³‚ê‚Ä‚¢‚È‚¢‚È‚çreturn
	if (!menuExists || exitFlag)return;
	enemyGuides->Draw();
	menu->Draw();
}

void XIIlib::Play::DrawBackground()
{
	// ”wŒiƒXƒvƒ‰ƒCƒg•`‰æ
	spStageBG1->Draw();
}

#include "Boss.h"
#include "BossHP.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include"../GameObject/Stone.h"
#include "../Tool/DebugText.h"
#include "../Tool/Easing.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>
#include <random>
#include "ModelLoader.h"

XIIlib::Boss::Boss()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = BossHP::GetInstance()->GetBossHP();
	_attack_point = 3;
	_defense_point = 1;
}

XIIlib::Boss::~Boss()
{
	delete attackTimer;
	delete object3d;
	delete object3d2;
}

std::shared_ptr<XIIlib::Boss> XIIlib::Boss::Create(int point_x, int point_z)
{
	std::shared_ptr<Boss> boss = std::make_shared<Boss>();
	boss.get()->SetElementStock(point_x, point_z);
	boss.get()->Initialize();

	return std::move(boss);
}

void XIIlib::Boss::Initialize()
{
	BossHP::GetInstance()->Initialize();
	// クラスネーム取得
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();

	// 大きさと座標の定数
	const Math::Vector3 scale = { 1.5f,1.5f,1.5f };
	const Math::Vector3 position = { Common::ConvertTilePosition(element_stock.a) - 2.7f,1.0f, Common::ConvertTilePosition(element_stock.b) };

	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS));
	object3d->scale = scale;
	object3d->position = position;

	object3d2 = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS2));
	object3d2->scale = scale;
	object3d2->position = position;
	
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();

	//攻撃ゲージの秒数
	SetAttackTimer(300,CountType::FRAME);

	nextPoint = { 0,0 };
	//isDrawTimer = true;
	fallFlag = true;
	InitAttackDisplay();
}

void XIIlib::Boss::Update()
{
	std::cout << "HP" << _hit_point << std::endl;

	if (switching == true)
	{
		switchingCount++;
	}
	if (switchingCount >= 30)
	{
		switching = false;
		switchingCount = 0;
	}
	_hit_point = BossHP::GetInstance()->GetBossHP();

	if (_hit_point <= BossHP::GetInstance()->GetDefaultBossHP() / 2 && bossType == BossType::normal && bossState == BossState::wait)
	{
		bossType = BossType::strong;
		SetAttackTimer(180, CountType::FRAME);
		std::shared_ptr<Stone> stone = std::move(Stone::Create(1, 7));
		std::shared_ptr<Stone> stone2 = std::move(Stone::Create(6, 7));
		std::shared_ptr<Stone> stone3 = std::move(Stone::Create(1, 6));
		std::shared_ptr<Stone> stone4 = std::move(Stone::Create(2, 6));
		std::shared_ptr<Stone> stone5 = std::move(Stone::Create(3, 6));
		std::shared_ptr<Stone> stone6 = std::move(Stone::Create(4, 6));
		std::shared_ptr<Stone> stone7 = std::move(Stone::Create(5, 6));
		std::shared_ptr<Stone> stone8 = std::move(Stone::Create(6, 6));
		UnitManager::GetInstance()->AddUnit(std::move(stone));
		UnitManager::GetInstance()->AddUnit(std::move(stone2));
		UnitManager::GetInstance()->AddUnit(std::move(stone3));
		UnitManager::GetInstance()->AddUnit(std::move(stone4));
		UnitManager::GetInstance()->AddUnit(std::move(stone5));
		UnitManager::GetInstance()->AddUnit(std::move(stone6));
		UnitManager::GetInstance()->AddUnit(std::move(stone7));
		UnitManager::GetInstance()->AddUnit(std::move(stone8));
	}
	BossHP::GetInstance()->Update();
	if (!determinateMoveAction) {
		// 駒の行動
		Action();

		// タイマーの更新
		attackTimer->Timer();

		pos = object3d->position;
	}

	if (determinateMoveAction) {
		// 攻撃処理
		Attack();
	}

	object3d->Update();
	object3d2->Update();
	// 座標設定
	attackTimer->SetPosition(object3d->position + Math::Vector3(0.0f, 18.0f, 0.0f));
}

void XIIlib::Boss::Draw()
{
	if (switchingCount >= 1)
	{
		object3d2->Draw();
		if (switchingCount <= 15)
		{
			object3d2->position.y += 0.3f;
		}
		if (switchingCount >= 15 && switchingCount <= 30)
		{
			object3d2->position.y -= 0.3f;
		}
	}
	else {
		object3d->Draw();
	}
}

void XIIlib::Boss::Action()
{
	isAttack = true;
	preElement_stock = kingPos;

	Move();
	
	if (attackTimer->SizeZeroFlag())
	{
		bossState = BossState::wait;
		switching = true;
		// Attack表示の初期化
		InitAttackDisplay();
		// 攻撃を開始するのでtrue
		determinateMoveAction = true;
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag())
	{
		int element = UnitManager::GetInstance()->GetUnitIDElements("King");
		if (element != -1)
		{
			std::shared_ptr<King> p_king =
				std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
			kingPos = p_king->GetElementStock();
		}
		Target();
		if (bossType == BossType::strong)
		{
			if (bossAttackSelect != 0)
			{
				srand(time(NULL));
				int count = 0;
				int breakCnt = 0;
				for (int i = 0; i < METEORS_MAX; i++)
				{
					numbersA[i] = -1;
					numbersB[i] = -1;
				}
				//メテオの座標生成
				while (1)
				{
					breakCnt++;
					if (breakCnt >= 150)break;//バグが生まれたとき用の強制終了
					//仮のランダムな座標を入れる
					Math::Point2 numbers;
					numbers.a = meteorAMin + (int)(rand() * (meteorAMax - meteorAMin + 1) / (1 + RAND_MAX));
					numbers.b = meteorBMin + (int)(rand() * (meteorBMax - meteorBMin + 1) / (1 + RAND_MAX));

					if (UnitManager::GetInstance()->AllOnUnit(numbers))continue;//仮の座標に誰かがいるか
					if (CheckMeteorArea(numbers))continue;//他のメテオの座標と被っているか
					//居なかったら座標を代入
					numbersA[count] = numbers.a;
					numbersB[count] = numbers.b;
					count++;

					if (count >= METEORS_MAX)break;
				}
			}	
		}	
		// Attack表示の初期化
		InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag())
	{
		//色を塗る
		if (count % DISPLAY_FRAME == 0)
		{
			if (bossAttackSelect == 0) {
				tileDeth[abs(tileNum - (MAX_TILE - 1))] = true;
			}
			else
			{
				tileDeth[tileNum] = true;
			}
			tileNum++;
		}
		count++;
		if (bossType == BossType::normal)
		{
			if (bossAttackSelect == 0)
			{
				// 縦
				for (int i = MAX_TILE - 1; i >= 0; --i)
				{
					if (tileDeth[i])
					{
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::ENEMY);
					}
				}
			}
			else
			{

				// 横
				for (int j = 0; j < MAX_TILE; ++j)
				{
					if (tileDeth[j])
					{
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::ENEMY);
					}
				}
			}
		}
		else if (bossType == BossType::strong)
		{
			if (bossAttackSelect == 0)
			{
				Math::Point2 anchorLUpos = { kingPos.a - 1, kingPos.b + 1 };//左上
				for (int i = 0; i < 3; i++)//3*3の点攻撃
				{
					for (int j = 0; j < 3; j++)
					{
						if (Common::GetExceptionPoint(anchorLUpos.a + j) || Common::GetExceptionPoint(anchorLUpos.b - i))continue;
						
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(anchorLUpos.a + j, anchorLUpos.b - i), (int)_PositionType::ENEMY);
					}
				}
			}
			else
			{
				for (int i = 0; i < METEORS_MAX; i++)//メテオ
				{	
					UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::ENEMY);					
				}
			}
		}
		bossState = BossState::attack;
	}

	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		//タイル表示
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		//密着したら弾く
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	}	
}

void XIIlib::Boss::Target()
{
	int BossjMin = bossMin;
	int BossjMax = bossMax;
	bossTileRand = bossMin + (int)(rand() * (bossMax - bossMin + 1) / (1 + RAND_MAX));

	int BossAttackMin = bossAttackMin;
	int BossAttackMax = bossAttackMax;
	bossAttackSelect = bossAttackMin + (int)(rand() * (bossAttackMax - bossAttackMin + 1) / (1 + RAND_MAX));

	std::vector<int> v(6);
	std::iota(v.begin(), v.end(), 0);
	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v.begin(), v.end(), engine);

	bossTileRand = v[bossTileRand] + 1;
}

void XIIlib::Boss::Attack()
{
	Math::Point2 temp = element_stock;
	if (bossType == BossType::normal)
	{
		if (bossAttackSelect == 0)
		{
			for (int i = 0; i < MAX_TILE; i++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::BOSS);
			}
		}
		else
		{
			for (int j = 0; j < MAX_TILE; j++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::BOSS);
			}
		}
	}
	else if (bossType == BossType::strong)
	{
		if (bossAttackSelect == 0)
		{
			Math::Point2 anchorLUpos = { kingPos.a - 1, kingPos.b + 1 };//左上
			for (int i = 0; i < 3; i++)//3*3の点攻撃
			{
				for (int j = 0; j < 3; j++)
				{
					if (Common::GetExceptionPoint(anchorLUpos.a + j) || Common::GetExceptionPoint(anchorLUpos.b - i))continue;
					UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(anchorLUpos.a + j, anchorLUpos.b - i), (int)_PositionType::BOSS);
				}
			}
		}
		else
		{
			for (int i = 0; i < METEORS_MAX; i++)//メテオ
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::BOSS);
			}
		}
	}

	// 攻撃フレームの加算
	attackFrameCnt++;
	// 最大攻撃フレームより攻撃フレームが多ければ攻撃は終了
	if (MAX_ATTACK_FRAME <= attackFrameCnt)
	{
		// 攻撃が完全に終わったのでfalse
		determinateMoveAction = false;
		// 攻撃フレームを0クリア
		attackFrameCnt = 0;
	}
}

void XIIlib::Boss::Move()
{
	
}

bool XIIlib::Boss::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	Math::Point2 dif = kingPos - element_stock;
	// xとzの絶対値が一緒だったら攻撃範囲にいる範囲
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Boss::AttackAreaDraw()
{
	
}

void XIIlib::Boss::IniState()
{
	isAttack = false;
}

void XIIlib::Boss::CreateAttackArea()
{

}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

void XIIlib::Boss::InitAttackDisplay()
{
	if (count >= DISPLAY_FRAME * MAX_TILE)
	{
		count = 0;
		tileNum = 0;
		for (int i = 0; i < MAX_TILE; i++)
		{
			tileDeth[i] = false;
		}
	}
}

bool XIIlib::Boss::CheckMeteorArea(Math::Point2 meteorPos)
{
	for (int i = 0; i < METEORS_MAX; i++)
	{
		//meteorPosがMETEORS_MAXの回数中一回でも一緒になればtrueを返す
		if (meteorPos.a == numbersA[i] && meteorPos.b == numbersB[i])return true;
	}
	return false;
}

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
#include "Boss.h"
#include "BossHP.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
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
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS));
	object3d->scale = Math::Vector3({ 1.5f,1.5f,1.5f });
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	object3d->position.x -= 2.7f;

	object3d2 = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS2));
	object3d2->scale = Math::Vector3({ 1.5f,1.5f,1.5f });
	object3d2->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	object3d2->position.x -= 2.7f;
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
	BossHP::GetInstance()->Update();
	// タイマーの更新
	attackTimer->Timer();

	Action();

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
		switching = true;
		InitAttackDisplay();
		Attack();
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag())
	{
		Target();

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
			for (int j = 0; j < MAX_TILE;++j)
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

void XIIlib::Boss::SetHitDamage(float attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
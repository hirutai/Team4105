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
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();

	//攻撃ゲージの秒数
	SetAttackTimer(300,CountType::FRAME);

	nextPoint = { 0,0 };
}

void XIIlib::Boss::Update()
{
	std::cout << "HP" << _hit_point << std::endl;

	_hit_point = BossHP::GetInstance()->GetBossHP();
	BossHP::GetInstance()->Update();
	// タイマーの更新
	attackTimer->Timer();

	Action();

	object3d->Update();
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// 座標設定
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Boss::Action()
{
	isAttack = true;
	preElement_stock = kingPos;

	Move();
	
	if (attackTimer->SizeZeroFlag())
	{
		Attack();
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag())
	{
		Target();
	}
	else if (attackTimer->SizeThirdBelowFlag())
	{
		//色を塗る
		//if (count % 10 == 0)
		//{
		//	int num = count * 0.1;
		//	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, num), (int)_PositionType::ENEMY);
		//}
		//if (count >= 70)
		//{
		//	count = 0;
		//}
		//count++;
		if (bossAttackSelect == 0)
		{
			for (int i = 0; i <= 7; i++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::ENEMY);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::ENEMY);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::ENEMY);
			}
		}
		else
		{
			for (int j = 0; j <= 7; j++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::ENEMY);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::ENEMY);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::ENEMY);
			}
		}
	}
	

	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		//タイル表示
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::ENEMY);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		////密着したら弾く
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
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
	if (bossAttackSelect == 0)
	{
		for (int i = 0; i <= 7; i++)
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::ENEMY);
		}
	}
	else
	{
		for (int j = 0; j <= 7; j++)
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::ENEMY);
		}
	}
	//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::ENEMY);

}

void XIIlib::Boss::Attack()
{
	/*Math::Point2 temp = element_stock;
	if (bossAttackSelect == 0)
	{
		for (int i = 0; i <= 7; i++)
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::BOSS);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::BOSS);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::BOSS);
		}
	}
	else
	{
		for (int j = 0; j <= 7; j++)
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::BOSS);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::BOSS);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::BOSS);
		}
	}*/
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

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
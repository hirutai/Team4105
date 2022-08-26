#include "BossAttack.h"
#include "UnitManager.h"
#include "Common.h"
#include <iostream>
#include <numeric>
#include <random>

XIIlib::BossAttack* XIIlib::BossAttack::GetInstance()
{
	static BossAttack bossAt;
	return &bossAt;
}

void XIIlib::BossAttack::DispTileDeathControl()
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
}

void XIIlib::BossAttack::CreateMeteorPosition()
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

bool XIIlib::BossAttack::CheckMeteorArea(const Math::Point2& meteorPos)
{
	for (int i = 0; i < METEORS_MAX; i++)
	{
		//meteorPosがMETEORS_MAXの回数中一回でも一緒になればtrueを返す
		if (meteorPos.a == numbersA[i] && meteorPos.b == numbersB[i])return true;
	}
	return false;
}

void XIIlib::BossAttack::InitAttackDisplay()
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

void XIIlib::BossAttack::Vertical3LineDisplay()
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

void XIIlib::BossAttack::Horizontal3LineDisplay()
{
	// 横
	for (int i = 0; i < MAX_TILE; ++i)
	{
		if (tileDeth[i])
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand + 1), (int)_PositionType::ENEMY);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand - 1), (int)_PositionType::ENEMY);
		}
	}
}

void XIIlib::BossAttack::OneMeteor3x3Display(const Math::Point2& kingPos)
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

void XIIlib::BossAttack::RandomMeteor1x1Display()
{
	//メテオ
	for (int i = 0; i < METEORS_MAX; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::ENEMY);
	}
}

void XIIlib::BossAttack::Target()
{
	int BossjMin = bossMin;
	int BossjMax = bossMax;
	bossTileRand = bossMin + (int)(rand() * (bossMax - bossMin + 1) / (1 + RAND_MAX));

	std::vector<int> v(6);
	std::iota(v.begin(), v.end(), 0);
	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v.begin(), v.end(), engine);

	bossTileRand = v[bossTileRand] + 1;
}

void XIIlib::BossAttack::Vertical3LineAttack()
{
	for (int i = 0; i < MAX_TILE; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::BOSS);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::BOSS);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::BOSS);
	}
}

void XIIlib::BossAttack::Horizontal3LineAttack()
{
	for (int i = 0; i < MAX_TILE; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand), (int)_PositionType::BOSS);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand + 1), (int)_PositionType::BOSS);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand - 1), (int)_PositionType::BOSS);
	}
}

void XIIlib::BossAttack::OneMeteor3x3Attack(const Math::Point2& kingPos)
{
	// 1点メテオ攻撃 3x3
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

void XIIlib::BossAttack::RandomMeteor1x1Attack()
{
	// (仮)ランダムメテオ攻撃 1x1
	for (int i = 0; i < METEORS_MAX; i++)//メテオ
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::BOSS);
	}
}

void XIIlib::BossAttack::KnockBackAttack(const Math::Point2& pos)
{
	//タイル表示
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 2, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 1, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a,     pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 1, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b - 1), (int)_PositionType::ENEMY);
	//密着したら弾く
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 2, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 1, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a,     pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 1, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
}

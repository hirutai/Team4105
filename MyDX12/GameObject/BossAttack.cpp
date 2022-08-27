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

void XIIlib::BossAttack::DispTileDeathControl(const int& bossAttackSelect)
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
	int meteorcount = 0;
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
		numbersA[meteorcount] = numbers.a;
		numbersB[meteorcount] = numbers.b;
		meteorcount++;

		if (meteorcount >= METEORS_MAX)break;
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
	// 全てのタイルが表示されているなら初期化
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

/*void XIIlib::BossAttack::RandomMeteor1x1Display()
{
	//メテオ
	for (int i = 0; i < METEORS_MAX; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::ENEMY);
	}
}*/

void XIIlib::BossAttack::Target()
{
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
	// ---------- 表示 ----------
	meteorsCount++;
	//���e�I1�`��
	if (meteorsCount >= -20 && meteorsCount <= 0)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[0], numbersB[0]), (int)_PositionType::ENEMY);
	}
	//���e�I2
	if (meteorsCount >= 0 && meteorsCount <= 20)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[1], numbersB[1]), (int)_PositionType::ENEMY);
	}
	//���e�I3
	if (meteorsCount >= 20 && meteorsCount <= 40)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[2], numbersB[2]), (int)_PositionType::ENEMY);
	}
	//���e�I4
	if (meteorsCount >= 40 && meteorsCount <= 60)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[3], numbersB[3]), (int)_PositionType::ENEMY);
	}
	//���e�I5
	if (meteorsCount >= 60 && meteorsCount <= 80)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[4], numbersB[4]), (int)_PositionType::ENEMY);
	}
	//���e�I6
	if (meteorsCount >= 80 && meteorsCount <= 100)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[5], numbersB[5]), (int)_PositionType::ENEMY);
	}
	//���e�I7
	if (meteorsCount >= 100 && meteorsCount <= 120)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[6], numbersB[6]), (int)_PositionType::ENEMY);
	}
	//���e�I8
	if (meteorsCount >= 120 && meteorsCount <= 140)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[7], numbersB[7]), (int)_PositionType::ENEMY);
	}
	//���e�I9
	if (meteorsCount >= 140 && meteorsCount <= 160)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[8], numbersB[8]), (int)_PositionType::ENEMY);
	}
	//���e�I10
	if (meteorsCount >= 160 && meteorsCount <= 180)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[9], numbersB[9]), (int)_PositionType::ENEMY);
	}
	//���e�I11
	if (meteorsCount >= 180 && meteorsCount <= 200)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[10], numbersB[10]), (int)_PositionType::ENEMY);
	}
	//���e�I12
	if (meteorsCount >= 200 && meteorsCount <= 220)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[11], numbersB[11]), (int)_PositionType::ENEMY);
	}
	//���e�I13
	if (meteorsCount >= 220 && meteorsCount <= 240)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[12], numbersB[12]), (int)_PositionType::ENEMY);
	}
	//���e�I14
	if (meteorsCount >= 240 && meteorsCount <= 260)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[13], numbersB[13]), (int)_PositionType::ENEMY);
	}
	//���e�I15
	if (meteorsCount >= 260 && meteorsCount <= 280)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[14], numbersB[14]), (int)_PositionType::ENEMY);
	}

	// ---------- 攻撃 ----------
				//���e�I1�̍U������
	if (meteorsCount >= 0 && meteorsCount <= 20)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[0], numbersB[0]), (int)_PositionType::BOSS);
	}
	//���e�I2
	if (meteorsCount >= 20 && meteorsCount <= 40)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[1], numbersB[1]), (int)_PositionType::BOSS);
	}
	//���e�I3
	if (meteorsCount >= 40 && meteorsCount <= 60)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[2], numbersB[2]), (int)_PositionType::BOSS);
	}
	//���e�I4
	if (meteorsCount >= 60 && meteorsCount <= 80)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[3], numbersB[3]), (int)_PositionType::BOSS);
	}
	//���e�I5
	if (meteorsCount >= 80 && meteorsCount <= 100)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[4], numbersB[4]), (int)_PositionType::BOSS);
	}
	//���e�I6
	if (meteorsCount >= 100 && meteorsCount <= 120)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[5], numbersB[5]), (int)_PositionType::BOSS);
	}
	//���e�I7
	if (meteorsCount >= 120 && meteorsCount <= 140)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[6], numbersB[6]), (int)_PositionType::BOSS);
	}
	//���e�I8
	if (meteorsCount >= 140 && meteorsCount <= 160)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[7], numbersB[7]), (int)_PositionType::BOSS);
	}
	//���e�I9
	if (meteorsCount >= 160 && meteorsCount <= 180)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[8], numbersB[8]), (int)_PositionType::BOSS);
	}
	//���e�I10
	if (meteorsCount >= 180 && meteorsCount <= 200)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[9], numbersB[9]), (int)_PositionType::BOSS);
	}
	//���e�I11
	if (meteorsCount >= 200 && meteorsCount <= 220)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[10], numbersB[10]), (int)_PositionType::BOSS);
	}
	//���e�I12
	if (meteorsCount >= 220 && meteorsCount <= 240)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[11], numbersB[11]), (int)_PositionType::BOSS);
	}
	//���e�I13
	if (meteorsCount >= 240 && meteorsCount <= 260)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[12], numbersB[12]), (int)_PositionType::BOSS);
	}
	//���e�I14
	if (meteorsCount >= 260 && meteorsCount <= 280)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[13], numbersB[13]), (int)_PositionType::BOSS);
	}
	//���e�I15
	if (meteorsCount >= 280 && meteorsCount <= 300)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[14], numbersB[14]), (int)_PositionType::BOSS);
	}

	// メテオsカウントが最大まで行ったら
	if (meteorsCount >= 300)
	{
		// メテオsカウントの初期化
		meteorsCount = -20;
	}

	// (仮)ランダムメテオ攻撃 1x1
	/*for (int i = 0; i < METEORS_MAX; i++)//メテオ
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::BOSS);
	}*/
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

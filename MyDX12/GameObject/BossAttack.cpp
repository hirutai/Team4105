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

void XIIlib::BossAttack::InitAttackDisplay()
{
	// 全てのタイルと付随した変数を初期化
	frameCount = 0;
	tileNum = 0;
	tileSwitch = true;
	for (int i = 0; i < MAX_TILE; i++)
	{
		tilesDispFlag[i] = false;
	}
}

void XIIlib::BossAttack::DispTileDeathControl(const int& bossAttackSelect)
{
	// タイルが全て表示されたら間隔を空けて非表示にする
	if (frameCount >= DISPLAY_FRAME * MAX_TILE) { 
		frameCount = 0;
		tileNum = 0;
		tileSwitch = false;
	}
	// tileDispが8枚trueになるまで通る 
	if (frameCount % DISPLAY_FRAME == 0 && tileNum <= (MAX_TILE - 1))
	{
		if (bossAttackSelect == 0) {
			tilesDispFlag[abs(tileNum - (MAX_TILE - 1))] = tileSwitch;
		}
		else
		{
			tilesDispFlag[tileNum] = tileSwitch;
		}
		tileNum++;
	}
	// フレームカウント
	frameCount++;
}

void XIIlib::BossAttack::Target()
{
	bossTileRand = BOSSTILE_MIN_MAX.a + (int)(rand() * (BOSSTILE_MIN_MAX.b - BOSSTILE_MIN_MAX.a + 1) / (1 + RAND_MAX));
	std::vector<int> v(6);
	std::iota(v.begin(), v.end(), 0);
	// シャッフル
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v.begin(), v.end(), engine);
	bossTileRand = v[bossTileRand] + 1;
}

void XIIlib::BossAttack::CreateMeteorPosition(int& bossAttackSelect)
{
	srand(time(NULL));
	int meteorcount = 0; // メテオの数
	int breakCnt = 0; // 強制終了用カウント変数
	// 配列の初期化
	for (int i = 0; i < METEORS_MAX; i++)
	{
		numbersA[i] = -1;
		numbersB[i] = -1;
	}

	//メテオの座標生成
	while (1)
	{
		// 200回回るまでにメテオの位置が決まらなければ攻撃タイプを変更して強制終了
		breakCnt++;
		if (breakCnt >= 200) {
			bossAttackSelect = 0;
			break;
		}
		//仮のランダムな座標を入れる
		Math::Point2 numbers;
		numbers.a = METEOR_MIN_MAX.a + (int)(rand() * (METEOR_MIN_MAX.b - METEOR_MIN_MAX.a + 1) / (1 + RAND_MAX));
		numbers.b = METEOR_MIN_MAX.a + (int)(rand() * (METEOR_MIN_MAX.b - METEOR_MIN_MAX.a + 1) / (1 + RAND_MAX));

		if (UnitManager::GetInstance()->AllOnUnit(numbers))continue;//仮の座標に誰かがいるか
		if (CheckMeteorArea(numbers))continue;//他のメテオの座標と被っているか

		//居なかったら座標を代入
		numbersA[meteorcount] = numbers.a;
		numbersB[meteorcount] = numbers.b;
		meteorcount++;

		// メテオがMAX値まで格納されたらループを抜ける
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

void XIIlib::BossAttack::AssignPositionType(const std::string& type)
{
	// typeによって代入
	if (type == "Display")
	{
		positionType = (int)_PositionType::ENEMY;
	}
	else if (type == "Attack")
	{
		positionType = (int)_PositionType::BOSS;
	}
}

void XIIlib::BossAttack::Vertical3Line(const std::string& type)
{
	// PositionTypeを判別
	AssignPositionType(type);
	// 縦
	for (int i = MAX_TILE - 1; i >= 0; --i)
	{
		if (tilesDispFlag[i])
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), positionType);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), positionType);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), positionType);
		}
	}
}

void XIIlib::BossAttack::Horizontal3Line(const std::string& type)
{
	// PositionTypeを判別
	AssignPositionType(type);
	// 横
	for (int i = 0; i < MAX_TILE; ++i)
	{
		if (tilesDispFlag[i])
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand), positionType);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand + 1), positionType);
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, bossTileRand - 1), positionType);
		}
	}
}

void XIIlib::BossAttack::OneMeteor3x3(const std::string& type,const Math::Point2& kingPos)
{
	// PositionTypeを判別
	AssignPositionType(type);
	// 左上を基準とする
	Math::Point2 anchorLUpos = { kingPos.a - 1, kingPos.b + 1 };
	// 2重for文で3x3を作りそこにアクセス
	for (int i = 0; i < 3; i++)//3*3の点攻撃
	{
		for (int j = 0; j < 3; j++)
		{
			if (Common::GetExceptionPoint(anchorLUpos.a + j) || Common::GetExceptionPoint(anchorLUpos.b - i))continue;

			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(anchorLUpos.a + j, anchorLUpos.b - i), positionType);
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
}

void XIIlib::BossAttack::KnockBackAttack(const Math::Point2& pos)
{
	//タイル表示
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b    ), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b    ), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 2, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 1, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a,     pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 1, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b - 1), (int)_PositionType::ENEMY);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b - 1), (int)_PositionType::ENEMY);
	//密着したら弾く
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b    ), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b    ), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 2, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 1, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a,     pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 1, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a - 3, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(pos.a + 2, pos.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
}

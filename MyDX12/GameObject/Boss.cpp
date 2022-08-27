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
#include "BossAttack.h"

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
	delete carobj;
	delete object3d2;
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
	// クラスネーム取得
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;

	BossHP::GetInstance()->Initialize();
	// 大きさと座標の定数
	const Math::Vector3 scale = { 1.5f,1.5f,1.5f };
	const Math::Vector3 position = { 
		Common::ConvertTilePosition(element_stock.a) - 2.7f,
		1.0f, 
		Common::ConvertTilePosition(element_stock.b) 
	};

	// ---------- Object3Dの初期設定 ----------
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS));
	object3d->scale = scale;
	object3d->position = position;
	object3d2 = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS2));
	object3d2->scale = scale;
	object3d2->position = position;
	carobj = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSSCAR));
	carobj->scale = scale;
	carobj->position = position;

	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();
	//攻撃ゲージの秒数
	SetAttackTimer(300,CountType::FRAME);
	// 座標設定
	attackTimer->SetPosition(object3d->position + Math::Vector3(0.0f, 18.0f, 0.0f));
	// Attackエリア表示各種変数初期化
	BossAttack::GetInstance()->InitAttackDisplay();
}

void XIIlib::Boss::Update()
{
	// 現在のHPを取得
	_hit_point = BossHP::GetInstance()->GetBossHP();
	// デバッグにHP表示
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
	
	// BossのHPがデフォルトのHPの半分以下でなおかつBossが通常の状態で待機中なら
	if (_hit_point <= BossHP::GetInstance()->GetDefaultBossHP() / 2 && bossType == BossType::normal && bossState == BossState::wait)
	{
		bossType = BossType::strong;
		SetAttackTimer(180, CountType::FRAME);
		attackTimer->SetPosition(Math::Vector3(0.0f, 19.0f, 0.0f));
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

	// objectのUpdate
	object3d->Update();
	object3d2->Update();
	carobj->Update();
}

void XIIlib::Boss::Draw()
{
	/*if (switchingCount >= 1)
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
	}*/
	object3d->Draw();
	carobj->Draw();
}

void XIIlib::Boss::Action()
{
	if (attackTimer->SizeZeroFlag()) // アタックタイマーが0(以下)になったら通る
	{
		switching = true;
		// Attack表示の初期化
		BossAttack::GetInstance()->InitAttackDisplay();
		// 攻撃を開始するのでtrue
		determinateMoveAction = true;
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag()) // アタックタイマーが3分の1になったら通る(1回)
	{
		// king(player)の座標を取得
		int element = UnitManager::GetInstance()->GetUnitIDElements("King");
		if (element != -1)
		{
			std::shared_ptr<King> p_king =
				std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
			kingPos = p_king->GetElementStock();
		}

		// ランダムで攻撃タイプを選択
		bossAttackSelect = bossAttackMin + (int)(rand() * (bossAttackMax - bossAttackMin + 1) / (1 + RAND_MAX));
		// どのラインを攻撃するか
		BossAttack::GetInstance()->Target();

		// BossTypeが強い状態なら
		if (bossType == BossType::strong)
		{
			if (bossAttackSelect != 0)
			{
				BossAttack::GetInstance()->CreateMeteorPosition();
			}	
		}	
		// Attack表示の初期化
		BossAttack::GetInstance()->InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag()) // アタックタイマーが3分の1以下になったら通る(n回)
	{
		BossAttack::GetInstance()->DispTileDeathControl(bossAttackSelect);
		if (bossType == BossType::normal)
		{
			object3d->position.y += 0.2f;
			object3d2->position.y += 0.2f;
			if (object3d->position.y >= 20.0f)
			{
				object3d->position.y = 20.0f;
				object3d2->position.y = 20.0f;
			}
			//carobj->position.y +=0.1f;
			if (bossAttackSelect == 0)
			{
				// 縦
				BossAttack::GetInstance()->Vertical3LineDisplay();
			}
			else
			{
				// 横
				BossAttack::GetInstance()->Horizontal3LineDisplay();
			}
		}
		else if (bossType == BossType::strong)
		{
			if (bossAttackSelect == 0)
			{
				BossAttack::GetInstance()->OneMeteor3x3Display(kingPos);
			}
			else
			{
				// BossAttack::GetInstance()->RandomMeteor1x1Display();
			}
		}
		bossState = BossState::attack;
	}

	// 直接殴られた時の処理
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		BossAttack::GetInstance()->KnockBackAttack(element_stock);
	}	
}

void XIIlib::Boss::Attack()
{
	switch (bossType)
	{
	case BossType::normal:
		if (bossAttackSelect == 0)
		{
			// 縦攻撃
			BossAttack::GetInstance()->Vertical3LineAttack();
		}
		else
		{
			// 横攻撃
			BossAttack::GetInstance()->Horizontal3LineAttack();
		}
		break;
	case BossType::strong:
		if (bossAttackSelect == 0)
		{
			// 1点メテオ攻撃 3x3
			BossAttack::GetInstance()->OneMeteor3x3Attack(kingPos);
		}
		else
		{
			// (仮)ランダムメテオ攻撃 1x1
			BossAttack::GetInstance()->RandomMeteor1x1Attack();
		}
		break;
	default:
		// 攻撃なし
		break;
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
		// bossの状態を待機状態に戻す
		bossState = BossState::wait;
	}
}

void XIIlib::Boss::Move()
{}

bool XIIlib::Boss::AttackAreaExists()
{
	return false;
}

void XIIlib::Boss::AttackAreaDraw()
{}

void XIIlib::Boss::IniState()
{}

void XIIlib::Boss::CreateAttackArea()
{}

void XIIlib::Boss::ObjectUpdate()
{
	object3d->Update();
	object3d2->Update();
	carobj->Update();
}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	return false;
}

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
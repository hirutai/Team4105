#include "Boss.h"
#include "BossHP.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../GameObject/Stone.h"
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
	carobj->position = {0,50.0f,0};

	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();
	//攻撃ゲージの秒数
	SetAttackTimer(300, CountType::FRAME);
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

	// スイッチ
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
		// ボスタイプを強い状態に
		bossType = BossType::strong;
		// ボスの攻撃間隔を変更
		SetAttackTimer(180, CountType::FRAME);
		attackTimer->SetPosition(Math::Vector3(0.0f, 19.0f, 0.0f));

		// ボスの周りに壁を生成
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
		// Bossの行動
		Action();

		// タイマーの更新
		attackTimer->Timer();
	}
	if (determinateMoveAction) {
		// 攻撃処理
		Attack();
	}

	// 無敵判定制御
	if (object3d->position.y >= COLISION_HEIGHT && !invincibleFlag)
	{
		// 無敵状態付与
		invincibleFlag = true;
	}
	if (object3d->position.y < COLISION_HEIGHT && invincibleFlag)
	{
		// 無敵状態解除
		invincibleFlag = false;
	}

	// 直接殴られた時の処理
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		if (invincibleFlag)return;
		BossHP::GetInstance()->Damage();
		BossAttack::GetInstance()->KnockBackAttack(element_stock);
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
	// アタックタイマーによって行動を変化
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
		attackSelect = ATTACK_RANDOM_MIN_MAX.a + (int)(rand() * (ATTACK_RANDOM_MIN_MAX.b - ATTACK_RANDOM_MIN_MAX.a + 1) / (1 + RAND_MAX));
		
		// ボスの状態で準備の内容を変える(設定系)
		if (bossType == BossType::normal)// BossTypeが通常状態なら
		{
			// どのラインを攻撃するか
			BossAttack::GetInstance()->Target();
			// carobjを設定
			if (attackSelect == 0) // 縦ならば
			{
				// モデルをY軸に-90度(後ろ方向を向くように設定)
				carobj->rotation.y = -90.0f;
				// 座標の設定
				// Tile座標からワールド座標に変換
				float targetPos = Common::ConvertTilePosition(BossAttack::GetInstance()->GetTargetTile());
				carobj->position = {targetPos,1.0f,50.0f};
			}
			else if (attackSelect == 1) // 横ならば
			{
				// モデルをY軸に180度(右方向に向くように設定)
				carobj->rotation.y = 180.0f;
				// 座標の設定
				// Tile座標からワールド座標に変換
				float targetPos = Common::ConvertTilePosition(BossAttack::GetInstance()->GetTargetTile());
				carobj->position = { -50.0f,1.0f,targetPos };
			}
		}
		else if (bossType == BossType::strong)// BossTypeが強い状態なら
		{
			// ランダムメテオ攻撃の準備
			if (attackSelect != 0)
			{
				// メテオの座標決め
				BossAttack::GetInstance()->CreateMeteorPosition(attackSelect);
			}
		}
		// ボスを無敵にする
		// Attack表示の初期化
		BossAttack::GetInstance()->InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag()) // アタックタイマーが3分の1以下になったら通る(n回)
	{
		// ボスの状態で準備の内容を変える(動きながら準備)
		if (bossType == BossType::normal)
		{
			// タイルの表示非表示を制御
			BossAttack::GetInstance()->DispTileDeathControl(attackSelect,0);

			// ボスが浮きます
			object3d->position.y += 0.2f;
			if (object3d->position.y >= 2.0f)
			{
				// 無敵状態の付与
				//*invincibleFlag = true;
			}

			// 座標が天井(20.0f)についたら固定化
			if (object3d->position.y >= 20.0f)
			{
				object3d->position.y = 20.0f;
			}

			// アタックセレクトによって攻撃する予定うぃ表示
			if (attackSelect == 0)
			{
				// 縦3列表示
				BossAttack::GetInstance()->Vertical3Line("Display");
			}
			else
			{
				// 横3列表示
				BossAttack::GetInstance()->Horizontal3Line("Display");
			}
		}
		else if (bossType == BossType::strong)
		{
			// 1点メテオ
			if (attackSelect == 0)
			{
				BossAttack::GetInstance()->OneMeteor3x3("Display",kingPos);
			}

			// ランダムメテオは表示と攻撃が同時なので、ここには書きません
		}
		bossState = BossState::attack;
	}
}

void XIIlib::Boss::Attack()
{
	const int START_COUNT = 25; // スタートフレーム
	const float ACC = 0.7f; // 加速値
	// ボスタイプごとに攻撃処理を変化
	switch (bossType)
	{
	case BossType::normal:
		BossAttack::GetInstance()->DispTileDeathControl(attackSelect, START_COUNT);
		if (attackSelect == 0)
		{
			// 座標に加速値を入れる
			carobj->position.z -= ACC;
			// 縦3攻撃
			BossAttack::GetInstance()->Vertical3Line("Attack");
		}
		else
		{
			// 座標に加速値を入れる
			carobj->position.x += ACC;
			// 横3攻撃
			BossAttack::GetInstance()->Horizontal3Line("Attack");
		}
		break;
	case BossType::strong:
		if (attackSelect == 0)
		{
			// 1点メテオ攻撃 3x3
			BossAttack::GetInstance()->OneMeteor3x3("Attack",kingPos);
		}
		else
		{
			// ランダムメテオ攻撃 1x1
			BossAttack::GetInstance()->RandomMeteor1x1Attack();
		}
		break;
	default:
		// 攻撃なし
		break;
	}

	// ボスのポジションを戻す
	if (carobj->position.x >= 50.0f || carobj->position.z <= -50.0f)// 縦横車が背景からフェードアウトしきったら
	{
		object3d->position.y -= 0.2f;
		// 元の座標に戻ったら
		if (object3d->position.y <= 1.0f)
		{
			// 座標を設定
			object3d->position.y = 1.0f;
			// 無敵状態の解除
			//*invincibleFlag = false;
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
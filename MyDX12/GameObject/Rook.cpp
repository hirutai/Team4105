#include "Rook.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/Easing.h"
#include "ModelLoader.h"
#include "SmokeManager.h"

XIIlib::Rook::Rook()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 3;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Rook::~Rook()
{
	delete sManager;
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Rook> XIIlib::Rook::Create(int point_x, int point_z)
{
	std::shared_ptr<Rook> rook = std::make_shared<Rook>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Rook::Initialize()
{
	// 特になし
	_hit_point = 3;

	// クラスネーム取得
	const type_info& id = typeid(Rook);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_ROOK));
	object3d->scale = Math::Vector3({ 0.8f,0.8f,0.8f });
	object3d->position.y = 30.0f;
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();
	correctionAngle = 90.0f;

	SetAttackTimer(countingNum);

	intTimeCount = 3;
	sManager = SmokeManager::Create();
}

void XIIlib::Rook::Update()
{
	if (!fallFlag) {
		FallAction();
	}
	else if (fallFlag) {
		// 位置座標の更新
		object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
		if (!determinateMoveAction) {
			// 駒の行動
			Action();
			// タイマーの更新
			attackTimer->Timer();

			pos = object3d->position;
			// エフェクトの設定
			intTimeCount = 3;
			sManager->AllClear();
		}
		if (determinateMoveAction) {
			// モーション処理
			Motion();
			// アドで動かす
			intTimeCount--;
			if (intTimeCount < 0) {
				Math::Vector3 nowP = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
				Math::Vector3 nextP = { Common::ConvertTilePosition(nextPoint.a),1.0f, Common::ConvertTilePosition(nextPoint.b) };
				Math::Vector3 v = nowP - nextP;
				v.normalize();
				float rnd_scale = static_cast<float>(rand() % 10 + 5) / 100.0f;
				float rnd_mullValue = static_cast<float>(rand() % 5 + 5) / 50.0f;
				sManager->Add(0.05f, rnd_scale, v * rnd_mullValue + Math::Vector3(0, 0.05f, 0), object3d->position + Math::Vector3(0, 2, 0));
				intTimeCount = 3;
			}
			sManager->Update();
		}

		object3d->Update();
		// 座標設定
		attackTimer->SetPosition(object3d->position);
	}
}

void XIIlib::Rook::OriginBillDraw()
{
	sManager->Draw();
}

void XIIlib::Rook::Action()
{
	// 未来への栄光のロードを初期化
	nextPoint = { 0,0 };

	// 範囲に入ってるかのチェック
	if (AttackAreaExists())
	{
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 norm(0, 0);
		if (dif.a != 0) { norm.a = dif.a / abs(dif.a); }
		else { norm.a = dif.a; }
		if (dif.b != 0) { norm.b = dif.b / abs(dif.b); }
		else { norm.b = dif.b; }

		if (dif.a == 0)
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.b))return;
		}
		else
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.a))return;
		}

		if (isAttack == false)
		{
			isAttack = true;
			preElement_stock = kingPos;
		}
	}
	else
	{
		Move();
	}

	if (attackTimer->SizeZeroFlag())
	{
		notAttackflag = TRUE;
	}

	if (isAttack == true && notAttackflag == true)
	{
		point_attack = preElement_stock;
		//攻撃マスの色を変える
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
	}
	else
	{
		isAttack = false;
		//移動範囲の色付け
		AttackAreaDraw();
	}

	// ノックバック(共通処理)
	KnockBack();
}

void XIIlib::Rook::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		// 縦に重なっている
		if (dif.a == 0)
		{
			// 0より小さければKingより上にいる
			if (dif.b < 0)
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						
						IniState();

						return;
					}
				}
			}
			else // 0より大きければKingより下にいる
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						
						IniState();
						return;
					}
				}
			}

		}
		else // 横に重なっている
		{
			// 0より小さければKingより右
			if (dif.a < 0)
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}
			else// 0より大きければKingより左
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}
		}
		//if (AttackAreaExists()) { preElement_stock = kingPos; }
		// 攻撃
		nextPoint = preElement_stock;
		audio_->PlaySE("yankeeVoice.wav");
		IniState();
		
		// 移動量を取得
		Math::Point2 v = nextPoint - element_stock;
		//移動量から角度を求めて設定
		Direction(v);

		// 移動ますが決定されました。
		determinateMoveAction = true;
	}
}

void XIIlib::Rook::Move()
{
	//攻撃フラグ
	if (isAttack == true)return;
	//移動までのインターバル
	if (!attackTimer->SizeZeroFlag())return;
	//ルークの座標
	Math::Point2 temp = element_stock;
	nextPoint = element_stock;
	notAttackflag = TRUE;
	
	//3マス以下しか動けない時の移動用乱数
	int RookjMin = jMin;
	int RookjMax = jMax;
	tileRand = 1;
	//Switch文用の乱数
	int RookSwitchiMin = SwitchRandiMin;
	int RookSwitchiMax = SwitchRandiMax;
	SwitchRand = SwitchRandiMin + (int)(rand() * (SwitchRandiMax - SwitchRandiMin + 1) / (1 + RAND_MAX));

	switch (SwitchRand)
	{
	case 0:
		//左方向			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		if (ThreeCheckArea(temp) || AttackAreaManager::GetInstance()->CheckMoveAreas(temp))
		{
			nextPoint.a = 0;
		}
		else
		{
			nextPoint.a = temp.a;
		}
		audio_->PlaySE("pararira.wav");
		break;
	case 1:
		//右方向

		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;
		if (ThreeCheckArea(temp) || AttackAreaManager::GetInstance()->CheckMoveAreas(temp))
		{
			nextPoint.a = 7;
		}
		else
		{
			nextPoint.a = temp.a;
		}
		audio_->PlaySE("pararira.wav");
		break;
	case 2:
		//下方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.b -= tileRand;
		if (ThreeCheckArea(temp) || AttackAreaManager::GetInstance()->CheckMoveAreas(temp))
		{
			nextPoint.b = 0;
		}
		else
		{
			nextPoint.b = temp.b;
		}
		audio_->PlaySE("pararira.wav");
		break;
		
	case 3:
		//上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.b += tileRand;
		if (ThreeCheckArea(temp)|| AttackAreaManager::GetInstance()->CheckMoveAreas(temp))
		{
			nextPoint.b = 7;
		}
		else
		{
			nextPoint.b = temp.b;
		}
		audio_->PlaySE("pararira.wav");
		break;
	}

	// 移動量を取得
	Math::Point2 v = nextPoint - element_stock;
	//移動量から角度を求めて設定
	Direction(v);
	AttackAreaManager::GetInstance()->SetMoveAreas(nextPoint);
	// 移動ますが決定されました。
	determinateMoveAction = true;

	return;

	//ルークの座標ープレイヤーの座標
	//Math::Point2 dif = kingPos - element_stock;
	//// 差分がXのほうが大きい
	//if (abs(dif.a) > abs(dif.b))
	//{
	//	//// 自分とキングの間を1マスづつ調べる
	//	if (dif.b < 0)//0より小さければKingより上にいる
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		for (int i = 0; i < abs(dif.b); ++i)
	//		{
	//			temp.b--;
	//			if (ThreeCheckArea(temp))return;
	//			element_stock.b--;
	//		}
	//	}
	//	else // 0より大きければKingより下にいる
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		for (int i = 0; i < abs(dif.b); ++i)
	//		{
	//			temp.b++;
	//			if (ThreeCheckArea(temp))return;
	//			element_stock.b++;
	//		}
	//	}
	//}
	//else //差分がZの方が大きい (or どっちも一緒の時Z優先)
	//{
	//	//// 自分とキングの間を1マスづつ調べる
	//	if (dif.a < 0)//0より小さければKingより左にいる
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		for (int i = 0; i < abs(dif.a); ++i)
	//		{
	//			temp.a--;
	//			if (ThreeCheckArea(temp))return;
	//			element_stock.a--;
	//		}
	//	}
	//	else // 0より大きければKingより下にいる
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		for (int i = 0; i < abs(dif.a); ++i)
	//		{
	//			temp.a++;
	//			if (ThreeCheckArea(temp))return;
	//			element_stock.a++;
	//		}
	//	}
	//}
}

bool XIIlib::Rook::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	if (kingPos.a == element_stock.a || kingPos.b == element_stock.b)return true;
	return false;
}

void XIIlib::Rook::AttackAreaDraw()
{
	//左方向に駒があるか
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b)) || element_stock.a - i < 0)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b));
			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b), 3);
		}
	}
	//右方向に駒があるか
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + i, element_stock.b)) || element_stock.a + i > 7)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b), 3);
		}
	}
	//下方向に駒があるか
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b - i)) || element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b - i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a , element_stock.b - i), 3);
		}
	}
	//上方向に駒があるか
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b + i))||  element_stock.b + i > 7)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b + i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b + i), 3);
		}
	}

}

void XIIlib::Rook::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Rook::CreateAttackArea()
{

}

bool XIIlib::Rook::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

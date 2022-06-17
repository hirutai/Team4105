#include "Rook.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../GameObject/AttackTimer.h"

XIIlib::Rook::Rook()
{
	// モデルの初期化
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(0, 1, 1, 1);
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Rook::~Rook()
{
	delete attackTimer;
	delete collCapsule;
}

std::shared_ptr<XIIlib::Rook> XIIlib::Rook::Create(int point_x, int point_z)
{
	std::shared_ptr<Rook> rook = std::make_shared<Rook>();
	rook.get()->SetStartElement(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Rook::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(Rook);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("cars"));

	attackTimer = new AttackTimer(countingNum);
	attackTimer->Initialize();
}

void XIIlib::Rook::Update()
{
	// 駒の行動
	Action();
	// 位置座標の更新
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));
	std::cout << tileRand << std::endl;

	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	
	// 攻撃当たったら
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
		isAttack = false;

		// ノックバックの移動量
		const Math::Point2 backVector = UnitManager::GetInstance()->GetBackVector(element_stock);
		
		// ノックバック
		const int countLoop = 2;
		for (int i = 0; i < countLoop; i++) {
			Math::Point2 movePoint = backVector / 2.0f;
			if (Common::GetExceptionPoint(element_stock.a + movePoint.a) || Common::GetExceptionPoint(element_stock.b + movePoint.b)) {
				movePoint *= 0;
			}
			element_stock += movePoint;
		}
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 temp = kingPos;
		// ノックバック時に間に他の駒がいる場合、その間の駒の位置に止まる
		// 縦にノックバック
		if (dif.a == 0)
		{
			// 0より小さければKingより上にいる
			if (dif.b < 0)
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else // 0より大きければKingより下にいる
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
		}
		else if (dif.b == 0) // 横にノックバック
		{
			// 0より小さければKingより右
			if (dif.a < 0)
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else// 0より大きければKingより左
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
		}
		else	// 斜めにノックバック
		{
			// 左上にノックバック
			if (dif.a > 0 && dif.b < 0)
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a--;
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;

					}
				}
			}
			else if (dif.a < 0 && dif.b < 0)	// 右上にノックバック
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
						break;
					}
				}
			}
			else if (dif.a > 0 && dif.b > 0)	// 左下にノックバック
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a--;
					temp.b--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
						break;
					}
				}
			}
			else if (dif.a < 0 && dif.b > 0)	// 右下にノックバック
			{
				// 自分とキングの間を1マスづつ調べる
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					temp.b--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
						break;
					}
				}
			}
		}
	}

	attackTimer->Timer();
	attackTimer->SetPosition(object3d->position);

	collCapsule->Update();
	object3d->Update();
}

void XIIlib::Rook::Draw()
{
	//collCapsule->Draw();
	object3d->Draw();
}

void XIIlib::Rook::SetStartElement(int x, int z)
{
	startElement_stock = Math::Point2(x, z);
	element_stock = Math::Point2(x, z);
}

void XIIlib::Rook::Action()
{
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
	
}

void XIIlib::Rook::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackInterval <= 150 && attackInterval >= 120 || attackInterval <= 90 && attackInterval >= 75 || attackInterval <= 60 && attackInterval >= 45 || attackInterval <= 30 && attackInterval >= 15 )
	{
		collCapsule->SetColor(1, 0, 0, 1);
	}
	else 
	{
		collCapsule->SetColor(0, 1, 1, 1);
	}
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
		element_stock = preElement_stock;

		IniState();
		//notAttackflag = false;
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
	notAttackflag = TRUE;

	collCapsule->SetColor(0, 1, 1, 1);
	
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

		if (ThreeCheckArea(temp))
		{
			element_stock.a = 0;
		}
		else
		{
			element_stock.a = temp.a;
		}
			break;
	case 1:
		//右方向

		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;

		if (ThreeCheckArea(temp))
		{
			element_stock.a = 7;
		}
		else
		{
			element_stock.a = temp.a;
		}
		break;
	case 2:
		//下方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.b -= tileRand;

		if (ThreeCheckArea(temp))
		{
			element_stock.b = 0;
		}
		else
		{
			element_stock.b = temp.b;
		}
		break;
	case 3:
		//上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.b += tileRand;

		if (ThreeCheckArea(temp))
		{
			element_stock.b = 7;
		}
		else
		{
			element_stock.b = temp.b;
		}
		break;
	}
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
	collCapsule->SetColor(0, 1, 1, 1);
}

void XIIlib::Rook::Hit(int attackPoint)
{
	//int _damage = _defense_point - attackPoint;
	//if (_damage < 0) {
	//	_hit_point += _damage;
	//	is_hit = 1;
	//}

	_hit_point -= attackPoint;
}

void XIIlib::Rook::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::Rook::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Rook::CreateAttackArea()
{

}

void XIIlib::Rook::BillObjectDraw()
{
	attackTimer->Draw(); // 攻撃タイマーの描画
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

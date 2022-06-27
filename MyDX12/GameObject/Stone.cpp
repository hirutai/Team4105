#include "Stone.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"

XIIlib::Stone::Stone()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Stone::~Stone()
{
	delete object3d;
}

std::shared_ptr<XIIlib::Stone> XIIlib::Stone::Create(int point_x, int point_z)
{
	std::shared_ptr<Stone> rook = std::make_shared<Stone>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Stone::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(Stone);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});

}

void XIIlib::Stone::Update()
{

	// 駒の行動
	Action();
	// 位置座標の更新
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// 攻撃当たったら
//	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
	//	isAttack = false;
		// ノックバック
	//	element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

	//	Math::Point2 dif = kingPos - element_stock;
	//	Math::Point2 temp = kingPos;
	//	// ノックバック時に間に他の駒がいる場合、その間の駒の位置に止まる
	//	// 縦にノックバック
	//	if (dif.a == 0)
	//	{
	//		// 0より小さければKingより上にいる
	//		if (dif.b < 0)
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.b) - 1; ++i)
	//			{
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//		else // 0より大きければKingより下にいる
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.b) - 1; ++i)
	//			{
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//	}
	//	else if (dif.b == 0) // 横にノックバック
	//	{
	//		// 0より小さければKingより右
	//		if (dif.a < 0)
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//		else// 0より大きければKingより左
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//	}
	//	else	// 斜めにノックバック
	//	{
	//		// 左上にノックバック
	//		if (dif.a > 0 && dif.b < 0)
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;

	//				}
	//			}
	//		}
	//		else if (dif.a < 0 && dif.b < 0)	// 右上にノックバック
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//		else if (dif.a > 0 && dif.b > 0)	// 左下にノックバック
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//		else if (dif.a < 0 && dif.b > 0)	// 右下にノックバック
	//		{
	//			// 自分とキングの間を1マスづつ調べる
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//	}


	//	if (Common::GetExceptionPoint(element_stock.a) || Common::GetExceptionPoint(element_stock.b)) {
	//		Hit(3);
	//	}

	//}

	object3d->Update();
}

void XIIlib::Stone::Action()
{
	//// 範囲に入ってるかのチェック
	//if (AttackAreaExists())
	//{
	//	Math::Point2 dif = kingPos - element_stock;
	//	Math::Point2 norm(0, 0);
	//	if (dif.a != 0) { norm.a = dif.a / abs(dif.a); }
	//	else { norm.a = dif.a; }
	//	if (dif.b != 0) { norm.b = dif.b / abs(dif.b); }
	//	else { norm.b = dif.b; }

	//	if (dif.a == 0)
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		if (MoveAreaCheck(element_stock, norm, dif.b))return;
	//	}
	//	else
	//	{
	//		// kingのポイション分を引いてfor文で調べる
	//		if (MoveAreaCheck(element_stock, norm, dif.a))return;
	//	}

	//	if (isAttack == false)
	//	{
	//		isAttack = true;
	//		preElement_stock = kingPos;
	//	}
	//}
	//else
	//{
	//	Move();
	//}

	//if (UnitManager::GetInstance()->GetIntervalTimer() == 420)
	//{
	//	notAttackflag = TRUE;
	//}

	//if (isAttack == true && notAttackflag == true)
	//{
	//	point_attack = preElement_stock;
	//	//攻撃マスの色を変える
	//	UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
	//	Attack();
	//}
	//else
	//{
	//	isAttack = false;
	//	//移動範囲の色付け
	//	AttackAreaDraw();
	//}

}

void XIIlib::Stone::Attack()
{
	//// カウントを減らす
	//attackInterval--;
	////色を変える
	//if (attackInterval <= 150 && attackInterval >= 120 || attackInterval <= 90 && attackInterval >= 75 || attackInterval <= 60 && attackInterval >= 45 || attackInterval <= 30 && attackInterval >= 15)
	//{
	//	collCapsule->SetColor(1, 0, 0, 1);
	//}
	//else
	//{
	//	collCapsule->SetColor(0, 1, 1, 1);
	//}
	//if (attackInterval == 0)
	//{
	//	Math::Point2 dif = kingPos - preElement_stock;
	//	Math::Point2 temp = element_stock;

	//	//// 自分とキングの間を1マスづつ調べる
	//	if (dif.b <= 0 && dif.a == element_stock.a)//キングより下にいるかを調べる
	//	{
	//		//上に１進む
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.b > 0 && dif.a == element_stock.a)//キングより上にいるかを調べる
	//	{
	//		//下に1進む
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.b <= 0 && dif.a <= 0)//キングより左下にいるかを調べる
	//	{
	//		//右上に1進む
	//		temp.a -= 1;
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.b > 0 && dif.a <= 0)//キングより左上にいるかを調べる
	//	{
	//		//右下に1進む
	//		temp.a -= 1;
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.b <= 0 && dif.a > 0)//キングより右下にいるかを調べる
	//	{
	//		//左上に1進む
	//		temp.a += 1;
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.b > 0 && dif.a >= 0)//キングより右上にいるかを調べる
	//	{
	//		//左下に1進む
	//		temp.a += 1;
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// 自分とキングの間を1マスづつ調べる
	//	else if (dif.a <= 0 && dif.b == element_stock.b)//キングより左にいるかを調べる
	//	{
	//		//右に１進む
	//		temp.a -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	else if (dif.a > 0 && dif.b == element_stock.b)// 0より大きければKingより右にいる
	//	{
	//		//左に１進む
	//		temp.a += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	// 攻撃
	//	element_stock = preElement_stock;

	//	IniState();
	//	notAttackflag = false;
	//}
}

void XIIlib::Stone::Move()
{
	////攻撃フラグ
	//if (isAttack == true)return;
	////移動までのインターバル
	//if (UnitManager::GetInstance()->GetIntervalTimer() < 420)return;
	////ヤンキーの座標
	//Math::Point2 temp = element_stock;
	//notAttackflag = TRUE;

	//collCapsule->SetColor(0, 1, 1, 1);

	////プレイヤーの座標 ー ヤンキーの座標
	//Math::Point2 dif = kingPos - element_stock;

	//// 自分とキングの間を1マスづつ調べる
	//if (dif.b < 0 && dif.a == 0)//キングより下にいるかを調べる
	//{
	//	//上に１進む
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.b -= 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.b > 0 && dif.a == 0)//キングより上にいるかを調べる
	//{
	//	//下に1進む
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.b += 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.a < 0 && dif.b == 0)//キングより左にいるかを調べる
	//{
	//	//右に１進む
	//	temp.a -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//}
	//else if (dif.a > 0 && dif.b == 0)// 0より大きければKingより右にいる
	//{
	//	//左に１進む
	//	temp.a += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.b <= 0 && dif.a <= 0)//キングより左下にいるかを調べる
	//{
	//	//右上に1進む
	//	temp.a -= 1;
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//	element_stock.b -= 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.b > 0 && dif.a <= 0)//キングより左上にいるかを調べる
	//{
	//	//右下に1進む
	//	temp.a -= 1;
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//	element_stock.b += 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.b <= 0 && dif.a > 0)//キングより右下にいるかを調べる
	//{
	//	//左上に1進む
	//	temp.a += 1;
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//	element_stock.b -= 1;
	//}
	//// 自分とキングの間を1マスづつ調べる
	//else if (dif.b > 0 && dif.a >= 0)//キングより右上にいるかを調べる
	//{
	//	//左下に1進む
	//	temp.a += 1;
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//	element_stock.b += 1;
	//}
}

bool XIIlib::Stone::AttackAreaExists()
{
	//int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	//if (element != -1)
	//{
	//	std::shared_ptr<King> p_king =
	//		std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
	//	kingPos = p_king->GetElementStock();
	//}
	//// 範囲に入ってるかのチェック
	//if (kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b ||     //右
	//	kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b + 1 || //右上
	//	kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b - 1 || //右下
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b ||     //左
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b - 1 || //左下
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b + 1 || //左上
	//	kingPos.a == element_stock.a && kingPos.b == element_stock.b + 1 ||     //上
	//	kingPos.a == element_stock.a && kingPos.b == element_stock.b - 1        //下
	//	)return true;
	return false;
}

void XIIlib::Stone::AttackAreaDraw()
{
	
}

void XIIlib::Stone::IniState()
{
	//isAttack = false;
	//attackInterval = 180;
	//collCapsule->SetColor(0, 1, 1, 1);
}

void XIIlib::Stone::CreateAttackArea()
{

}

bool XIIlib::Stone::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	//Math::Point2 pos = crPos;
	//for (int i = 0; i < abs(tileNum) - 1; ++i)
	//{
	//	pos += vec;
	//	if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	//}
	return false;
}

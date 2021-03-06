#include "Knight.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Knight::Knight()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Knight::~Knight()
{
	delete object3d;
}

std::shared_ptr<XIIlib::Knight> XIIlib::Knight::Create(int point_x, int point_z)
{
	std::shared_ptr<Knight> knight = std::make_shared<Knight>();
	knight.get()->SetElementStock(point_x, point_z);
	knight.get()->Initialize();

	return std::move(knight);
}

void XIIlib::Knight::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(Knight);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
}

void XIIlib::Knight::Update()
{
	// 駒の行動
	Action();
	// 位置座標の更新


	// 攻撃当たったら
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
		isAttack = false;
		// ノックバック
		element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

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


		if (Common::GetExceptionPoint(element_stock.a) || Common::GetExceptionPoint(element_stock.b)) {
			SetHitDamage(3);
		}

	}

}

void XIIlib::Knight::Action()
{

	//if (UnitManager::GetInstance()->GetIntervalTimer() > 0)return;

	// 範囲に入ってるかのチェック
	if (AttackAreaExists())
	{
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

	/*if (UnitManager::GetInstance()->GetIntervalTimer() == 420)
	{
		notAttackflag = 1;
	}*/

	if (isAttack == true && notAttackflag == 1)
	{
		point_attack = preElement_stock;
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
	}
	else
	{
		isAttack = false;
		AttackAreaDraw();
	}
}

void XIIlib::Knight::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackInterval == 0)
	{
		//if (AttackAreaExists()) { preElement_stock = kingPos; }
		// 攻撃
		element_stock = preElement_stock;
		IniState();
		notAttackflag = 0;
	}
}

void XIIlib::Knight::Move()
{
	if (isAttack == true)return;
	//if (UnitManager::GetInstance()->GetIntervalTimer() < 420)return;
	notAttackflag = true;

	Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp(0, 0);


	if (UnitManager::GetInstance()->AllOnUnit(temp));
	// キング上ナイト下
	if (dif.b >= 0)
	{
		// キング右ナイト左
		if (dif.a >= 0)
		{
			// 上右方向
			temp = element_stock + Math::Point2(1, 2);
			if (ThreeCheckArea(temp))
			{
				// 右上方向
				temp = element_stock + Math::Point2(2, 1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
		else // キング左ナイト右
		{
			// 上左方向
			temp = element_stock + Math::Point2(-1, 2);
			if (ThreeCheckArea(temp))
			{
				// 左上方向
				temp = element_stock + Math::Point2(-2, 1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
	}
	else if (dif.b < 0) // キング下ナイト上
	{
		// キング右ナイト左
		if (dif.a >= 0)
		{
			// 下右方向
			temp = element_stock + Math::Point2(1, -2);
			if (ThreeCheckArea(temp))
			{
				// 右下方向
				temp = element_stock + Math::Point2(2, -1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
		else // キング左ナイト右
		{
			// 下左方向
			temp = element_stock + Math::Point2(-1, -2);
			if (ThreeCheckArea(temp))
			{
				// 左上方向
				temp = element_stock + Math::Point2(-2, -1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
	}
}

bool XIIlib::Knight::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	Math::Point2 temp(0, 0);
	// 上方向
	temp = element_stock + Math::Point2(1, 2);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	temp = element_stock + Math::Point2(-1, 2);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	// 右方向
	temp = element_stock + Math::Point2(2, 1);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	temp = element_stock + Math::Point2(2, -1);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	// 左方向
	temp = element_stock + Math::Point2(-2, 1);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	temp = element_stock + Math::Point2(-2, -1);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	// 下方向
	temp = element_stock + Math::Point2(1, -2);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;
	temp = element_stock + Math::Point2(-1, -2);
	if (kingPos.a == temp.a && kingPos.b == temp.b)return true;

	return false;
}

void XIIlib::Knight::AttackAreaDraw()
{
	// 範囲に入ってるかのチェック
	Math::Point2 temp(0, 0);
	// 上方向
	temp = element_stock + Math::Point2(1, 2);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	temp = element_stock + Math::Point2(-1, 2);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	// 右方向
	temp = element_stock + Math::Point2(2, 1);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	temp = element_stock + Math::Point2(2, -1);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	// 左方向
	temp = element_stock + Math::Point2(-2, 1);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	temp = element_stock + Math::Point2(-2, -1);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	// 下方向
	temp = element_stock + Math::Point2(1, -2);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
	temp = element_stock + Math::Point2(-1, -2);
	if (temp.a <= 7 && temp.a >= 0 && temp.b <= 7 && temp.b >= 0)
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(temp));

		//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(temp), 3);
	}
}

void XIIlib::Knight::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Knight::CreateAttackArea()
{

}

bool XIIlib::Knight::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

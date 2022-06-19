#include "Unit.h"
#include "UnitManager.h"
#include "../GameObject/Common.h"
#include <iostream>
#include "../3D/Object3D.h"

bool XIIlib::Unit::ThreeCheckArea(Math::Point2 element_stock)
{
	if (UnitManager::GetInstance()->AllOnUnit(element_stock)
		|| Common::GetExceptionPoint(element_stock.a)
		|| Common::GetExceptionPoint(element_stock.b)) return true;

	return false;

}

void XIIlib::Unit::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Unit::BillObjectDraw()
{
}

void XIIlib::Unit::ZeroHp()
{
	if (_hit_point <= 0)
	{
		OnDead();
		UnitManager::GetInstance()->AddCountDead();
	}
}

void XIIlib::Unit::Draw()
{
	// オブジェクトの描画
	object3d->Draw();
}

void XIIlib::Unit::KnockBack()
{
	// 攻撃当たっていなければそく返す
	if (!UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))return;

	SetHitDamage(1);
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

void XIIlib::Unit::SetElementStock(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::Unit::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
}

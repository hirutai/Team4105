#include "Unit.h"
#include "UnitManager.h"
#include "../GameObject/Common.h"
#include <iostream>

bool XIIlib::Unit::ThreeCheckArea(Math::Point2 element_stock)
{
	if (UnitManager::GetInstance()->AllOnUnit(element_stock)
		|| Common::GetExceptionPoint(element_stock.a)
		|| Common::GetExceptionPoint(element_stock.b)) return true;

	return false;

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

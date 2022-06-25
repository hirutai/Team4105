#include "Unit.h"
#include "UnitManager.h"
#include "../GameObject/Common.h"
#include <iostream>
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"

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
	OriginBillDraw();

	if (attackTimer == nullptr) return;
	// 攻撃タイマーの描画
	attackTimer->Draw();
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

void XIIlib::Unit::OriginBillDraw()
{
	// 共通ではない時用の関数
}

void XIIlib::Unit::KnockBack()
{
	// 攻撃当たっていなければそく返す
	if (!UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))return;

	SetHitDamage(1);
	audio_->PlaySE("damage.wav",0.3f);
	isAttack = false;
	// ノックバックの移動量
	const Math::Point2 backVector = UnitManager::GetInstance()->GetBackVector(element_stock);

	// ノックバック
	const int countLoop = 2;
	Math::Point2 myTemp = element_stock;

	for (int i = 0; i < countLoop; i++) {
		Math::Point2 movePoint = backVector / 2.0f;

		// 移動先に駒があるのかどうか?
		if(UnitManager::GetInstance()->AllOnUnit(myTemp + movePoint)) {
			myTemp += movePoint;
			break;
		}
		// 例外かどうか
		if (Common::GetExceptionPoint(myTemp.a + movePoint.a) || Common::GetExceptionPoint(myTemp.b + movePoint.b)) {
			movePoint *= 0;// 一マス分の移動量を0にする
		}

		myTemp += movePoint;
	}
	nextPoint = myTemp;

	// 移動ますが決定されました。
	determinateMoveAction = true;
}

void XIIlib::Unit::SetElementStock(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::Unit::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
}

void XIIlib::Unit::SetAttackTimer(int countNum)
{
	attackTimer = new AttackTimer(countNum);
	attackTimer->Initialize();
}

void XIIlib::Unit::ObjectUpdate()
{
	object3d->Update();
}
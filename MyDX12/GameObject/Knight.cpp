#include "Knight.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Knight::Knight()
{
	// モデルの初期化
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(1, 1, 0, 1);
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Knight::~Knight()
{
	delete collCapsule;
}

std::shared_ptr<XIIlib::Knight> XIIlib::Knight::Create(int point_x, int point_z)
{
	std::shared_ptr<Knight> rook = std::make_shared<Knight>();
	rook.get()->SetStartElement(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
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
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));

	// 攻撃当たったら
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		Hit(1);
		// ノックバック
		element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

		if (Common::GetExceptionPoint(element_stock.a) || Common::GetExceptionPoint(element_stock.b)) {
			Hit(3);
		}

		element_stock = Common::OffsetTilePosition2(element_stock);
	}

	collCapsule->Update();
}

void XIIlib::Knight::Draw()
{
	collCapsule->Draw();
}

void XIIlib::Knight::SetStartElement(int x, int z)
{
	element_stock = Math::Point2(x, z);
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

	if (isAttack == true && notAttackflag == TRUE)
	{
		point_attack = preElement_stock;
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
	}
	else
	{
		isAttack = false;
	}
}

void XIIlib::Knight::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	collCapsule->SetColor(1, 0, 0, 1);
	if (attackInterval == 0)
	{
		if (AttackAreaExists()) { preElement_stock = kingPos; }
		// 攻撃
		element_stock = preElement_stock;
		IniState();
		notAttackflag = FALSE;
	}
}

void XIIlib::Knight::Move()
{
	if (isAttack == true)return;
	if (UnitManager::GetInstance()->GetIntervalTimer() > 0)return;
	notAttackflag = true;

	collCapsule->SetColor(1, 1, 0, 1);
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
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				// 右上方向
				temp = element_stock + Math::Point2(2, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
			}
			element_stock = temp;
		}
		else // キング左ナイト右
		{
			// 上左方向
			temp = element_stock + Math::Point2(-1, 2);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				// 左上方向
				temp = element_stock + Math::Point2(-2, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
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
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				// 右下方向
				temp = element_stock + Math::Point2(2, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
			}
			element_stock = temp;
		}
		else // キング左ナイト右
		{
			// 下左方向
			temp = element_stock + Math::Point2(-1, -2);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				// 左上方向
				temp = element_stock + Math::Point2(-2, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
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

void XIIlib::Knight::IniState()
{
	isAttack = false;
	attackInterval = 60;
	collCapsule->SetColor(1, 1, 0, 1);
}

void XIIlib::Knight::Hit(int attackPoint)
{
	//int _damage = _defense_point - attackPoint;
	//if (_damage < 0) {
	//	_hit_point += _damage;
	//	is_hit = 1;
	//}

	_hit_point -= attackPoint;
}

void XIIlib::Knight::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::Knight::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Knight::CreateAttackArea()
{

}
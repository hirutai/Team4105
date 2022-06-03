#include "Bishop.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Bishop::Bishop()
{
	// モデルの初期化
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(0, 1, 0, 1);
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Bishop::~Bishop()
{
	delete collCapsule;
}

std::shared_ptr<XIIlib::Bishop> XIIlib::Bishop::Create(int point_x, int point_z)
{
	std::shared_ptr<Bishop> rook = std::make_shared<Bishop>();
	rook.get()->SetStartElement(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Bishop::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(Bishop);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
}

void XIIlib::Bishop::Update()
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

void XIIlib::Bishop::Draw()
{
	collCapsule->Draw();
}

void XIIlib::Bishop::SetStartElement(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::Bishop::Action()
{
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

	if (isAttack == TRUE)
	{
		point_attack = preElement_stock;
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
		flag = FALSE;
	}
	else
	{
		//移動範囲の色付け
		AttackAreaDraw();
	}
}

void XIIlib::Bishop::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える  
	collCapsule->SetColor(1, 0, 0, 1);
	if (attackInterval == 0)
	{
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 temp = element_stock;

		Math::Point2 temp_norm;
		if (dif.a == 0) { temp_norm.a = dif.a; }
		else { temp_norm.a = dif.a / abs(dif.a); }
		if (dif.b == 0) { temp_norm.b = dif.b; }
		else { temp_norm.b = dif.b / abs(dif.b); }
		// 自分とキングの間を1マスづつ調べる
		for (int i = 0; i < abs(dif.a) - 1; ++i)
		{
			temp += temp_norm;
			if (UnitManager::GetInstance()->AllOnUnit(temp))
			{
				IniState();
				return;
			}
		}
		if (AttackAreaExists()) { preElement_stock = kingPos; }
		// 攻撃
		element_stock = preElement_stock;
		IniState();
	}
}

void XIIlib::Bishop::Move()
{
	//なんか追加

	if (flag == TRUE)
	{
		if (isAttack == true)return;
	}

	if (UnitManager::GetInstance()->GetIntervalTimer() > 0)
	{
		return;
	}

	collCapsule->SetColor(0, 1, 0, 1);
	Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp(0, 0);
	// もうすでに範囲上にいたら動かない
	if (abs(dif.a) == abs(dif.b))return;
	// キングが自分より上にいる
	if (dif.b > 0)
	{
		// キングが右にいる
		if (dif.a > 0)
		{
			temp = element_stock + Math::Point2(1, 1);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				temp = element_stock + Math::Point2(-1, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))return;
			}
			element_stock = temp;
		}
		else // キングが左か真ん中にいる
		{
			temp = element_stock + Math::Point2(-1, 1);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				temp = element_stock + Math::Point2(1, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))return;
			}
			element_stock = temp;
		}

	}
	else if (dif.b < 0) // キングが自分より下にいる
	{
		if (dif.a > 0)
		{
			temp = element_stock + Math::Point2(1, -1);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				temp = element_stock + Math::Point2(-1, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))return;
			}
			element_stock = temp;
		}
		else
		{
			temp = element_stock + Math::Point2(-1, -1);
			if (UnitManager::GetInstance()->AllOnUnit(temp)
				|| Common::GetExceptionPoint(temp.a)
				|| Common::GetExceptionPoint(temp.b))
			{
				temp = element_stock + Math::Point2(1, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))return;
			}
			element_stock = temp;
		}
	}
	else // 真ん中だったら
	{
		if (dif.a > 0)
		{
			if (element_stock.b >= 4)
			{
				temp = element_stock + Math::Point2(1, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))
				{
					temp = element_stock + Math::Point2(1, 1);
					if (UnitManager::GetInstance()->AllOnUnit(temp)
						|| Common::GetExceptionPoint(temp.a)
						|| Common::GetExceptionPoint(temp.b))return;
				}
			}
			else
			{
				temp = element_stock + Math::Point2(1, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))
				{
					temp = element_stock + Math::Point2(1, -1);
					if (UnitManager::GetInstance()->AllOnUnit(temp)
						|| Common::GetExceptionPoint(temp.a)
						|| Common::GetExceptionPoint(temp.b))return;
				}
			}
		}
		else
		{
			if (element_stock.b >= 4)
			{
				temp = element_stock + Math::Point2(-1, -1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))
				{
					temp = element_stock + Math::Point2(-1, 1);
					if (UnitManager::GetInstance()->AllOnUnit(temp)
						|| Common::GetExceptionPoint(temp.a)
						|| Common::GetExceptionPoint(temp.b))return;
				}
			}
			else
			{
				temp = element_stock + Math::Point2(-1, 1);
				if (UnitManager::GetInstance()->AllOnUnit(temp)
					|| Common::GetExceptionPoint(temp.a)
					|| Common::GetExceptionPoint(temp.b))
				{
					temp = element_stock + Math::Point2(-1, -1);
					if (UnitManager::GetInstance()->AllOnUnit(temp)
						|| Common::GetExceptionPoint(temp.a)
						|| Common::GetExceptionPoint(temp.b))return;
				}
			}
		}
		element_stock = temp;
	}
	flag = TRUE;
}

bool XIIlib::Bishop::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	Math::Point2 dif = kingPos - element_stock;
	// xとzの絶対値が一緒だったら攻撃範囲にいる範囲
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Bishop::AttackAreaDraw()
{
	//左下
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b-i)) || element_stock.a - i < 0|| element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b - i), 3);
		}
	}
	//右上
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + i, element_stock.b + i)) || element_stock.a + i > 7 || element_stock.b + i > 7)
		{
			break;
		}
		else
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b + i), 3);
		}
	}
	//右下
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + i, element_stock.b - i)) || element_stock.a + i > 7 || element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b - i), 3);
		}
	}
	//左上
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b + i)) || element_stock.a - i < 0 || element_stock.b + i > 7)
		{
			break;
		}
		else
		{
			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b + i), 3);
		}
	}
}

void XIIlib::Bishop::IniState()
{
	isAttack = false;
	attackInterval = 60;
	collCapsule->SetColor(0, 1, 0, 1);
}

void XIIlib::Bishop::Hit(int attackPoint)
{
	int _damage = _defense_point - attackPoint;
	if (_damage < 0) {
		_hit_point += _damage;
		is_hit = 1;
	}
}

void XIIlib::Bishop::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::Bishop::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Bishop::CreateAttackArea()
{

}
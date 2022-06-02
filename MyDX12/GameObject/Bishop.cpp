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
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 norm(0, 0);
		if (dif.a != 0) { norm.a = dif.a / abs(dif.a); }
		else { norm.a = dif.a; }
		if (dif.b != 0) { norm.b = dif.b / abs(dif.b); }
		else { norm.b = dif.b; }

		if (dif.a == dif.b)
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.b))return;
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
	if (UnitManager::GetInstance()->GetIntervalTimer()==0)
	{
		notAttackflag = TRUE;
	}

	if (isAttack == TRUE && notAttackflag == TRUE)
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

void XIIlib::Bishop::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	collCapsule->SetColor(1, 0, 0, 1);
	if (attackInterval == 0)
	{
		Math::Point2 dif = kingPos - preElement_stock;
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
		//if (AttackAreaExists()) { preElement_stock = kingPos; }
		// 攻撃
		element_stock = preElement_stock;
		IniState();
		notAttackflag = FALSE;
	}
}

void XIIlib::Bishop::Move()
{

	if (isAttack == true)return;
	if (UnitManager::GetInstance()->GetIntervalTimer() > 0)return;

	notAttackflag = TRUE;

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
			if (ThreeCheckArea(temp))
			{
				temp = element_stock + Math::Point2(-1, 1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
		else // キングが左か真ん中にいる
		{
			temp = element_stock + Math::Point2(-1, 1);
			if (ThreeCheckArea(temp))
			{
				temp = element_stock + Math::Point2(1, 1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}

	}
	else if (dif.b < 0) // キングが自分より下にいる
	{
		if (dif.a > 0)
		{
			temp = element_stock + Math::Point2(1, -1);
			if (ThreeCheckArea(temp))
			{
				temp = element_stock + Math::Point2(-1, -1);
				if (ThreeCheckArea(temp))return;
			}
			element_stock = temp;
		}
		else
		{
			temp = element_stock + Math::Point2(-1, -1);
			if (ThreeCheckArea(temp))
			{
				temp = element_stock + Math::Point2(1, -1);
				if (ThreeCheckArea(temp))return;
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
				if (ThreeCheckArea(temp))
				{
					temp = element_stock + Math::Point2(1, 1);
					if (ThreeCheckArea(temp))return;
				}
			}
			else
			{
				temp = element_stock + Math::Point2(1, 1);
				if (ThreeCheckArea(temp))
				{
					temp = element_stock + Math::Point2(1, -1);
					if (ThreeCheckArea(temp))return;
				}
			}
		}
		else
		{
			if (element_stock.b >= 4)
			{
				temp = element_stock + Math::Point2(-1, -1);
				if (ThreeCheckArea(temp))
				{
					temp = element_stock + Math::Point2(-1, 1);
					if (ThreeCheckArea(temp))return;
				}
			}
			else
			{
				temp = element_stock + Math::Point2(-1, 1);
				if (ThreeCheckArea(temp))
				{
					temp = element_stock + Math::Point2(-1, -1);
					if (ThreeCheckArea(temp))return;
				}
			}
		}
		element_stock = temp;
	}
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

bool XIIlib::Bishop::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

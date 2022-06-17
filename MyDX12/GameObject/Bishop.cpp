#include "Bishop.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../GameObject/AttackTimer.h"

XIIlib::Bishop::Bishop()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Bishop::~Bishop()
{
	delete attackTimer;
	delete object3d;
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
	object3d = Object3D::Create(Model::CreateFromOBJ("bike"));

	attackTimer = new AttackTimer(countingNum);
	attackTimer->Initialize();
}

void XIIlib::Bishop::Update()
{
	// 駒の行動
	Action();
	// 位置座標の更新
	std::cout << tileRand << "ビショップ" << std::endl;

	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// 攻撃当たったら
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		Hit(1);
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

	object3d->Update();
}

void XIIlib::Bishop::Draw()
{
	//collCapsule->Draw();
	object3d->Draw();
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
	if (attackTimer->SizeZeroFlag())
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
		AttackAreaDraw();
		isAttack = false;
	}
}

void XIIlib::Bishop::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
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
		//notAttackflag = FALSE;
	}
}

void XIIlib::Bishop::Move()
{

	if (isAttack == true)return;
	if (!attackTimer->SizeZeroFlag())return;

	//notAttackflag = TRUE;

	//Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp = element_stock;
	// もうすでに範囲上にいたら動かない
	//if (abs(dif.a) == abs(dif.b))return;
	//3マス以下しか動けない時の移動用乱数
	int BishopjMin = jMin;
	int BishopjMax = jMax;
	tileRand = 1;
	//Switch文用の乱数
	int BishopSwitchiMin = SwitchRandiMin;
	int BishopSwitchiMax = SwitchRandiMax;
	SwitchRand = SwitchRandiMin + (int)(rand() * (SwitchRandiMax - SwitchRandiMin + 1) / (1 + RAND_MAX));

	switch (SwitchRand)
	{
	case 0:
		//左下方向			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		temp.b -= tileRand;

		if (temp.a <= -1 && temp.b <= -1)
		{
			element_stock.a = 0;
			element_stock.b = 0;
		}
		else if(temp.a <= -1)
		{
			element_stock.b = temp.b - temp.a;
			element_stock.a = 0;
		}
		else if (temp.b <= -1)
		{
			element_stock.a = temp.a - temp.b;
			element_stock.b = 0;
		}
		else
		{
			element_stock = temp;
		}
		break;
	case 1:
		//右下方向			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;
		temp.b -= tileRand;

		if (temp.a >= 8 && temp.b <= -1)
		{
			element_stock.a = 8;
			element_stock.b = 0;
		}
		else if (temp.a >= 8)
		{
			element_stock.b = temp.b + (temp.a - 7);
			element_stock.a = 7;
		}
		else if (temp.b <= -1)
		{
			element_stock.a = temp.a + temp.b;
			element_stock.b = 0;
		}
		else
		{
			element_stock = temp;
		}
		break;
	case 2:
		//左上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		temp.b += tileRand;

		if (temp.a <= -1 && temp.b >= 8)
		{
			element_stock.a = 0;
			element_stock.b = 7;
		}
		else if (temp.a <= -1)
		{
			element_stock.b = temp.b + temp.a;
			element_stock.a = 0;
		}
		else if (temp.b >= 8)
		{
			element_stock.a = temp.a - (temp.b - 7);
			element_stock.b = 7;
		}
		else
		{
			element_stock = temp;
		}
		break;
	case 3:
		//右上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;
		temp.b += tileRand;

		if (temp.a >= 8 && temp.b >= 8)
		{
			element_stock.a = 7;
			element_stock.b = 7;
		}
		else if (temp.a >= 8)
		{
			element_stock.a = 7;
			element_stock.b = temp.b - (temp.a - 7);
		}
		else if (temp.b >= 8)
		{
			element_stock.a = temp.a - (temp.b - 7);
			element_stock.b = 7;
		}
		else
		{
			element_stock = temp;
		}
		break;
	}
	return;
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
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b - i)) || element_stock.a - i < 0 || element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b - i));
			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b - i), 3);
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
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b + i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b + i), 3);
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
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b - i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b - i), 3);
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
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b + i));
			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b + i), 3);
		}
	}
}

void XIIlib::Bishop::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Bishop::Hit(int attackPoint)
{
	//int _damage = _defense_point - attackPoint;
	//if (_damage < 0) {
	//	_hit_point += _damage;
	//	is_hit = 1;
	//}

	_hit_point -= attackPoint;
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

void XIIlib::Bishop::BillObjectDraw()
{
	attackTimer->Draw(); // 攻撃タイマーの描画
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

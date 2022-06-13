#include "Bishop.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Bishop::Bishop()
{
	// ƒ‚ƒfƒ‹‚Ì‰Šú‰»
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(0, 1, 0, 1);
	// ŠeƒXƒe[ƒ^ƒX‚Ì‰Šú‰»
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Bishop::~Bishop()
{
	delete collCapsule;
<<<<<<< HEAD
=======
	delete bike;
>>>>>>> parent of 1d1fcc5 (ã‚³ãƒ¡ãƒ³ãƒˆ)
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
	// “Á‚É‚È‚µ
	_hit_point = 2;

	// ƒNƒ‰ƒXƒl[ƒ€æ“¾
	const type_info& id = typeid(Bishop);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
}

void XIIlib::Bishop::Update()
{
	// ‹î‚Ìs“®
	Action();
	// ˆÊ’uÀ•W‚ÌXV
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));
	std::cout << tileRand << "ƒrƒVƒ‡ƒbƒv" << std::endl;
	// UŒ‚“–‚½‚Á‚½‚ç
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
		isAttack = false;
		// ƒmƒbƒNƒoƒbƒN
		element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 temp = kingPos;
		// ƒmƒbƒNƒoƒbƒN‚ÉŠÔ‚É‘¼‚Ì‹î‚ª‚¢‚éê‡A‚»‚ÌŠÔ‚Ì‹î‚ÌˆÊ’u‚É~‚Ü‚é
		// c‚ÉƒmƒbƒNƒoƒbƒN
		if (dif.a == 0)
		{
			// 0‚æ‚è¬‚³‚¯‚ê‚ÎKing‚æ‚èã‚É‚¢‚é
			if (dif.b < 0)
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else // 0‚æ‚è‘å‚«‚¯‚ê‚ÎKing‚æ‚è‰º‚É‚¢‚é
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
		else if (dif.b == 0) // ‰¡‚ÉƒmƒbƒNƒoƒbƒN
		{
			// 0‚æ‚è¬‚³‚¯‚ê‚ÎKing‚æ‚è‰E
			if (dif.a < 0)
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else// 0‚æ‚è‘å‚«‚¯‚ê‚ÎKing‚æ‚è¶
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
		else	// Î‚ß‚ÉƒmƒbƒNƒoƒbƒN
		{
			// ¶ã‚ÉƒmƒbƒNƒoƒbƒN
			if (dif.a > 0 && dif.b < 0)
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
			else if (dif.a < 0 && dif.b < 0)	// ‰Eã‚ÉƒmƒbƒNƒoƒbƒN
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
			else if (dif.a > 0 && dif.b > 0)	// ¶‰º‚ÉƒmƒbƒNƒoƒbƒN
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
			else if (dif.a < 0 && dif.b > 0)	// ‰E‰º‚ÉƒmƒbƒNƒoƒbƒN
			{
				// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
			Hit(3);
		}

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
	// ”ÍˆÍ‚É“ü‚Á‚Ä‚é‚©‚Ìƒ`ƒFƒbƒN
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
			// king‚Ìƒ|ƒCƒVƒ‡ƒ“•ª‚ğˆø‚¢‚Äfor•¶‚Å’²‚×‚é
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
	if (UnitManager::GetInstance()->GetIntervalTimer() == 420)
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
	// ƒJƒEƒ“ƒg‚ğŒ¸‚ç‚·
	attackInterval--;
	//F‚ğ•Ï‚¦‚é
	if (attackInterval <= 150 && attackInterval >= 120 || attackInterval <= 90 && attackInterval >= 75 || attackInterval <= 60 && attackInterval >= 45 || attackInterval <= 30 && attackInterval >= 15)
	{
		collCapsule->SetColor(1, 0, 0, 1);
	}
	else
	{
		collCapsule->SetColor(0, 1, 0, 1);
	}
	if (attackInterval == 0)
	{
		Math::Point2 dif = kingPos - preElement_stock;
		Math::Point2 temp = element_stock;

		Math::Point2 temp_norm;
		if (dif.a == 0) { temp_norm.a = dif.a; }
		else { temp_norm.a = dif.a / abs(dif.a); }
		if (dif.b == 0) { temp_norm.b = dif.b; }
		else { temp_norm.b = dif.b / abs(dif.b); }

		// ©•ª‚ÆƒLƒ“ƒO‚ÌŠÔ‚ğ1ƒ}ƒX‚Ã‚Â’²‚×‚é
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
		// UŒ‚
		element_stock = preElement_stock;
		IniState();
		notAttackflag = FALSE;
	}
}

void XIIlib::Bishop::Move()
{

	if (isAttack == true)return;
	if (UnitManager::GetInstance()->GetIntervalTimer() < 420)return;

	notAttackflag = TRUE;

	collCapsule->SetColor(0, 1, 0, 1);
	//Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp(0, 0);
	// ‚à‚¤‚·‚Å‚É”ÍˆÍã‚É‚¢‚½‚ç“®‚©‚È‚¢
	//if (abs(dif.a) == abs(dif.b))return;
	//3ƒ}ƒXˆÈ‰º‚µ‚©“®‚¯‚È‚¢‚ÌˆÚ“®—p—”
	int BishopjMin = jMin;
	int BishopjMax = jMax;
	tileRand = 1;
	//Switch•¶—p‚Ì—”
	int BishopSwitchiMin = SwitchRandiMin;
	int BishopSwitchiMax = SwitchRandiMax;
	SwitchRand = SwitchRandiMin + (int)(rand() * (SwitchRandiMax - SwitchRandiMin + 1) / (1 + RAND_MAX));

	switch (SwitchRand)
	{
	case 0:
		//¶‰º•ûŒü			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		temp.b -= tileRand;
		
		//if (ThreeCheckArea(element_stock))
		//{

		//}

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
		//‰E‰º•ûŒü			
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
		//¶ã•ûŒü
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
		//‰Eã•ûŒü
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
			element_stock.b = temp.b - (temp.a - 7);
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
	// ”ÍˆÍ‚É“ü‚Á‚Ä‚é‚©‚Ìƒ`ƒFƒbƒN
	Math::Point2 dif = kingPos - element_stock;
	// x‚Æz‚Ìâ‘Î’l‚ªˆê‚¾‚Á‚½‚çUŒ‚”ÍˆÍ‚É‚¢‚é”ÍˆÍ
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Bishop::AttackAreaDraw()
{
	//¶‰º
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
	//‰Eã
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
	//‰E‰º
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
	//¶ã
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

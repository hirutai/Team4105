#include "Bishop.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Bishop::Bishop()
{
	// ���f���̏�����
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(0, 1, 0, 1);
	// �e�X�e�[�^�X�̏�����
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
	// ���ɂȂ�
	_hit_point = 2;

	// �N���X�l�[���擾
	const type_info& id = typeid(Bishop);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
}

void XIIlib::Bishop::Update()
{
	// ��̍s��
	Action();
	// �ʒu���W�̍X�V
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));
	std::cout << tileRand << "�r�V���b�v" << std::endl;
	// �U������������
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
		isAttack = false;
		// �m�b�N�o�b�N
		element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 temp = kingPos;
		// �m�b�N�o�b�N���ɊԂɑ��̋����ꍇ�A���̊Ԃ̋�̈ʒu�Ɏ~�܂�
		// �c�Ƀm�b�N�o�b�N
		if (dif.a == 0)
		{
			// 0��菬�������King����ɂ���
			if (dif.b < 0)
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else // 0���傫�����King��艺�ɂ���
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
		else if (dif.b == 0) // ���Ƀm�b�N�o�b�N
		{
			// 0��菬�������King���E
			if (dif.a < 0)
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						element_stock = temp;
					}
				}
			}
			else// 0���傫�����King��荶
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
		else	// �΂߂Ƀm�b�N�o�b�N
		{
			// ����Ƀm�b�N�o�b�N
			if (dif.a > 0 && dif.b < 0)
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
			else if (dif.a < 0 && dif.b < 0)	// �E��Ƀm�b�N�o�b�N
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
			else if (dif.a > 0 && dif.b > 0)	// �����Ƀm�b�N�o�b�N
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
			else if (dif.a < 0 && dif.b > 0)	// �E���Ƀm�b�N�o�b�N
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
	// �͈͂ɓ����Ă邩�̃`�F�b�N
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
			// king�̃|�C�V��������������for���Œ��ׂ�
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
	// �J�E���g�����炷
	attackInterval--;
	//�F��ς���
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

		// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
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
		// �U��
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
	// �������łɔ͈͏�ɂ����瓮���Ȃ�
	//if (abs(dif.a) == abs(dif.b))return;
	//3�}�X�ȉ����������Ȃ����̈ړ��p����
	int BishopjMin = jMin;
	int BishopjMax = jMax;
	tileRand = 1;
	//Switch���p�̗���
	int BishopSwitchiMin = SwitchRandiMin;
	int BishopSwitchiMax = SwitchRandiMax;
	SwitchRand = SwitchRandiMin + (int)(rand() * (SwitchRandiMax - SwitchRandiMin + 1) / (1 + RAND_MAX));

	switch (SwitchRand)
	{
	case 0:
		//��������			
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
		//�E������			
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
		//�������
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
		//�E�����
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
	// �͈͂ɓ����Ă邩�̃`�F�b�N
	Math::Point2 dif = kingPos - element_stock;
	// x��z�̐�Βl���ꏏ��������U���͈͂ɂ���͈�
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Bishop::AttackAreaDraw()
{
	//����
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
	//�E��
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
	//�E��
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
	//����
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

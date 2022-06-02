#include "Rook.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"

XIIlib::Rook::Rook()
{
	// ���f���̏�����
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, { 0,-1,0 }, 0.5f, 16);
	collCapsule->SetColor(0, 1, 1, 1);
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Rook::~Rook()
{
	delete collCapsule;
}

std::shared_ptr<XIIlib::Rook> XIIlib::Rook::Create(int point_x, int point_z)
{
	std::shared_ptr<Rook> rook = std::make_shared<Rook>();
	rook.get()->SetStartElement(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Rook::Initialize()
{
	// ���ɂȂ�
	_hit_point = 2;

	// �N���X�l�[���擾
	const type_info& id = typeid(Rook);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
}

void XIIlib::Rook::Update()
{
	// ��̍s��
	Action();


	// �ʒu���W�̍X�V
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));

	// �U������������
	if (UnitManager::GetInstance()
		->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		Hit(1);
		// �m�b�N�o�b�N
		element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

		if (Common::GetExceptionPoint(element_stock.a) || Common::GetExceptionPoint(element_stock.b)) {
			Hit(3);
		}

		element_stock = Common::OffsetTilePosition2(element_stock);
	}

	collCapsule->Update();
}

void XIIlib::Rook::Draw()
{
	collCapsule->Draw();
}

void XIIlib::Rook::SetStartElement(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::Rook::Action()
{

	//if (UnitManager::GetInstance()->GetIntervalTimer() > 0)return;

	// �͈͂ɓ����Ă邩�̃`�F�b�N
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

	if (isAttack == true && notAttackflag == true)
	{
		point_attack = preElement_stock;
		//�U���}�X�̐F��ς���
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
	}
	else
	{
		isAttack = false;
		//�ړ��͈͂̐F�t��
		AttackAreaDraw();
	}


}

void XIIlib::Rook::Attack()
{
	// �J�E���g�����炷
	attackInterval--;
	//�F��ς���
	collCapsule->SetColor(1, 0, 0, 1);
	if (attackInterval == 0)
	{
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 temp = element_stock;

		// �c�ɏd�Ȃ��Ă���
		if (dif.a == 0)
		{
			// 0��菬�������King����ɂ���
			if (dif.b < 0)
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}
			else // 0���傫�����King��艺�ɂ���
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.b) - 1; ++i)
				{
					temp.b++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}

		}
		else // ���ɏd�Ȃ��Ă���
		{
			// 0��菬�������King���E
			if (dif.a < 0)
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a--;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}
			else// 0���傫�����King��荶
			{
				// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
				for (int i = 0; i < abs(dif.a) - 1; ++i)
				{
					temp.a++;
					if (UnitManager::GetInstance()->AllOnUnit(temp))
					{
						IniState();
						return;
					}
				}
			}
		}
		if (AttackAreaExists()) { preElement_stock = kingPos; }
		// �U��
		element_stock = preElement_stock;

		IniState();
		notAttackflag = false;
	}
}

void XIIlib::Rook::Move()
{
	if (isAttack == true)return;
	if (UnitManager::GetInstance()->GetIntervalTimer() > 0)return;
	notAttackflag = TRUE;

	collCapsule->SetColor(0, 1, 1, 1);
	Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp = element_stock;
	if (abs(dif.a) == 1 && abs(dif.b) == 1)return;
	// ������X�̂ق����傫��
	if (abs(dif.a) > abs(dif.b))
	{
		//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
		if (dif.b < 0)//0��菬�������King����ɂ���
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			for (int i = 0; i < abs(dif.b); ++i)
			{
				temp.b--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
				element_stock.b--;
			}
		}
		else // 0���傫�����King��艺�ɂ���
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			for (int i = 0; i < abs(dif.b); ++i)
			{
				temp.b++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
				element_stock.b++;
			}
		}
	}
	else //������Z�̕����傫�� (or �ǂ������ꏏ�̎�Z�D��)
	{
		//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
		if (dif.a < 0)//0��菬�������King��荶�ɂ���
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			for (int i = 0; i < abs(dif.a); ++i)
			{
				temp.a--;
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
				element_stock.a--;
			}
		}
		else // 0���傫�����King��艺�ɂ���
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			for (int i = 0; i < abs(dif.a); ++i)
			{
				temp.a++;
				if (UnitManager::GetInstance()->AllOnUnit(temp))return;
				element_stock.a++;
			}
		}
	}
}

bool XIIlib::Rook::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// �͈͂ɓ����Ă邩�̃`�F�b�N
	if (kingPos.a == element_stock.a || kingPos.b == element_stock.b)return true;
	return false;
}

void XIIlib::Rook::AttackAreaDraw()
{
	for (int i = 0; i < 8; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, element_stock.b), 3);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, i), 3);
	}
}

void XIIlib::Rook::IniState()
{
	isAttack = false;
	attackInterval = 60;
	collCapsule->SetColor(0, 1, 1, 1);
}

void XIIlib::Rook::Hit(int attackPoint)
{
	//int _damage = _defense_point - attackPoint;
	//if (_damage < 0) {
	//	_hit_point += _damage;
	//	is_hit = 1;
	//}

	_hit_point -= attackPoint;
}

void XIIlib::Rook::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::Rook::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Rook::CreateAttackArea()
{

}
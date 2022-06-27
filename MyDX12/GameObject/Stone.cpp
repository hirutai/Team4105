#include "Stone.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"

XIIlib::Stone::Stone()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Stone::~Stone()
{
	delete object3d;
}

std::shared_ptr<XIIlib::Stone> XIIlib::Stone::Create(int point_x, int point_z)
{
	std::shared_ptr<Stone> rook = std::make_shared<Stone>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Stone::Initialize()
{
	// ���ɂȂ�
	_hit_point = 2;

	// �N���X�l�[���擾
	const type_info& id = typeid(Stone);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});

}

void XIIlib::Stone::Update()
{

	// ��̍s��
	Action();
	// �ʒu���W�̍X�V
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// �U������������
//	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		//Hit(1);
	//	isAttack = false;
		// �m�b�N�o�b�N
	//	element_stock += UnitManager::GetInstance()->GetBackVector(element_stock);

	//	Math::Point2 dif = kingPos - element_stock;
	//	Math::Point2 temp = kingPos;
	//	// �m�b�N�o�b�N���ɊԂɑ��̋����ꍇ�A���̊Ԃ̋�̈ʒu�Ɏ~�܂�
	//	// �c�Ƀm�b�N�o�b�N
	//	if (dif.a == 0)
	//	{
	//		// 0��菬�������King����ɂ���
	//		if (dif.b < 0)
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.b) - 1; ++i)
	//			{
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//		else // 0���傫�����King��艺�ɂ���
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.b) - 1; ++i)
	//			{
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//	}
	//	else if (dif.b == 0) // ���Ƀm�b�N�o�b�N
	//	{
	//		// 0��菬�������King���E
	//		if (dif.a < 0)
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//		else// 0���傫�����King��荶
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//				}
	//			}
	//		}
	//	}
	//	else	// �΂߂Ƀm�b�N�o�b�N
	//	{
	//		// ����Ƀm�b�N�o�b�N
	//		if (dif.a > 0 && dif.b < 0)
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;

	//				}
	//			}
	//		}
	//		else if (dif.a < 0 && dif.b < 0)	// �E��Ƀm�b�N�o�b�N
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				temp.b++;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//		else if (dif.a > 0 && dif.b > 0)	// �����Ƀm�b�N�o�b�N
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a--;
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//		else if (dif.a < 0 && dif.b > 0)	// �E���Ƀm�b�N�o�b�N
	//		{
	//			// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//			for (int i = 0; i < abs(dif.a) - 1; ++i)
	//			{
	//				temp.a++;
	//				temp.b--;
	//				if (UnitManager::GetInstance()->AllOnUnit(temp))
	//				{
	//					element_stock = temp;
	//					break;
	//				}
	//			}
	//		}
	//	}


	//	if (Common::GetExceptionPoint(element_stock.a) || Common::GetExceptionPoint(element_stock.b)) {
	//		Hit(3);
	//	}

	//}

	object3d->Update();
}

void XIIlib::Stone::Action()
{
	//// �͈͂ɓ����Ă邩�̃`�F�b�N
	//if (AttackAreaExists())
	//{
	//	Math::Point2 dif = kingPos - element_stock;
	//	Math::Point2 norm(0, 0);
	//	if (dif.a != 0) { norm.a = dif.a / abs(dif.a); }
	//	else { norm.a = dif.a; }
	//	if (dif.b != 0) { norm.b = dif.b / abs(dif.b); }
	//	else { norm.b = dif.b; }

	//	if (dif.a == 0)
	//	{
	//		// king�̃|�C�V��������������for���Œ��ׂ�
	//		if (MoveAreaCheck(element_stock, norm, dif.b))return;
	//	}
	//	else
	//	{
	//		// king�̃|�C�V��������������for���Œ��ׂ�
	//		if (MoveAreaCheck(element_stock, norm, dif.a))return;
	//	}

	//	if (isAttack == false)
	//	{
	//		isAttack = true;
	//		preElement_stock = kingPos;
	//	}
	//}
	//else
	//{
	//	Move();
	//}

	//if (UnitManager::GetInstance()->GetIntervalTimer() == 420)
	//{
	//	notAttackflag = TRUE;
	//}

	//if (isAttack == true && notAttackflag == true)
	//{
	//	point_attack = preElement_stock;
	//	//�U���}�X�̐F��ς���
	//	UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
	//	Attack();
	//}
	//else
	//{
	//	isAttack = false;
	//	//�ړ��͈͂̐F�t��
	//	AttackAreaDraw();
	//}

}

void XIIlib::Stone::Attack()
{
	//// �J�E���g�����炷
	//attackInterval--;
	////�F��ς���
	//if (attackInterval <= 150 && attackInterval >= 120 || attackInterval <= 90 && attackInterval >= 75 || attackInterval <= 60 && attackInterval >= 45 || attackInterval <= 30 && attackInterval >= 15)
	//{
	//	collCapsule->SetColor(1, 0, 0, 1);
	//}
	//else
	//{
	//	collCapsule->SetColor(0, 1, 1, 1);
	//}
	//if (attackInterval == 0)
	//{
	//	Math::Point2 dif = kingPos - preElement_stock;
	//	Math::Point2 temp = element_stock;

	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	if (dif.b <= 0 && dif.a == element_stock.a)//�L���O��艺�ɂ��邩�𒲂ׂ�
	//	{
	//		//��ɂP�i��
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.b > 0 && dif.a == element_stock.a)//�L���O����ɂ��邩�𒲂ׂ�
	//	{
	//		//����1�i��
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.b <= 0 && dif.a <= 0)//�L���O��荶���ɂ��邩�𒲂ׂ�
	//	{
	//		//�E���1�i��
	//		temp.a -= 1;
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.b > 0 && dif.a <= 0)//�L���O��荶��ɂ��邩�𒲂ׂ�
	//	{
	//		//�E����1�i��
	//		temp.a -= 1;
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.b <= 0 && dif.a > 0)//�L���O���E���ɂ��邩�𒲂ׂ�
	//	{
	//		//�����1�i��
	//		temp.a += 1;
	//		temp.b -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.b > 0 && dif.a >= 0)//�L���O���E��ɂ��邩�𒲂ׂ�
	//	{
	//		//������1�i��
	//		temp.a += 1;
	//		temp.b += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//	else if (dif.a <= 0 && dif.b == element_stock.b)//�L���O��荶�ɂ��邩�𒲂ׂ�
	//	{
	//		//�E�ɂP�i��
	//		temp.a -= 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	else if (dif.a > 0 && dif.b == element_stock.b)// 0���傫�����King���E�ɂ���
	//	{
	//		//���ɂP�i��
	//		temp.a += 1;
	//		if (UnitManager::GetInstance()->AllOnUnit(temp))
	//		{
	//			IniState();
	//			return;
	//		}
	//	}
	//	// �U��
	//	element_stock = preElement_stock;

	//	IniState();
	//	notAttackflag = false;
	//}
}

void XIIlib::Stone::Move()
{
	////�U���t���O
	//if (isAttack == true)return;
	////�ړ��܂ł̃C���^�[�o��
	//if (UnitManager::GetInstance()->GetIntervalTimer() < 420)return;
	////�����L�[�̍��W
	//Math::Point2 temp = element_stock;
	//notAttackflag = TRUE;

	//collCapsule->SetColor(0, 1, 1, 1);

	////�v���C���[�̍��W �[ �����L�[�̍��W
	//Math::Point2 dif = kingPos - element_stock;

	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//if (dif.b < 0 && dif.a == 0)//�L���O��艺�ɂ��邩�𒲂ׂ�
	//{
	//	//��ɂP�i��
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.b -= 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.b > 0 && dif.a == 0)//�L���O����ɂ��邩�𒲂ׂ�
	//{
	//	//����1�i��
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.b += 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.a < 0 && dif.b == 0)//�L���O��荶�ɂ��邩�𒲂ׂ�
	//{
	//	//�E�ɂP�i��
	//	temp.a -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//}
	//else if (dif.a > 0 && dif.b == 0)// 0���傫�����King���E�ɂ���
	//{
	//	//���ɂP�i��
	//	temp.a += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.b <= 0 && dif.a <= 0)//�L���O��荶���ɂ��邩�𒲂ׂ�
	//{
	//	//�E���1�i��
	//	temp.a -= 1;
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//	element_stock.b -= 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.b > 0 && dif.a <= 0)//�L���O��荶��ɂ��邩�𒲂ׂ�
	//{
	//	//�E����1�i��
	//	temp.a -= 1;
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a -= 1;
	//	element_stock.b += 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.b <= 0 && dif.a > 0)//�L���O���E���ɂ��邩�𒲂ׂ�
	//{
	//	//�����1�i��
	//	temp.a += 1;
	//	temp.b -= 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//	element_stock.b -= 1;
	//}
	//// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	//else if (dif.b > 0 && dif.a >= 0)//�L���O���E��ɂ��邩�𒲂ׂ�
	//{
	//	//������1�i��
	//	temp.a += 1;
	//	temp.b += 1;
	//	if (ThreeCheckArea(temp))return;
	//	element_stock.a += 1;
	//	element_stock.b += 1;
	//}
}

bool XIIlib::Stone::AttackAreaExists()
{
	//int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	//if (element != -1)
	//{
	//	std::shared_ptr<King> p_king =
	//		std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
	//	kingPos = p_king->GetElementStock();
	//}
	//// �͈͂ɓ����Ă邩�̃`�F�b�N
	//if (kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b ||     //�E
	//	kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b + 1 || //�E��
	//	kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b - 1 || //�E��
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b ||     //��
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b - 1 || //����
	//	kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b + 1 || //����
	//	kingPos.a == element_stock.a && kingPos.b == element_stock.b + 1 ||     //��
	//	kingPos.a == element_stock.a && kingPos.b == element_stock.b - 1        //��
	//	)return true;
	return false;
}

void XIIlib::Stone::AttackAreaDraw()
{
	
}

void XIIlib::Stone::IniState()
{
	//isAttack = false;
	//attackInterval = 180;
	//collCapsule->SetColor(0, 1, 1, 1);
}

void XIIlib::Stone::CreateAttackArea()
{

}

bool XIIlib::Stone::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	//Math::Point2 pos = crPos;
	//for (int i = 0; i < abs(tileNum) - 1; ++i)
	//{
	//	pos += vec;
	//	if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	//}
	return false;
}

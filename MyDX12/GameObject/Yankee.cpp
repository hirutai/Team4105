#include "Yankee.h"
#include "King.h"
#include "../3D/CollCapsule.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../GameObject/AttackTimer.h"

XIIlib::Yankee::Yankee()
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

XIIlib::Yankee::~Yankee()
{
	delete attackTimer;
	delete collCapsule;
}

std::shared_ptr<XIIlib::Yankee> XIIlib::Yankee::Create(int point_x, int point_z)
{
	std::shared_ptr<Yankee> rook = std::make_shared<Yankee>();
	rook.get()->SetStartElement(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Yankee::Initialize()
{
	// ���ɂȂ�
	_hit_point = 2;

	// �N���X�l�[���擾
	const type_info& id = typeid(Yankee);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));

	attackTimer = new AttackTimer(5);
	attackTimer->Initialize();
}

void XIIlib::Yankee::Update()
{

	// ��̍s��
	Action();
	// �ʒu���W�̍X�V
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a), 1.0f,
		Common::ConvertTilePosition(element_stock.b));
	std::cout << tileRand << std::endl;

	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// �U������������
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE)) {
		Hit(1);
		isAttack = false;
		// �m�b�N�o�b�N�̈ړ���
		const Math::Point2 backVector = UnitManager::GetInstance()->GetBackVector(element_stock);

		// �m�b�N�o�b�N
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
	}

	attackTimer->Timer();
	attackTimer->SetPosition(object3d->position);

	collCapsule->Update();
	object3d->Update();
}

void XIIlib::Yankee::Draw()
{
	//collCapsule->Draw();
	object3d->Draw();
}

void XIIlib::Yankee::SetStartElement(int x, int z)
{
	startElement_stock = Math::Point2(x, z);
	element_stock = Math::Point2(x, z);
}

void XIIlib::Yankee::Action()
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

		if (dif.a == 0)
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			if (MoveAreaCheck(element_stock, norm, dif.b))return;
		}
		else
		{
			// king�̃|�C�V��������������for���Œ��ׂ�
			if (MoveAreaCheck(element_stock, norm, dif.a))return;
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

void XIIlib::Yankee::Attack()
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
		collCapsule->SetColor(0, 1, 1, 1);
	}
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		// �U��
		element_stock = preElement_stock;

		IniState();
		//notAttackflag = false;
	}
}

void XIIlib::Yankee::Move()
{
	//�U���t���O
	if (isAttack == true)return;
	//�ړ��܂ł̃C���^�[�o��
	if (!attackTimer->SizeZeroFlag())return;
	//�����L�[�̍��W
	Math::Point2 temp = element_stock;
	notAttackflag = TRUE;

	collCapsule->SetColor(0, 1, 1, 1);

	//�v���C���[�̍��W �[ �����L�[�̍��W
	Math::Point2 dif = kingPos - element_stock;

	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	if (dif.b < 0 && dif.a == 0)//�L���O��艺�ɂ��邩�𒲂ׂ�
	{
		//��ɂP�i��
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		element_stock.b -= 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a == 0)//�L���O����ɂ��邩�𒲂ׂ�
	{
		//����1�i��
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.b += 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.a < 0 && dif.b == 0)//�L���O��荶�ɂ��邩�𒲂ׂ�
	{
		//�E�ɂP�i��
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a -= 1;
	}
	else if (dif.a > 0 && dif.b == 0)// 0���傫�����King���E�ɂ���
	{
		//���ɂP�i��
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a += 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b <= 0 && dif.a <= 0)//�L���O��荶���ɂ��邩�𒲂ׂ�
	{
		//�E���1�i��
		temp.a -= 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a -= 1;
		element_stock.b -= 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a <= 0)//�L���O��荶��ɂ��邩�𒲂ׂ�
	{
		//�E����1�i��
		temp.a -= 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a -= 1;
		element_stock.b += 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b <= 0 && dif.a > 0)//�L���O���E���ɂ��邩�𒲂ׂ�
	{
		//�����1�i��
		temp.a += 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a += 1;
		element_stock.b -= 1;
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a >= 0)//�L���O���E��ɂ��邩�𒲂ׂ�
	{
		//������1�i��
		temp.a += 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a += 1;
		element_stock.b += 1;
	}
}

bool XIIlib::Yankee::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// �͈͂ɓ����Ă邩�̃`�F�b�N
	if (kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b ||     //�E
		kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b + 1 || //�E��
		kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b - 1 || //�E��
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b ||     //��
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b - 1 || //����
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b + 1 || //����
		kingPos.a == element_stock.a && kingPos.b == element_stock.b + 1 ||     //��
		kingPos.a == element_stock.a && kingPos.b == element_stock.b - 1        //��
		)return true;
	return false;
}

void XIIlib::Yankee::AttackAreaDraw()
{
	//�������ɋ���邩

	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b)) || element_stock.a - 1 < 0)
	{
		
	}
	//�Ȃ�������ړ��͈͕`��
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b));
	}	
	//�E�����ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b)) || element_stock.a + 1 > 7)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b));
	}
	//�������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b - 1)) || element_stock.b - 1 < 0)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a , element_stock.b - 1));
	}
	//������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b + 1)) || element_stock.b + 1 > 7)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b + 1));
	}
	//���������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b - 1)) || element_stock.a - 1 < 0 || element_stock.b - 1 < 0)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b - 1));
	}
	//�E�������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b - 1)) || element_stock.a + 1 > 7 || element_stock.b - 1 < 0)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b - 1));
	}
	//��������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b + 1)) || element_stock.a - 1 < 0 || element_stock.b + 1 > 7)
	{
		
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b + 1));
	}
	//�E������ɋ���邩
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b +1)) || element_stock.a + 1 > 7 || element_stock.b + 1 > 7)
	{
	
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b + 1));
	}
}

void XIIlib::Yankee::IniState()
{
	isAttack = false;
	attackInterval = 180;
	collCapsule->SetColor(0, 1, 1, 1);
}

void XIIlib::Yankee::Hit(int attackPoint)
{
	//int _damage = _defense_point - attackPoint;
	//if (_damage < 0) {
	//	_hit_point += _damage;
	//	is_hit = 1;
	//}

	_hit_point -= attackPoint;
}

void XIIlib::Yankee::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::Yankee::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::Yankee::CreateAttackArea()
{

}

void XIIlib::Yankee::BillObjectDraw()
{
	attackTimer->Draw(); // �U���^�C�}�[�̕`��
}

bool XIIlib::Yankee::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

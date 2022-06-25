#include "Boss.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"

XIIlib::Boss::Boss()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Boss::~Boss()
{
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Boss> XIIlib::Boss::Create(int point_x, int point_z)
{
	std::shared_ptr<Boss> rook = std::make_shared<Boss>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Boss::Initialize()
{
	// ���ɂȂ�
	_hit_point = 2;

	// �N���X�l�[���擾
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();

	SetAttackTimer(5);

	nextPoint = { 0,0 };
}

void XIIlib::Boss::Update()
{

	// ��̍s��
	Action();

	// �^�C�}�[�̍X�V
	attackTimer->Timer();

	// �ʒu���W�̍X�V
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	object3d->Update();
	// ���W�ݒ�
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Boss::Action()
{
	// �����ւ̉h���̃��[�h��������
	nextPoint = { 0,0 };

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

	// �m�b�N�o�b�N(���ʏ���)
	KnockBack();

}

void XIIlib::Boss::Attack()
{
	// �J�E���g�����炷
	attackInterval--;
	//�F��ς���
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		// �U��
		element_stock = preElement_stock;
		audio_->PlaySE("yankeeVoice.wav");
		IniState();
		//notAttackflag = false;
	}
}

void XIIlib::Boss::Move()
{
	//�U���t���O
	if (isAttack == true)return;
	//�ړ��܂ł̃C���^�[�o��
	if (!attackTimer->SizeZeroFlag())return;
	//�����L�[�̍��W
	Math::Point2 temp = element_stock;
	notAttackflag = TRUE;


	//�v���C���[�̍��W �[ �����L�[�̍��W
	Math::Point2 dif = kingPos - element_stock;

	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	if (dif.b < 0 && dif.a == 0)//�L���O��艺�ɂ��邩�𒲂ׂ�
	{
		//��ɂP�i��
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;

		//element_stock.b -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a == 0)//�L���O����ɂ��邩�𒲂ׂ�
	{
		//����1�i��
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.b += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.a < 0 && dif.b == 0)//�L���O��荶�ɂ��邩�𒲂ׂ�
	{
		//�E�ɂP�i��
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	else if (dif.a > 0 && dif.b == 0)// 0���傫�����King���E�ɂ���
	{
		//���ɂP�i��
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		element_stock.a += 1;
		audio_->PlaySE("yankeeVoice.wav");
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
		audio_->PlaySE("yankeeVoice.wav");
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
		audio_->PlaySE("yankeeVoice.wav");
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
		audio_->PlaySE("yankeeVoice.wav");
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
		audio_->PlaySE("yankeeVoice.wav");
	}
}

bool XIIlib::Boss::AttackAreaExists()
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

void XIIlib::Boss::AttackAreaDraw()
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
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b - 1));
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
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b + 1)) || element_stock.a + 1 > 7 || element_stock.b + 1 > 7)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b + 1));
	}
}

void XIIlib::Boss::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Boss::CreateAttackArea()
{

}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}
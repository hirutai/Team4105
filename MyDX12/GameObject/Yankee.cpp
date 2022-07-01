#include "Yankee.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/DebugText.h"

XIIlib::Yankee::Yankee()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Yankee::~Yankee()
{
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Yankee> XIIlib::Yankee::Create(int point_x, int point_z)
{
	std::shared_ptr<Yankee> rook = std::make_shared<Yankee>();
	rook.get()->SetElementStock(point_x, point_z);
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
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});
	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();

	SetAttackTimer(5);

	nextPoint = { 0,0 };
}

void XIIlib::Yankee::Update()
{
	// �ʒu���W�̍X�V
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	if (!determinateMoveAction) {
		// ��̍s��
		Action();

		// �^�C�}�[�̍X�V
		attackTimer->Timer();

		pos = object3d->position;
	}
	else {
		// ���[�V��������
		Motion();
	}
	object3d->Update();

	// ���W�ݒ�
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Yankee::Action()
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

void XIIlib::Yankee::Attack()
{
	// �J�E���g�����炷
	attackInterval--;
	//�F��ς���
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		// �U��
		nextPoint = preElement_stock;
		Math::Point2 addResult = element_stock - nextPoint;
		
		if (Math::MatchPoint2(addResult,pointArray[static_cast<int>(RotState::UP_LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_LEFT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::UP)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::UP_RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN_RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN_LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_LEFT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::LEFT)];
		}

		audio_->PlaySE("yankeeVoice.wav");
		IniState();

		// �ړ��܂������肳��܂����B
		determinateMoveAction = true;
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
	nextPoint = element_stock;
	notAttackflag = TRUE;


	//�v���C���[�̍��W �[ �����L�[�̍��W
	Math::Point2 dif = kingPos - element_stock;

	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	if (dif.b < 0 && dif.a == 0)//�L���O��艺�ɂ��邩�𒲂ׂ�
	{
		//��ɂP�i��
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;

		nextPoint.b -= 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::UP)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a == 0)//�L���O����ɂ��邩�𒲂ׂ�
	{
		//����1�i��
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.b += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.a < 0 && dif.b == 0)//�L���O��荶�ɂ��邩�𒲂ׂ�
	{
		//�E�ɂP�i��
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::RIGHT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	else if (dif.a > 0 && dif.b == 0)// 0���傫�����King���E�ɂ���
	{
		//���ɂP�i��
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::LEFT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b <= 0 && dif.a <= 0)//�L���O��荶���ɂ��邩�𒲂ׂ�
	{
		//�E���1�i��
		temp.a -= 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		nextPoint.b -= 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_RIGHT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a <= 0)//�L���O��荶��ɂ��邩�𒲂ׂ�
	{
		//�E����1�i��
		temp.a -= 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		nextPoint.b += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_RIGHT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b <= 0 && dif.a > 0)//�L���O���E���ɂ��邩�𒲂ׂ�
	{
		//�����1�i��
		temp.a += 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		nextPoint.b -= 1;
		object3d->rotation.y =
			rotArray[static_cast<int>(RotState::UP_LEFT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a >= 0)//�L���O���E��ɂ��邩�𒲂ׂ�
	{
		//������1�i��
		temp.a += 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		nextPoint.b += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_LEFT)];
		audio_->PlaySE("yankeeVoice.wav");
	}

	// �ړ��܂������肳��܂����B
	determinateMoveAction = true;
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
}

void XIIlib::Yankee::CreateAttackArea()
{

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

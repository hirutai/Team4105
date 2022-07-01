#include "Boss.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/DebugText.h"
#include "../Tool/Easing.h"

XIIlib::Boss::Boss()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = defaultHp;
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
	_hit_point = defaultHp;

	// �N���X�l�[���擾
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });
	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();

	SetAttackTimer(2);

	nextPoint = { 0,0 };
}

void XIIlib::Boss::Update()
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
		const float maxTime = 1.0f;
		movingTimer += (1.0f / 40.0f);
		Math::Vector3 nowP = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
		Math::Vector3 nextP = { Common::ConvertTilePosition(nextPoint.a),1.0f, Common::ConvertTilePosition(nextPoint.b) };
		Math::Vector3 v = nextP - nowP;

		bool isVx = false, isVz = false;
		if (v.x < 0.0f) {
			isVx = true;
			v.x *= -1.0f;
		}
		if (v.z < 0.0f) {
			isVz = true;
			v.z *= -1.0f;
		}

		float resultX = Easing::InOutCubic(movingTimer, 0.0f, v.x, maxTime);
		float resultZ = Easing::InOutCubic(movingTimer, 0.0f, v.z, maxTime);
		if (isVx)resultX *= -1.0f;
		if (isVz)resultZ *= -1.0f;

		object3d->position.x = pos.x + resultX;
		object3d->position.z = pos.z + resultZ;

		if (movingTimer >= maxTime) {
			determinateMoveAction = false;
			element_stock = nextPoint;
			movingTimer = 0.0f;
			nextPoint = Math::Point2(0, 0);
			pos = Math::Vector3();
		}
	}
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
		if (type_attack != AREA::NONE) {
			std::vector<Math::Point2> container = attack_area[(int)type_attack];
			int add_element = 0;
			Math::Vector2 at[3];
			for (int i = 0; i < 3; i++) {
				container[i] += element_stock;
				if ((container[i].a < 8 && container[i].a >= 0) && (container[i].b < 8 && container[i].b >= 0)) {
					audio_->PlaySE("swing.wav");
					UnitManager::GetInstance()->ChangeAttackValidTile(container[i], (int)type);
					Math::Point2 vec_point = container[i] - element_stock;
					UnitManager::GetInstance()->SetBackVector(container[i], vec_point * 2);
				}
			}
		}
	}
	else
	{
		isAttack = false;
		//�ړ��͈͂̐F�t��
		AttackAreaDraw();
	}

	// �m�b�N�o�b�N
	// �U���������Ă��Ȃ���΂����Ԃ�
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		_hit_point--;
	}
}

void XIIlib::Boss::Attack()
{
	//�F��ς���
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		//Math::Point2 tmpAttackArea;

		/*if (KeyInput::GetInstance()->Push(DIK_UP)) {
			type_attack = AREA::UP;
			tmpAttackArea = attack_area[(int)type_attack][1];
			object3d->rotation.y = 180.0f;
			attackAreasBillboard->SetRotation(0, 0, 0);
			attackAreasBillboard->SetPosition(
				Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
				-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
			);
		}
		else if (KeyInput::GetInstance()->Push(DIK_LEFT)) {
			type_attack = AREA::LEFT;
			tmpAttackArea = attack_area[(int)type_attack][1];
			object3d->rotation.y = 90.0f;
			attackAreasBillboard->SetRotation(0, -90, 0);
			attackAreasBillboard->SetPosition(
				Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
				-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
			);
		}
		else if (KeyInput::GetInstance()->Push(DIK_DOWN)) {
			type_attack = AREA::DOWN;
			tmpAttackArea = attack_area[(int)type_attack][1];
			object3d->rotation.y = 0.0f;
			attackAreasBillboard->SetRotation(0, 180, 0);
			attackAreasBillboard->SetPosition(
				Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
				-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
			);
		}
		else if (KeyInput::GetInstance()->Push(DIK_RIGHT)) {
			type_attack = AREA::RIGHT;
			tmpAttackArea = attack_area[(int)type_attack][1];
			object3d->rotation.y = -90.0f;
			attackAreasBillboard->SetRotation(0, 90, 0);
			attackAreasBillboard->SetPosition(
				Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
				-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
			);
		}
		else {
			type_attack = AREA::NONE;
			attackAreasBillboard->SetPosition(
				object3d->position.x,
				-1.0f,
				object3d->position.z
			);
		}*/

		/*type_attack = AREA::DOWN;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = 0.0f;
		attackAreasBillboard->SetRotation(0, 180, 0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);*/
		// �U��
		nextPoint = preElement_stock;
		audio_->PlaySE("yankeeVoice.wav");
		IniState();

		// �ړ��܂������肳��܂����B
		determinateMoveAction = true;
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
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.b > 0 && dif.a == 0)//�L���O����ɂ��邩�𒲂ׂ�
	{
		//����1�i��
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.b += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// �����ƃL���O�̊Ԃ�1�}�X�Â��ׂ�
	else if (dif.a < 0 && dif.b == 0)//�L���O��荶�ɂ��邩�𒲂ׂ�
	{
		//�E�ɂP�i��
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	else if (dif.a > 0 && dif.b == 0)// 0���傫�����King���E�ɂ���
	{
		//���ɂP�i��
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
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
		audio_->PlaySE("yankeeVoice.wav");
	}

	// �ړ��܂������肳��܂����B
	determinateMoveAction = true;
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

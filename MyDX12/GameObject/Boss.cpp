#include "Boss.h"
#include "BossHP.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../GameObject/Stone.h"
#include "ModelLoader.h"
#include "BossAttack.h"

XIIlib::Boss::Boss()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = BossHP::GetInstance()->GetBossHP();
	_attack_point = 3;
	_defense_point = 1;
}

XIIlib::Boss::~Boss()
{
	delete carobj;
	delete object3d2;
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Boss> XIIlib::Boss::Create(int point_x, int point_z)
{
	std::shared_ptr<Boss> boss = std::make_shared<Boss>();
	boss.get()->SetElementStock(point_x, point_z);
	boss.get()->Initialize();

	return std::move(boss);
}

void XIIlib::Boss::Initialize()
{
	// �N���X�l�[���擾
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;

	BossHP::GetInstance()->Initialize();
	// �傫���ƍ��W�̒萔
	const Math::Vector3 scale = { 1.5f,1.5f,1.5f };
	const Math::Vector3 position = {
		Common::ConvertTilePosition(element_stock.a) - 2.7f,
		1.0f,
		Common::ConvertTilePosition(element_stock.b)
	};

	// ---------- Object3D�̏����ݒ� ----------
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS));
	object3d->scale = scale;
	object3d->position = position;
	object3d2 = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS2));
	object3d2->scale = scale;
	object3d2->position = position;
	carobj = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSSCAR));
	carobj->scale = scale;
	carobj->position = {0,50.0f,0};

	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();
	//�U���Q�[�W�̕b��
	SetAttackTimer(300, CountType::FRAME);
	// ���W�ݒ�
	attackTimer->SetPosition(object3d->position + Math::Vector3(0.0f, 18.0f, 0.0f));
	// Attack�G���A�\���e��ϐ�������
	BossAttack::GetInstance()->InitAttackDisplay();
}

void XIIlib::Boss::Update()
{
	// ���݂�HP���擾
	_hit_point = BossHP::GetInstance()->GetBossHP();
	// �f�o�b�O��HP�\��
	std::cout << "HP" << _hit_point << std::endl;

	// �X�C�b�`
	if (switching == true)
	{
		switchingCount++;
	}
	if (switchingCount >= 30)
	{
		switching = false;
		switchingCount = 0;
	}

	// Boss��HP���f�t�H���g��HP�̔����ȉ��łȂ�����Boss���ʏ�̏�Ԃőҋ@���Ȃ�
	if (_hit_point <= BossHP::GetInstance()->GetDefaultBossHP() / 2 && bossType == BossType::normal && bossState == BossState::wait)
	{
		// �{�X�^�C�v��������Ԃ�
		bossType = BossType::strong;
		// �{�X�̍U���Ԋu��ύX
		SetAttackTimer(180, CountType::FRAME);
		attackTimer->SetPosition(Math::Vector3(0.0f, 19.0f, 0.0f));

		// �{�X�̎���ɕǂ𐶐�
		std::shared_ptr<Stone> stone = std::move(Stone::Create(1, 7));
		std::shared_ptr<Stone> stone2 = std::move(Stone::Create(6, 7));
		std::shared_ptr<Stone> stone3 = std::move(Stone::Create(1, 6));
		std::shared_ptr<Stone> stone4 = std::move(Stone::Create(2, 6));
		std::shared_ptr<Stone> stone5 = std::move(Stone::Create(3, 6));
		std::shared_ptr<Stone> stone6 = std::move(Stone::Create(4, 6));
		std::shared_ptr<Stone> stone7 = std::move(Stone::Create(5, 6));
		std::shared_ptr<Stone> stone8 = std::move(Stone::Create(6, 6));
		UnitManager::GetInstance()->AddUnit(std::move(stone));
		UnitManager::GetInstance()->AddUnit(std::move(stone2));
		UnitManager::GetInstance()->AddUnit(std::move(stone3));
		UnitManager::GetInstance()->AddUnit(std::move(stone4));
		UnitManager::GetInstance()->AddUnit(std::move(stone5));
		UnitManager::GetInstance()->AddUnit(std::move(stone6));
		UnitManager::GetInstance()->AddUnit(std::move(stone7));
		UnitManager::GetInstance()->AddUnit(std::move(stone8));
	}
	BossHP::GetInstance()->Update();
	if (!determinateMoveAction) {
		// Boss�̍s��
		Action();

		// �^�C�}�[�̍X�V
		attackTimer->Timer();
	}
	if (determinateMoveAction) {
		// �U������
		Attack();
	}

	// ���G���萧��
	if (object3d->position.y >= COLISION_HEIGHT && !invincibleFlag)
	{
		// ���G��ԕt�^
		invincibleFlag = true;
	}
	if (object3d->position.y < COLISION_HEIGHT && invincibleFlag)
	{
		// ���G��ԉ���
		invincibleFlag = false;
	}

	// ���ډ���ꂽ���̏���
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		if (invincibleFlag)return;
		BossHP::GetInstance()->Damage();
		BossAttack::GetInstance()->KnockBackAttack(element_stock);
	}

	// object��Update
	object3d->Update();
	object3d2->Update();
	carobj->Update();
}

void XIIlib::Boss::Draw()
{
	/*if (switchingCount >= 1)
	{
		object3d2->Draw();
		if (switchingCount <= 15)
		{
			object3d2->position.y += 0.3f;
		}
		if (switchingCount >= 15 && switchingCount <= 30)
		{
			object3d2->position.y -= 0.3f;
		}
	}
	else {
		object3d->Draw();
	}*/
	object3d->Draw();
	carobj->Draw();
}

void XIIlib::Boss::Action()
{
	// �A�^�b�N�^�C�}�[�ɂ���čs����ω�
	if (attackTimer->SizeZeroFlag()) // �A�^�b�N�^�C�}�[��0(�ȉ�)�ɂȂ�����ʂ�
	{
		switching = true;
		// Attack�\���̏�����
		BossAttack::GetInstance()->InitAttackDisplay();
		// �U�����J�n����̂�true
		determinateMoveAction = true;
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag()) // �A�^�b�N�^�C�}�[��3����1�ɂȂ�����ʂ�(1��)
	{
		// king(player)�̍��W���擾
		int element = UnitManager::GetInstance()->GetUnitIDElements("King");
		if (element != -1)
		{
			std::shared_ptr<King> p_king =
				std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
			kingPos = p_king->GetElementStock();
		}

		// �����_���ōU���^�C�v��I��
		attackSelect = ATTACK_RANDOM_MIN_MAX.a + (int)(rand() * (ATTACK_RANDOM_MIN_MAX.b - ATTACK_RANDOM_MIN_MAX.a + 1) / (1 + RAND_MAX));
		
		// �{�X�̏�Ԃŏ����̓��e��ς���(�ݒ�n)
		if (bossType == BossType::normal)// BossType���ʏ��ԂȂ�
		{
			// �ǂ̃��C�����U�����邩
			BossAttack::GetInstance()->Target();
			// carobj��ݒ�
			if (attackSelect == 0) // �c�Ȃ��
			{
				// ���f����Y����-90�x(�������������悤�ɐݒ�)
				carobj->rotation.y = -90.0f;
				// ���W�̐ݒ�
				// Tile���W���烏�[���h���W�ɕϊ�
				float targetPos = Common::ConvertTilePosition(BossAttack::GetInstance()->GetTargetTile());
				carobj->position = {targetPos,1.0f,50.0f};
			}
			else if (attackSelect == 1) // ���Ȃ��
			{
				// ���f����Y����180�x(�E�����Ɍ����悤�ɐݒ�)
				carobj->rotation.y = 180.0f;
				// ���W�̐ݒ�
				// Tile���W���烏�[���h���W�ɕϊ�
				float targetPos = Common::ConvertTilePosition(BossAttack::GetInstance()->GetTargetTile());
				carobj->position = { -50.0f,1.0f,targetPos };
			}
		}
		else if (bossType == BossType::strong)// BossType��������ԂȂ�
		{
			// �����_�����e�I�U���̏���
			if (attackSelect != 0)
			{
				// ���e�I�̍��W����
				BossAttack::GetInstance()->CreateMeteorPosition(attackSelect);
			}
		}
		// �{�X�𖳓G�ɂ���
		// Attack�\���̏�����
		BossAttack::GetInstance()->InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag()) // �A�^�b�N�^�C�}�[��3����1�ȉ��ɂȂ�����ʂ�(n��)
	{
		// �{�X�̏�Ԃŏ����̓��e��ς���(�����Ȃ��珀��)
		if (bossType == BossType::normal)
		{
			// �^�C���̕\����\���𐧌�
			BossAttack::GetInstance()->DispTileDeathControl(attackSelect,0);

			// �{�X�������܂�
			object3d->position.y += 0.2f;
			if (object3d->position.y >= 2.0f)
			{
				// ���G��Ԃ̕t�^
				//*invincibleFlag = true;
			}

			// ���W���V��(20.0f)�ɂ�����Œ艻
			if (object3d->position.y >= 20.0f)
			{
				object3d->position.y = 20.0f;
			}

			// �A�^�b�N�Z���N�g�ɂ���čU������\�肤���\��
			if (attackSelect == 0)
			{
				// �c3��\��
				BossAttack::GetInstance()->Vertical3Line("Display");
			}
			else
			{
				// ��3��\��
				BossAttack::GetInstance()->Horizontal3Line("Display");
			}
		}
		else if (bossType == BossType::strong)
		{
			// 1�_���e�I
			if (attackSelect == 0)
			{
				BossAttack::GetInstance()->OneMeteor3x3("Display",kingPos);
			}

			// �����_�����e�I�͕\���ƍU���������Ȃ̂ŁA�����ɂ͏����܂���
		}
		bossState = BossState::attack;
	}
}

void XIIlib::Boss::Attack()
{
	const int START_COUNT = 25; // �X�^�[�g�t���[��
	const float ACC = 0.7f; // �����l
	// �{�X�^�C�v���ƂɍU��������ω�
	switch (bossType)
	{
	case BossType::normal:
		BossAttack::GetInstance()->DispTileDeathControl(attackSelect, START_COUNT);
		if (attackSelect == 0)
		{
			// ���W�ɉ����l������
			carobj->position.z -= ACC;
			// �c3�U��
			BossAttack::GetInstance()->Vertical3Line("Attack");
		}
		else
		{
			// ���W�ɉ����l������
			carobj->position.x += ACC;
			// ��3�U��
			BossAttack::GetInstance()->Horizontal3Line("Attack");
		}
		break;
	case BossType::strong:
		if (attackSelect == 0)
		{
			// 1�_���e�I�U�� 3x3
			BossAttack::GetInstance()->OneMeteor3x3("Attack",kingPos);
		}
		else
		{
			// �����_�����e�I�U�� 1x1
			BossAttack::GetInstance()->RandomMeteor1x1Attack();
		}
		break;
	default:
		// �U���Ȃ�
		break;
	}

	// �{�X�̃|�W�V������߂�
	if (carobj->position.x >= 50.0f || carobj->position.z <= -50.0f)// �c���Ԃ��w�i����t�F�[�h�A�E�g����������
	{
		object3d->position.y -= 0.2f;
		// ���̍��W�ɖ߂�����
		if (object3d->position.y <= 1.0f)
		{
			// ���W��ݒ�
			object3d->position.y = 1.0f;
			// ���G��Ԃ̉���
			//*invincibleFlag = false;
		}
	}
	
	// �U���t���[���̉��Z
	attackFrameCnt++;
	// �ő�U���t���[�����U���t���[����������΍U���͏I��
	if (MAX_ATTACK_FRAME <= attackFrameCnt)
	{
		// �U�������S�ɏI������̂�false
		determinateMoveAction = false;
		// �U���t���[����0�N���A
		attackFrameCnt = 0;
		// boss�̏�Ԃ�ҋ@��Ԃɖ߂�
		bossState = BossState::wait;
	}
}

void XIIlib::Boss::Move()
{}

bool XIIlib::Boss::AttackAreaExists()
{
	return false;
}

void XIIlib::Boss::IniState()
{}

void XIIlib::Boss::CreateAttackArea()
{}

void XIIlib::Boss::ObjectUpdate()
{
	object3d->Update();
	object3d2->Update();
	carobj->Update();
}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	return false;
}

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
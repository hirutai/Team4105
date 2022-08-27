#include "Boss.h"
#include "BossHP.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include"../GameObject/Stone.h"
#include "../Tool/DebugText.h"
#include "../Tool/Easing.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>
#include <random>
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
	carobj->position = position;

	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();
	//�U���Q�[�W�̕b��
	SetAttackTimer(300,CountType::FRAME);
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
		bossType = BossType::strong;
		SetAttackTimer(180, CountType::FRAME);
		attackTimer->SetPosition(Math::Vector3(0.0f, 19.0f, 0.0f));
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
		// ��̍s��
		Action();

		// �^�C�}�[�̍X�V
		attackTimer->Timer();

		pos = object3d->position;
	}
	if (determinateMoveAction) {
		// �U������
		Attack();
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
		bossAttackSelect = bossAttackMin + (int)(rand() * (bossAttackMax - bossAttackMin + 1) / (1 + RAND_MAX));
		// �ǂ̃��C�����U�����邩
		BossAttack::GetInstance()->Target();

		// BossType��������ԂȂ�
		if (bossType == BossType::strong)
		{
			if (bossAttackSelect != 0)
			{
				BossAttack::GetInstance()->CreateMeteorPosition();
			}	
		}	
		// Attack�\���̏�����
		BossAttack::GetInstance()->InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag()) // �A�^�b�N�^�C�}�[��3����1�ȉ��ɂȂ�����ʂ�(n��)
	{
		BossAttack::GetInstance()->DispTileDeathControl(bossAttackSelect);
		if (bossType == BossType::normal)
		{
			object3d->position.y += 0.2f;
			object3d2->position.y += 0.2f;
			if (object3d->position.y >= 20.0f)
			{
				object3d->position.y = 20.0f;
				object3d2->position.y = 20.0f;
			}
			//carobj->position.y +=0.1f;
			if (bossAttackSelect == 0)
			{
				// �c
				BossAttack::GetInstance()->Vertical3LineDisplay();
			}
			else
			{
				// ��
				BossAttack::GetInstance()->Horizontal3LineDisplay();
			}
		}
		else if (bossType == BossType::strong)
		{
			if (bossAttackSelect == 0)
			{
				BossAttack::GetInstance()->OneMeteor3x3Display(kingPos);
			}
			else
			{
				// BossAttack::GetInstance()->RandomMeteor1x1Display();
			}
		}
		bossState = BossState::attack;
	}

	// ���ډ���ꂽ���̏���
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		BossAttack::GetInstance()->KnockBackAttack(element_stock);
	}	
}

void XIIlib::Boss::Attack()
{
	switch (bossType)
	{
	case BossType::normal:
		if (bossAttackSelect == 0)
		{
			// �c�U��
			BossAttack::GetInstance()->Vertical3LineAttack();
		}
		else
		{
			// ���U��
			BossAttack::GetInstance()->Horizontal3LineAttack();
		}
		break;
	case BossType::strong:
		if (bossAttackSelect == 0)
		{
			// 1�_���e�I�U�� 3x3
			BossAttack::GetInstance()->OneMeteor3x3Attack(kingPos);
		}
		else
		{
			// (��)�����_�����e�I�U�� 1x1
			BossAttack::GetInstance()->RandomMeteor1x1Attack();
		}
		break;
	default:
		// �U���Ȃ�
		break;
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

void XIIlib::Boss::AttackAreaDraw()
{}

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
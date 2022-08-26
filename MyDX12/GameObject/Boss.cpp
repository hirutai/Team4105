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
	delete attackTimer;
	delete object3d;
	delete object3d2;
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
	BossHP::GetInstance()->Initialize();
	// �N���X�l�[���擾
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();

	// �傫���ƍ��W�̒萔
	const Math::Vector3 scale = { 1.5f,1.5f,1.5f };
	const Math::Vector3 position = { Common::ConvertTilePosition(element_stock.a) - 2.7f,1.0f, Common::ConvertTilePosition(element_stock.b) };

	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS));
	object3d->scale = scale;
	object3d->position = position;

	object3d2 = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BOSS2));
	object3d2->scale = scale;
	object3d2->position = position;
	
	// Audio�̏�����
	audio_ = UnitManager::GetInstance()->GetAudio();

	//�U���Q�[�W�̕b��
	SetAttackTimer(300,CountType::FRAME);

	nextPoint = { 0,0 };
	//isDrawTimer = true;
	fallFlag = true;
	BossAttack::GetInstance()->InitAttackDisplay();
}

void XIIlib::Boss::Update()
{
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
	_hit_point = BossHP::GetInstance()->GetBossHP();

	if (_hit_point <= BossHP::GetInstance()->GetDefaultBossHP() / 2 && bossType == BossType::normal && bossState == BossState::wait)
	{
		bossType = BossType::strong;
		SetAttackTimer(180, CountType::FRAME);
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

	object3d->Update();
	object3d2->Update();
	// ���W�ݒ�
	attackTimer->SetPosition(object3d->position + Math::Vector3(0.0f, 18.0f, 0.0f));
}

void XIIlib::Boss::Draw()
{
	if (switchingCount >= 1)
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
	}
}

void XIIlib::Boss::Action()
{
	isAttack = true;
	preElement_stock = kingPos;

	Move();
	
	if (attackTimer->SizeZeroFlag()) // �A�^�b�N�^�C�}�[��0(�ȉ�)�ɂȂ�����ʂ�
	{
		bossState = BossState::wait;
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
		BossAttack::GetInstance()->DispTileDeathControl();
		if (bossType == BossType::normal)
		{
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
				BossAttack::GetInstance()->RandomMeteor1x1Display();
			}
		}
		bossState = BossState::attack;
	}

	// �{�X�𒼐ډ��������̏���
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		BossAttack::GetInstance()->KnockBackAttack(element_stock);
	}	
}

void XIIlib::Boss::Attack()
{
	Math::Point2 temp = element_stock;
	switch (bossType)
	{
	case BossType::normal:
		if (bossAttackSelect == 0)
		{
			BossAttack::GetInstance()->Vertical3LineAttack();
		}
		else
		{
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
	}
}

void XIIlib::Boss::Move()
{}

bool XIIlib::Boss::AttackAreaExists()
{}

void XIIlib::Boss::AttackAreaDraw()
{}

void XIIlib::Boss::IniState()
{}

void XIIlib::Boss::CreateAttackArea()
{}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{}

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
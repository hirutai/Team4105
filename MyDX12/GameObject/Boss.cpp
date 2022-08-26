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
	InitAttackDisplay();
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
	
	if (attackTimer->SizeZeroFlag())
	{
		bossState = BossState::wait;
		switching = true;
		// Attack�\���̏�����
		InitAttackDisplay();
		// �U�����J�n����̂�true
		determinateMoveAction = true;
		audio_->PlaySE("swing.wav");
	}
	else if (attackTimer->SizeThirdFlag())
	{
		int element = UnitManager::GetInstance()->GetUnitIDElements("King");
		if (element != -1)
		{
			std::shared_ptr<King> p_king =
				std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
			kingPos = p_king->GetElementStock();
		}
		Target();
		if (bossType == BossType::strong)
		{
			if (bossAttackSelect != 0)
			{
				srand(time(NULL));
				int count = 0;
				int breakCnt = 0;
				for (int i = 0; i < METEORS_MAX; i++)
				{
					numbersA[i] = -1;
					numbersB[i] = -1;
				}
				//���e�I�̍��W����
				while (1)
				{
					breakCnt++;
					if (breakCnt >= 150)break;//�o�O�����܂ꂽ�Ƃ��p�̋����I��
					//���̃����_���ȍ��W������
					Math::Point2 numbers;
					numbers.a = meteorAMin + (int)(rand() * (meteorAMax - meteorAMin + 1) / (1 + RAND_MAX));
					numbers.b = meteorBMin + (int)(rand() * (meteorBMax - meteorBMin + 1) / (1 + RAND_MAX));

					if (UnitManager::GetInstance()->AllOnUnit(numbers))continue;//���̍��W�ɒN�������邩
					if (CheckMeteorArea(numbers))continue;//���̃��e�I�̍��W�Ɣ���Ă��邩
					//���Ȃ���������W����
					numbersA[count] = numbers.a;
					numbersB[count] = numbers.b;
					count++;

					if (count >= METEORS_MAX)break;
				}
			}	
		}	
		// Attack�\���̏�����
		InitAttackDisplay();
	}
	else if (attackTimer->SizeThirdBelowFlag())
	{
		//�F��h��
		if (count % DISPLAY_FRAME == 0)
		{
			if (bossAttackSelect == 0) {
				tileDeth[abs(tileNum - (MAX_TILE - 1))] = true;
			}
			else
			{
				tileDeth[tileNum] = true;
			}
			tileNum++;
		}
		count++;
		if (bossType == BossType::normal)
		{
			if (bossAttackSelect == 0)
			{
				// �c
				for (int i = MAX_TILE - 1; i >= 0; --i)
				{
					if (tileDeth[i])
					{
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::ENEMY);
					}
				}
			}
			else
			{

				// ��
				for (int j = 0; j < MAX_TILE; ++j)
				{
					if (tileDeth[j])
					{
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::ENEMY);
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::ENEMY);
					}
				}
			}
		}
		else if (bossType == BossType::strong)
		{
			if (bossAttackSelect == 0)
			{
				Math::Point2 anchorLUpos = { kingPos.a - 1, kingPos.b + 1 };//����
				for (int i = 0; i < 3; i++)//3*3�̓_�U��
				{
					for (int j = 0; j < 3; j++)
					{
						if (Common::GetExceptionPoint(anchorLUpos.a + j) || Common::GetExceptionPoint(anchorLUpos.b - i))continue;
						
						UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(anchorLUpos.a + j, anchorLUpos.b - i), (int)_PositionType::ENEMY);
					}
				}
			}
			//else
			//{
			//	for (int i = 0; i < METEORS_MAX; i++)//���e�I
			//	{	
			//		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[i], numbersB[i]), (int)_PositionType::ENEMY);					
			//	}
			//}
		}
		bossState = BossState::attack;
	}

	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		//�^�C���\��
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::ENEMY);
		//����������e��
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 1, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - 3, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + 2, element_stock.b - 1), (int)_PositionType::BOSS_KNOCKBACK);
	}	
}

void XIIlib::Boss::Target()
{
	int BossjMin = bossMin;
	int BossjMax = bossMax;
	bossTileRand = bossMin + (int)(rand() * (bossMax - bossMin + 1) / (1 + RAND_MAX));

	int BossAttackMin = bossAttackMin;
	int BossAttackMax = bossAttackMax;
	bossAttackSelect = bossAttackMin + (int)(rand() * (bossAttackMax - bossAttackMin + 1) / (1 + RAND_MAX));

	std::vector<int> v(6);
	std::iota(v.begin(), v.end(), 0);
	// �V���b�t��
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v.begin(), v.end(), engine);

	bossTileRand = v[bossTileRand] + 1;
}

void XIIlib::Boss::Attack()
{
	Math::Point2 temp = element_stock;
	if (bossType == BossType::normal)
	{
		if (bossAttackSelect == 0)
		{
			for (int i = 0; i < MAX_TILE; i++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand + 1, i), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand - 1, i), (int)_PositionType::BOSS);
			}
		}
		else
		{
			for (int j = 0; j < MAX_TILE; j++)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand + 1), (int)_PositionType::BOSS);
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(j, bossTileRand - 1), (int)_PositionType::BOSS);
			}
		}
		attackFrameCnt = 300;
	}
	else if (bossType == BossType::strong)
	{
		if (bossAttackSelect == 0)
		{
			Math::Point2 anchorLUpos = { kingPos.a - 1, kingPos.b + 1 };//����
			for (int i = 0; i < 3; i++)//3*3�̓_�U��
			{
				for (int j = 0; j < 3; j++)
				{
					if (Common::GetExceptionPoint(anchorLUpos.a + j) || Common::GetExceptionPoint(anchorLUpos.b - i))continue;
					UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(anchorLUpos.a + j, anchorLUpos.b - i), (int)_PositionType::BOSS);
				}
			}
			attackFrameCnt = 300;
		}
		else
		{
			meteorsCount++;
			//���e�I1�`��
			if (meteorsCount >= -20 && meteorsCount <= 0)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[0], numbersB[0]), (int)_PositionType::ENEMY);
			}
			//���e�I2
			if (meteorsCount >= 0 && meteorsCount <= 20)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[1], numbersB[1]), (int)_PositionType::ENEMY);
			}
			//���e�I3
			if (meteorsCount >= 20 && meteorsCount <= 40)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[2], numbersB[2]), (int)_PositionType::ENEMY);
			}
			//���e�I4
			if (meteorsCount >= 40 && meteorsCount <= 60)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[3], numbersB[3]), (int)_PositionType::ENEMY);
			}
			//���e�I5
			if (meteorsCount >= 60 && meteorsCount <= 80)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[4], numbersB[4]), (int)_PositionType::ENEMY);
			}
			//���e�I6
			if (meteorsCount >= 80 && meteorsCount <= 100)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[5], numbersB[5]), (int)_PositionType::ENEMY);
			}
			//���e�I7
			if (meteorsCount >= 100 && meteorsCount <= 120)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[6], numbersB[6]), (int)_PositionType::ENEMY);
			}
			//���e�I8
			if (meteorsCount >= 120 && meteorsCount <= 140)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[7], numbersB[7]), (int)_PositionType::ENEMY);
			}
			//���e�I9
			if (meteorsCount >= 140 && meteorsCount <= 160)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[8], numbersB[8]), (int)_PositionType::ENEMY);
			}
			//���e�I10
			if (meteorsCount >= 160 && meteorsCount <= 180)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[9], numbersB[9]), (int)_PositionType::ENEMY);
			}
			//���e�I11
			if (meteorsCount >= 180 && meteorsCount <= 200)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[10], numbersB[10]), (int)_PositionType::ENEMY);
			}
			//���e�I12
			if (meteorsCount >= 200 && meteorsCount <= 220)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[11], numbersB[11]), (int)_PositionType::ENEMY);
			}
			//���e�I13
			if (meteorsCount >= 220 && meteorsCount <= 240)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[12], numbersB[12]), (int)_PositionType::ENEMY);
			}
			//���e�I14
			if (meteorsCount >= 240 && meteorsCount <= 260)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[13], numbersB[13]), (int)_PositionType::ENEMY);
			}
			//���e�I15
			if (meteorsCount >= 260 && meteorsCount <= 280)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[14], numbersB[14]), (int)_PositionType::ENEMY);
			}

			//�撣�������ǂނ�ł���
			//meteorsSpaceCount++;
			//if (meteorsSpaceCount >= 20)
			//{
			//	meteorsFCount++;
			//	//���e�I�̍U������
			//	if (meteorsFCount >= 0 && meteorsFCount <= 40 && meteorsCount < METEORS_MAX)
			//	{
			//		if (meteorsFCount >= 0 && meteorsFCount <= 20)
			//		{
			//			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[meteorsIndex + meteorsCount], numbersB[meteorsIndex + meteorsCount]), (int)_PositionType::ENEMY);
			//		}
			//		if (meteorsFCount >= 20 && meteorsFCount <= 40)
			//		{
			//			UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[meteorsIndex + meteorsCount], numbersB[meteorsIndex + meteorsCount]), (int)_PositionType::BOSS);
			//		}
			//		meteorsCount++;
			//	}
			//}
			//
			//if (meteorsFCount >= 40)
			//{
			//	meteorsFCount = 0;
			//	meteorsSpaceCount = 0;
			//}

			//���e�I1�̍U������
			if (meteorsCount >= 0 && meteorsCount <= 20)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[0], numbersB[0]), (int)_PositionType::BOSS);
			}
			//���e�I2
			if (meteorsCount >= 20 && meteorsCount <= 40)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[1], numbersB[1]), (int)_PositionType::BOSS);
			}
			//���e�I3
			if (meteorsCount >= 40 && meteorsCount <= 60)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[2], numbersB[2]), (int)_PositionType::BOSS);
			}
			//���e�I4
			if (meteorsCount >= 60 && meteorsCount <= 80)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[3], numbersB[3]), (int)_PositionType::BOSS);
			}
			//���e�I5
			if (meteorsCount >= 80 && meteorsCount <= 100)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[4], numbersB[4]), (int)_PositionType::BOSS);
			}
			//���e�I6
			if (meteorsCount >= 100 && meteorsCount <= 120)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[5], numbersB[5]), (int)_PositionType::BOSS);
			}
			//���e�I7
			if (meteorsCount >= 120 && meteorsCount <= 140)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[6], numbersB[6]), (int)_PositionType::BOSS);
			}
			//���e�I8
			if (meteorsCount >= 140 && meteorsCount <= 160)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[7], numbersB[7]), (int)_PositionType::BOSS);
			}
			//���e�I9
			if (meteorsCount >= 160 && meteorsCount <= 180)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[8], numbersB[8]), (int)_PositionType::BOSS);
			}
			//���e�I10
			if (meteorsCount >= 180 && meteorsCount <= 200)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[9], numbersB[9]), (int)_PositionType::BOSS);
			}
			//���e�I11
			if (meteorsCount >= 200 && meteorsCount <= 220)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[10], numbersB[10]), (int)_PositionType::BOSS);
			}
			//���e�I12
			if (meteorsCount >= 220 && meteorsCount <= 240)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[11], numbersB[11]), (int)_PositionType::BOSS);
			}
			//���e�I13
			if (meteorsCount >= 240 && meteorsCount <= 260)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[12], numbersB[12]), (int)_PositionType::BOSS);
			}
			//���e�I14
			if (meteorsCount >= 260 && meteorsCount <= 280)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[13], numbersB[13]), (int)_PositionType::BOSS);
			}
			//���e�I15
			if (meteorsCount >= 280 && meteorsCount <= 300)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(numbersA[14], numbersB[14]), (int)_PositionType::BOSS);
			}
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
		//���e�I�̐�-20��(�`��̊֌W��-�ɂȂ��Ă܂�)
		meteorsCount = -20;
	}
}

void XIIlib::Boss::Move()
{
	
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
	Math::Point2 dif = kingPos - element_stock;
	// x��z�̐�Βl���ꏏ��������U���͈͂ɂ���͈�
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Boss::AttackAreaDraw()
{
	
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

void XIIlib::Boss::InitAttackDisplay()
{
	if (count >= DISPLAY_FRAME * MAX_TILE)
	{
		count = 0;
		tileNum = 0;
		for (int i = 0; i < MAX_TILE; i++)
		{
			tileDeth[i] = false;
		}
	}
}

bool XIIlib::Boss::CheckMeteorArea(Math::Point2 meteorPos)
{
	for (int i = 0; i < METEORS_MAX; i++)
	{
		//meteorPos��METEORS_MAX�̉񐔒����ł��ꏏ�ɂȂ��true��Ԃ�
		if (meteorPos.a == numbersA[i] && meteorPos.b == numbersB[i])return true;
	}
	return false;
}

void XIIlib::Boss::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
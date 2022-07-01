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
#include <algorithm>
#include <iostream>
#include <numeric>
#include <iterator>
#include <random>

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
	// �^�C�}�[�̍X�V
	attackTimer->Timer();

	Action();

	object3d->Update();
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	// ���W�ݒ�
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Boss::Action()
{
	isAttack = true;
	preElement_stock = kingPos;

	Move();

	if (attackTimer->SizeZeroFlag())
	{
		Attack();
		audio_->PlaySE("swing.wav");
	}

	//if (attackTimer->SizeZeroFlag())
	//{
	//	notAttackflag = TRUE;
	//}
	//isAttack = false;
	
	////�ړ��͈͂̐F�t��
	//AttackAreaDraw();

	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		_hit_point--;
	}
}

void XIIlib::Boss::Attack()
{
	int BossjMin = bossMin;
	int BossjMax = bossMax;
	bossTileRand = bossMin + (int)(rand() * (bossMax - bossMin + 1) / (1 + RAND_MAX));

	std::vector<int> v(8);
	std::iota(v.begin(), v.end(), 0);
	// �V���b�t��
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::shuffle(v.begin(), v.end(), engine);

	bossTileRand = v[bossTileRand];
	////�������ɋ���邩
	//for (int i = 0; i <= 7; i++)
	//{
	//	AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b));
	//}
	////�E�����ɋ���邩
	//for (int i = 0; i <= 7; i++)
	//{
	//	AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b));
	//}
	//�������ɋ���邩
	for (int i = 0; i <= 7; i++)
	{
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(bossTileRand, i), (int)type);
	}
	//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(1, 1), (int)type);
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

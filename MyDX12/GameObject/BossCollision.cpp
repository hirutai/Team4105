#include "BossCollision.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "BossHP.h"

XIIlib::BossCollision::BossCollision()
{
	// �e�X�e�[�^�X�̏�����
	_cost = 0;
	_hit_point = BossHP::GetInstance()->GetBossHP();
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::BossCollision::~BossCollision()
{
	delete object3d;
}

std::shared_ptr<XIIlib::BossCollision> XIIlib::BossCollision::Create(int point_x, int point_z)
{
	std::shared_ptr<BossCollision> rook = std::make_shared<BossCollision>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::BossCollision::Initialize()
{
	// �N���X�l�[���擾
	const type_info& id = typeid(BossCollision);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });

}

void XIIlib::BossCollision::Update()
{
	_hit_point = BossHP::GetInstance()->GetBossHP();
	// ��̍s��
	Action();
	// �ʒu���W�̍X�V
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
	}

	object3d->Update();
}

void XIIlib::BossCollision::Action()
{
}

void XIIlib::BossCollision::Attack()
{
}

void XIIlib::BossCollision::Move()
{
}

bool XIIlib::BossCollision::AttackAreaExists()
{
	return false;
}

void XIIlib::BossCollision::AttackAreaDraw()
{
}

void XIIlib::BossCollision::IniState()
{
}

void XIIlib::BossCollision::CreateAttackArea()
{
}

bool XIIlib::BossCollision::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	return false;
}

void XIIlib::BossCollision::SetHitDamage(int attackPoint)
{
	_hit_point -= attackPoint;
	BossHP::GetInstance()->SetBossHP(_hit_point);
}
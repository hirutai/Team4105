#include "BossCollision.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "BossHP.h"

XIIlib::BossCollision::BossCollision()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = BossHP::GetInstance()->GetBossHP();
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::BossCollision::~BossCollision()
{
	if (object3d != nullptr) {
		delete object3d;
	}
}

std::shared_ptr<XIIlib::BossCollision> XIIlib::BossCollision::Create(int point_x, int point_z)
{
	std::shared_ptr<BossCollision> boss = std::make_shared<BossCollision>();
	boss.get()->SetElementStock(point_x, point_z);
	boss.get()->Initialize();

	return std::move(boss);
}

void XIIlib::BossCollision::Initialize()
{
	// クラスネーム取得
	const type_info& id = typeid(BossCollision);
	std::string path = id.name();
	_hit_point = BossHP::GetInstance()->GetBossHP();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	/*object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });*/

}

void XIIlib::BossCollision::Update()
{
	_hit_point = BossHP::GetInstance()->GetBossHP();
	// 駒の行動
	Action();
	// 位置座標の更新
	//object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		BossHP::GetInstance()->Damage();
		//タイル表示
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(1, 7), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(6, 7), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(1, 6), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(2, 6), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(3, 6), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(4, 6), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(5, 6), (int)_PositionType::ENEMY);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(6, 6), (int)_PositionType::ENEMY);
		//密着したら弾く
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(1, 7), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(6, 7), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(1, 6), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(2, 6), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(3, 6), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(4, 6), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(5, 6), (int)_PositionType::BOSS_KNOCKBACK);
		UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(6, 6), (int)_PositionType::BOSS_KNOCKBACK);
	}

	//object3d->Update();
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

void XIIlib::BossCollision::Draw() 
{

}
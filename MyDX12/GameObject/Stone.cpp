#include "Stone.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"

XIIlib::Stone::Stone()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Stone::~Stone()
{
	delete object3d;
}

std::shared_ptr<XIIlib::Stone> XIIlib::Stone::Create(int point_x, int point_z)
{
	std::shared_ptr<Stone> rook = std::make_shared<Stone>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Stone::Initialize()
{
	// 特になし
	_hit_point = 2;
	// クラスネーム取得
	const type_info& id = typeid(Stone);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});
}

void XIIlib::Stone::Update()
{
	// 駒の行動
	Action();
	// 位置座標の更新
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	object3d->Update();
}

void XIIlib::Stone::Action()
{
}

void XIIlib::Stone::Attack()
{
}

void XIIlib::Stone::Move()
{
}

bool XIIlib::Stone::AttackAreaExists()
{
	return false;
}

void XIIlib::Stone::AttackAreaDraw()
{
}

void XIIlib::Stone::IniState()
{
}

void XIIlib::Stone::CreateAttackArea()
{
}

bool XIIlib::Stone::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	return false;
}

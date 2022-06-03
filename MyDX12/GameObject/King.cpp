#include "King.h"
#include "../3D/CollCapsule.h"
#include "../Input/KeyInput.h"
#include "Common.h"
#include "UnitManager.h"
#include "Rook.h"
#include "../Tool/Messenger.h"

XIIlib::King::King()
{
	// モデルの初期化
	collCapsule = CollisionCapsule::Create({ 0,1,0 }, {0,-1,0},0.5f,16);
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::King::~King()
{
	delete collCapsule;
}

std::shared_ptr<XIIlib::King> XIIlib::King::CreateKing(int point_x, int point_z)
{
	std::shared_ptr<King> pawn = std::make_shared<King>();
	pawn.get()->SetStartElement(point_x,point_z);
	pawn.get()->Initialize();

	return std::move(pawn);
}

void XIIlib::King::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(King);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::MINE;

	CreateAttackArea();

	now_attack.reserve(3);
}

void XIIlib::King::Update()
{
	
	Action();

	if (type_attack != AREA::NONE) {
		std::vector<Math::Point2> container = attack_area[(int)type_attack];
		int add_element = 0;
		Math::Vector2 at[3];
		for (int i = 0; i < 3; i++) {
			container[i] += element_stock;
			if ((container[i].a < 8 && container[i].a >= 0) && (container[i].b < 8 && container[i].b >= 0)) {
				UnitManager::GetInstance()->ChangeAttackValidTile(container[i],(int)type);
				Math::Point2 vec_point = container[i] - element_stock;
				UnitManager::GetInstance()->SetBackVector(container[i], vec_point * 2);
			}
		}
	}

	if (is_hit) {
		collCapsule->SetColor(1, 0, 0, 1);
		damage_counter++;

		if (damage_counter > 20) {
			is_hit = 0;
			damage_counter = 0;
			collCapsule->SetColor(1, 1, 1, 1);
		}
	}

	// 位置座標の更新
	collCapsule->SetPosition(
		Common::ConvertTilePosition(element_stock.a),1.0f,
		Common::ConvertTilePosition(element_stock.b));

	collCapsule->Update();
}

void XIIlib::King::Draw()
{
	collCapsule->Draw();
}

void XIIlib::King::SetStartElement(int x, int z)
{
	element_stock = Math::Point2(x, z);
}

void XIIlib::King::Move()
{
	Math::Point2 next_state = element_stock;

	Math::Point2 move_vec[4] = {
		{0, 1},		// W|0
		{0, -1},	// S|1
		{-1, 0},	// A|2
		{1, 0}		// D|3
	};

	if (KeyInput::GetInstance()->Trigger(DIK_W)) {

		next_state += move_vec[0];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			element_stock = next_state;
		}
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_S)) {
		next_state += move_vec[1];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			element_stock = next_state;
		}
	}

	if (KeyInput::GetInstance()->Trigger(DIK_A)) {
		next_state += move_vec[2];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			element_stock = next_state;
		}
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_D)) {
		next_state += move_vec[3];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			element_stock = next_state;
		}
	}

	element_stock = Common::OffsetTilePosition2(element_stock);
}

void XIIlib::King::Attack()
{
	if (KeyInput::GetInstance()->Trigger(DIK_UP)) {
		type_attack = AREA::UP;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_UP) && KeyInput::GetInstance()->Trigger(DIK_LEFT)) {
		type_attack = AREA::UP_LEFT;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_LEFT)) {
		type_attack = AREA::LEFT;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_LEFT) && KeyInput::GetInstance()->Trigger(DIK_DOWN)) {
		type_attack = AREA::LEFT_DOWN;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_DOWN)) {
		type_attack = AREA::DOWN;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_DOWN) && KeyInput::GetInstance()->Trigger(DIK_RIGHT)) {
		type_attack = AREA::DOWN_RIGHT;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_RIGHT)) {
		type_attack = AREA::RIGHT;
	}
	else if (KeyInput::GetInstance()->Trigger(DIK_RIGHT) && KeyInput::GetInstance()->Trigger(DIK_UP)) {
		type_attack = AREA::RIGHT_UP;
	}
	else {
		type_attack = AREA::NONE;
		now_attack.clear();
	}
}

void XIIlib::King::Action()
{
	Attack();
	Move();
}

bool XIIlib::King::AttackAreaExists()
{
	return false;
}

void XIIlib::King::IniState()
{
}

void XIIlib::King::Hit(int attackPoint)
{
	int _damage = _defense_point - attackPoint;
	if (_damage < 0) {
		_hit_point += _damage;
		is_hit = 1;
	}
}

void XIIlib::King::SetElementStock(int x, int z)
{
	element_stock.a = x; element_stock.b = z;
}

void XIIlib::King::SetTypePositioning(_PositionType changeType)
{
	type = changeType;
}

void XIIlib::King::CreateAttackArea()
{
	const int size_data = 8;
	attack_area.resize(size_data);
	for (int i = 0; i < size_data; i++) {
		attack_area[i].reserve(3);
	}

	attack_area[(int)AREA::UP].push_back(Math::Point2(-1, 1));
	attack_area[(int)AREA::UP].push_back(Math::Point2( 0, 1));
	attack_area[(int)AREA::UP].push_back(Math::Point2( 1, 1));

	attack_area[(int)AREA::UP_LEFT].push_back(Math::Point2(-1, 0));
	attack_area[(int)AREA::UP_LEFT].push_back(Math::Point2(-1, 1));
	attack_area[(int)AREA::UP_LEFT].push_back(Math::Point2( 0, 1));

	attack_area[(int)AREA::LEFT].push_back(Math::Point2(-1, -1));
	attack_area[(int)AREA::LEFT].push_back(Math::Point2(-1,  0));
	attack_area[(int)AREA::LEFT].push_back(Math::Point2(-1,  1));

	attack_area[(int)AREA::LEFT_DOWN].push_back(Math::Point2( 0, -1));
	attack_area[(int)AREA::LEFT_DOWN].push_back(Math::Point2(-1, -1));
	attack_area[(int)AREA::LEFT_DOWN].push_back(Math::Point2(-1,  0));

	attack_area[(int)AREA::DOWN].push_back(Math::Point2( 1, -1));
	attack_area[(int)AREA::DOWN].push_back(Math::Point2( 0, -1));
	attack_area[(int)AREA::DOWN].push_back(Math::Point2(-1, -1));

	attack_area[(int)AREA::DOWN_RIGHT].push_back(Math::Point2(1,  0));
	attack_area[(int)AREA::DOWN_RIGHT].push_back(Math::Point2(1, -1));
	attack_area[(int)AREA::DOWN_RIGHT].push_back(Math::Point2(0, -1));

	attack_area[(int)AREA::RIGHT].push_back(Math::Point2(1,  1));
	attack_area[(int)AREA::RIGHT].push_back(Math::Point2(1,  0));
	attack_area[(int)AREA::RIGHT].push_back(Math::Point2(1, -1));

	attack_area[(int)AREA::RIGHT_UP].push_back(Math::Point2(0,  1));
	attack_area[(int)AREA::RIGHT_UP].push_back(Math::Point2(1,  1));
	attack_area[(int)AREA::RIGHT_UP].push_back(Math::Point2(1,  0));
}

bool XIIlib::King::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	return false;
}

#include "Tile.h"
#include "../3D/CollBox.h"

XIIlib::Tile* XIIlib::Tile::Create(float _x, float _z)
{
	Tile* p_tile = new Tile();
	if (p_tile == nullptr) {
		return nullptr;
	}

	// 初期化
	p_tile->Initialize(_x,_z);

	return p_tile;
}

XIIlib::Tile::Tile(){
}

XIIlib::Tile::~Tile(){
	delete p_box;
}

void XIIlib::Tile::Initialize(float _x, float _z)
{
	const Math::Vector3 startPoint = { 0.5f,0.5f,0.5f };
	const Math::Vector3 endPoint = startPoint * -1.0f;

	p_box = CollisionBox::Create(startPoint,endPoint);
	p_box->SetPosition(_x,0,_z);
}

void XIIlib::Tile::Update()
{
	on_tile = false;

	p_box->SetColor(1,1,1,1);
	// フラグがオンで10f赤に
	if (!is_attack_player && !is_attack_enemy && !is_move_point)return;

	if (is_attack_player)p_box->SetColor(1, 0, 0, 1);
	if (is_attack_enemy)p_box->SetColor(1, 0, 0, 1);
	if (is_move_point)p_box->SetColor(0.6f, 0.4f, 0, 1);

	if (is_attack_player && is_attack_enemy)p_box->SetColor(1,0,1,1);

	time_valid++;
	if (time_valid > 10) {
		is_attack_player = false;
		is_attack_enemy = false;
		is_move_point = false;
		vec_back = Math::Point2();
		time_valid = 0;
	}

	// BoxのUpdateはDrawの内部に存在するためここには記述しない。
}

void XIIlib::Tile::Draw()
{
	p_box->Draw();
}

void XIIlib::Tile::SetPlayerAttackValid()
{
	is_attack_player = true;
}

void XIIlib::Tile::SetEnemyAttackValid()
{
	is_attack_enemy = true;
}

void XIIlib::Tile::SetMovePointValid()
{
	is_move_point = true;
}

void XIIlib::Tile::SetBackVector(const Math::Point2& point)
{
	vec_back = point;
}

void XIIlib::Tile::SetOnTile()
{
	on_tile = true;
}

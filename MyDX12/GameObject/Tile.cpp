#include "Tile.h"
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include "../GameScene/SceneState.h"
#include "ModelLoader.h"

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
	delete scullTex;
	delete warningTex;

	delete tile;
}

void XIIlib::Tile::Initialize(float _x, float _z)
{
	// オブジェクトの初期化
	tile = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_TILE));
	tile->position = { _x,0,_z };
	tile->scale = Math::Vector3(4,1,4);
	warningTex = BillObj::Create(tile->position + Math::Vector3(0, 0.4f, 0), "warningTex.png", true);
	warningTex->SetRotation(90.0f, 0, 0);
	warningTex->SetSize({ 4,4 });
	scullTex = BillObj::Create(tile->position + Math::Vector3(0, 0.4f,0),"crashTex.png", true);
	scullTex->SetRotation(90.0f, 0, 0);
	scullTex->SetSize({ 4,4 });
}

void XIIlib::Tile::AreaPlayer()
{
	// 判定がオンで無ければ即返す
	if (!is_attack_player)return;

	// 判定用の色に変える
	if (is_attack_player)tile->color = { 1,0,0 };

	// 時間を進める
	time_valid_player++;

	// 時間になったら初期化
	if (time_valid_player > 10) {
		is_attack_player = false;
		vec_back = Math::Point2();
		time_valid_player = 0;
	}
}

void XIIlib::Tile::AreaEnemy()
{
	// 判定がオンで無ければ即返す
	if (!is_attack_enemy)return;

	// 判定用の色に変える
	if (is_attack_enemy)tile->color = { 1,0,0 };

	// 時間を進める
	time_valid_enemy++;

	// 時間になったら初期化
	if (time_valid_enemy > 10) {
		is_attack_enemy = false;
		time_valid_enemy = 0;
	}
}

void XIIlib::Tile::AreaMovePoint()
{
	// 判定がオンで無ければ即返す
	if (!is_move_point)return;

	// 判定用の色に変える
	if (is_move_point)tile->color = { 0, 0.3f, 0.7f };

	// 時間を進める
	time_valid_move_point++;

	// 時間になったら初期化
	if (time_valid_move_point > 10) {
		is_move_point = false;
		time_valid_move_point = 0;
	}
}

void XIIlib::Tile::AreaAttackPoint()
{
	// 判定がオンで無ければ即返す
	if (!is_attack_point)return;

	// 判定用の色に変える
	if (is_attack_point)tile->color = { 0, 0, 0 };

	// 時間を進める
	time_valid_attack_point++;

	// 時間になったら初期化
	if (time_valid_attack_point > 10) {
		is_attack_point = false;
		time_valid_attack_point = 0;
	}
}

void XIIlib::Tile::AreaAttackBoss()
{
	// 判定がオンで無ければ即返す
	if (!is_attack_boss)return;

	// 判定用の色に変える
	if (is_attack_boss)tile->color = { 1,0,0 };

	// 時間を進める
	time_valid_attack_boss++;

	// 時間になったら初期化
	if (time_valid_attack_boss > 10) {
		is_attack_boss = false;
		time_valid_attack_boss = 0;
	}
}
void XIIlib::Tile::AreaKnockBackBoss()
{
	// 判定がオンで無ければ即返す
	if (!is_knockBack_boss)return;

	// 判定用の色に変える
	if (is_knockBack_boss)tile->color = { 1,0,0 };

	// 時間を進める
	time_valid_attack_boss++;

	// 時間になったら初期化
	if (time_valid_attack_boss > 10) {
		is_knockBack_boss = false;
		time_valid_attack_boss = 0;
	}
}

void XIIlib::Tile::Update()
{
	tile->Update();

	on_tile = false;

	tile->color = { red,green,blue};

	if (SceneState::GetPhase() == Phase::CameraDirecting)return;

	AreaPlayer();
	AreaEnemy();
	AreaMovePoint();
	AreaAttackPoint();
	AreaAttackBoss();
	AreaKnockBackBoss();
}

void XIIlib::Tile::Draw()
{
	tile->Draw();
}

void XIIlib::Tile::DrawBillObj()
{
	if (is_attack_enemy) {
		warningTex->Draw();
	}

	if (is_attack_point) {
		scullTex->Draw();
	}
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

void XIIlib::Tile::SetAttackPointValid()
{
	is_attack_point = true;
}

void XIIlib::Tile::SetBossAttackValid()
{
	is_attack_boss = true;
}

void XIIlib::Tile::SetBossKnockBackValid()
{
	is_knockBack_boss = true;
}

void XIIlib::Tile::SetBackVector(const Math::Point2& point)
{
	vec_back = point;
}

void XIIlib::Tile::SetOnTile()
{
	on_tile = true;
}

void XIIlib::Tile::Flat()
{
	on_tile = false;

	is_attack_player = false;
	is_attack_enemy = false;
	is_attack_boss = false;
	is_move_point = false;
	is_attack_point = false;
	is_knockBack_boss = false;
}

void XIIlib::Tile::SetBaseColor(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}

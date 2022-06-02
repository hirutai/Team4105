#include "BoardMap.h"
#include "../Struct/Math.h"
#include "Tile.h"
#include "../3D/CollBox.h"

XIIlib::BoardMap* XIIlib::BoardMap::Create()
{
	BoardMap* p_map_board = new BoardMap();
	if (p_map_board == nullptr) {
		return nullptr;
	}

	// 初期化
	p_map_board->Initialize();

	return p_map_board;
}

XIIlib::BoardMap::BoardMap()
{
}

XIIlib::BoardMap::~BoardMap()
{
	for (auto tiles : map_board) {
		for (auto tile : tiles) {
			delete tile;
		}
	}
}

void XIIlib::BoardMap::Initialize()
{
	// マップの生成
	const int elementZ = 8;
	const int elementX = 8;
	map_board.resize(elementZ);
	for (int i = 0; i < elementZ; i++) {
		map_board.at(i).resize(elementX);
	}
	
	Math::Vector2 pos_start = { -17.5f,-17.5f };
	const Math::Vector3 startPoint = { 0.5f,0.5f,0.5f };
	const Math::Vector3 endPoint = startPoint * -1.0f;

	for (int i = 0; i < elementZ; i++) {
		for (int j = 0; j < elementX; j++) {
			map_board[i][j] = Tile::Create(pos_start.x, pos_start.y);
			pos_start.x += 5.0f;
		}

		pos_start.y += 5.0f;
		pos_start.x = -17.5f;
	}
}

void XIIlib::BoardMap::Update()
{
	for (auto tiles : map_board) {
		for (auto tile : tiles) {
			tile->Update();
		}
	}
}

void XIIlib::BoardMap::Draw()
{
	CollisionBox::PreDraw();
	for (auto tiles : map_board) {
		for (auto tile : tiles) {
			tile->Draw();
		}
	}
	CollisionBox::PostDraw();
}

void XIIlib::BoardMap::SetBackVector(const Math::Point2& element, const Math::Point2& point)
{
	map_board[element.b][element.a]->SetBackVector(point);
}

void XIIlib::BoardMap::ChangeAttackValidTile(const Math::Point2& element, int type_positioning)
{
	if (type_positioning == 0) {
		map_board[element.b][element.a]->SetPlayerAttackValid();
	}
	else if (type_positioning == 1) {
		map_board[element.b][element.a]->SetEnemyAttackValid();
	}
	//二体以上の攻撃可能マス
	else if (type_positioning == 2) {
		map_board[element.b][element.a]->SetAttackPointValid();
	}
	//一体のみの攻撃可能マス
	else if (type_positioning == 3) {
		map_board[element.b][element.a]->SetMovePointValid();
	}
	else {
		map_board[element.b][element.a]->SetMovePointValid();
	}
	
}

void XIIlib::BoardMap::SetOnTile(const Math::Point2& element)
{
	map_board[element.b][element.a]->SetOnTile();
}

bool XIIlib::BoardMap::IsAttackValid(const Math::Point2& element, int type_positioning) const
{
	if (type_positioning == 0) {
		return map_board[element.b][element.a]->IsPlayerAttackValid();
	}
	else if (type_positioning == 1) {
		return map_board[element.b][element.a]->IsEnemyAttackValid();
	}
}

Math::Point2 XIIlib::BoardMap::GetBackVector(const Math::Point2& element) const
{
	return map_board[element.b][element.a]->GetBackVector();
}

bool XIIlib::BoardMap::GetOnTile(const Math::Point2& element) const
{
	return map_board[element.b][element.a]->GetOnTile();
}

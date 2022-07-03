#include "BoardMap.h"
#include "../Struct/Math.h"
#include "Tile.h"
#include "../3D/CollBox.h"
#include "Common.h"

XIIlib::BoardMap* XIIlib::BoardMap::Create()
{
	BoardMap* p_map_board = new BoardMap();
	if (p_map_board == nullptr) {
		return nullptr;
	}

	// ������
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
	// �}�b�v�̐���
	const int elementZ = 8;
	const int elementX = 8;
	map_board.resize(elementZ);
	for (int i = 0; i < elementZ; i++) {
		map_board.at(i).resize(elementX);
	}
	
	// ���W���߂�萔�Q
	const float tileDistance = 5.0f,startValue = -17.5f;
	// ���W�i�[�p
	Math::Vector2 pos = { startValue,startValue };

	// ���W�̑��
	for (int i = 0; i < elementZ; i++) {
		for (int j = 0; j < elementX; j++) {
			map_board[i][j] = Tile::Create(pos.x, pos.y);
			pos.x += tileDistance;
		}

		pos.y += tileDistance;
		pos.x = startValue;
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
	for (auto tiles : map_board) {
		for (auto tile : tiles) {
			tile->Draw();
		}
	}
}

void XIIlib::BoardMap::AllFlat()
{
	for (auto y : map_board) {
		for (auto x : y) {
			x->Flat();
		}
	}
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
	//��̈ȏ�̍U���\�}�X
	else if (type_positioning == 2) {
		map_board[element.b][element.a]->SetAttackPointValid();
	}
	//��݂̂̂̍U���\�}�X
	else if (type_positioning == 3) {
		map_board[element.b][element.a]->SetMovePointValid();
	}
	//�{�X�̍U���}�X
	else if (type_positioning == 4) {
		map_board[element.b][element.a]->SetBossAttackValid();
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
	if (Common::GetExceptionPoint(element.a))return false;
	if (Common::GetExceptionPoint(element.b))return false;
	
	if (type_positioning == 0) {
		return map_board[element.b][element.a]->IsPlayerAttackValid();
	}
	else if (type_positioning == 1) {
		return map_board[element.b][element.a]->IsEnemyAttackValid();
	}
	else if(type_positioning==2)
	{
		return map_board[element.b][element.a]->IsBossAttackValid();
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

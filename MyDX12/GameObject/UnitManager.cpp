#include "UnitManager.h"
#include "../3D/CollCapsule.h"
#include <iostream>
#include "BoardMap.h"
#include "../Struct/Math.h"
#include "../3D/Object3D.h"
#include "../3D/BillObj.h"
#include "../Audio/Audio.h"
#include "Common.h"
#include "GrainManager.h"

XIIlib::UnitManager::UnitManager() {}

XIIlib::UnitManager::~UnitManager()
{
	delete grainMs;
	delete map_board;
}

XIIlib::UnitManager* XIIlib::UnitManager::GetInstance()
{
	static XIIlib::UnitManager instance;
	return &instance;
}

void XIIlib::UnitManager::Initialize()
{
	std::cout << "Unit->初期化しました。" << std::endl;
	add_units.reserve(100);
	map_board = BoardMap::Create();

	hitPos.reserve(100);
	grainMs = GrainManager::Create();
}

void XIIlib::UnitManager::Update()
{
	map_board->Update();

	for (auto& unit : units)
	{
		unit->Update();
	}

	// ここに「重なったらダメージを入れる」を書く
	for (auto& unit1 : units) {
		for (auto& unit2 : units) {
			if (unit1->GetFallFlag() != unit2->GetFallFlag() || unit1.get() == unit2.get() || unit1->IsDead() || unit2->IsDead())continue;

			// 重なったら死ぬ
			if (Math::MatchPoint2(unit1->GetElementStock(),unit2->GetElementStock())) {
				GetAudio()->PlaySE("hitAB.wav",0.3f);
				unit1->SetHitDamage(3);
				unit2->SetHitDamage(3);

				/*Math::Vector3 addPoint = {
					Common::ConvertTilePosition(unit1->GetElementStock().a),
					2.0f,
					Common::ConvertTilePosition(unit1->GetElementStock().b)
				};*/
				// 何もない状態で取得する場合はそのまま追加
				if(hitPos.size() == 0)hitPos.push_back(unit1->GetElementStock());
				else {// そうでない場合は同じ物がないか確認して衝突しなければ追加
					for (auto x : hitPos) {
						if (Math::MatchPoint2(x, unit1->GetElementStock()))continue;

						hitPos.push_back(unit1->GetElementStock());
					}
				}
			}
		}
	}

	// ここでパーティクルを発生させる
	//GrainCreate();
	hitPos.clear();

	// hpが0のやつを死なせる。
	for (auto& unit : units) {
		unit->ZeroHp();
	}

	if (add_units.size() != 0)
	{
		for (auto& m : add_units)
		{
			m.get()->Update();
			units.push_back(std::move(m));
		}

		add_units.clear();
	}
}

void XIIlib::UnitManager::Draw()
{
	// モデルの描画(.obj)
	Object3D::PreDraw();

	// マスの描画
	map_board->Draw();

	// 駒の描画
	for (auto& obj : units)
	{
		obj->Draw();
	}

	Object3D::PostDraw();
}

void XIIlib::UnitManager::BillDraw()
{
	// 板ポリビルボードオブジェクトの描画
	BillObj::PreDraw();
	map_board->DrawBillObj();

	for (auto& obj : units)
	{
		obj->BillObjectDraw();
	}

	grainMs->Draw();
	BillObj::PostDraw();
}

void XIIlib::UnitManager::AddUnit(const std::shared_ptr<Unit>& spObject)
{
	if (spObject)
	{
		spObject.get()->Initialize();
		add_units.push_back(std::move(spObject));
	}
}

void XIIlib::UnitManager::DeadUnit()
{
	auto removeIt = std::remove_if(units.begin(), units.end(), [&](std::shared_ptr<Unit> itr)
		{
			return itr->IsDead() == true;
		}
	);

	units.erase(removeIt, units.end());
}

void XIIlib::UnitManager::AllDestroy()
{
	units.clear();
}

bool XIIlib::UnitManager::AllOnUnit(const Math::Point2& point)
{
	for (auto unit : units) {
		if (!unit->GetFallFlag())continue;

		if (Math::MatchPoint2(unit->GetElementStock(),point)) {
			return true;
		}
	}
	return false;
}

bool XIIlib::UnitManager::IsPlayerOnTile(const Math::Point2& point)
{
	return false;
}

int XIIlib::UnitManager::GetUnitIDElements(std::string name) const
{
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].get()->GetID() == name)
		{
			return i;
		}
	}
	return -1;
}

// 指定要素数のオブジェクトデータ丸ごと取得
std::shared_ptr<XIIlib::Unit> XIIlib::UnitManager::GetUnit(int index)const
{
	return units[index];
}

std::vector<int> XIIlib::UnitManager::GetUnitNumbers(std::string name)const
{
	std::vector<int> array_numbers;
	for (int i = 0; i < units.size(); i++)
	{
		if (units[i].get()->GetID() == name)
		{
			array_numbers.push_back(i);
		}
	}
	return array_numbers;
}

void XIIlib::UnitManager::SetBackVector(const Math::Point2& element, const Math::Point2& point)
{
	map_board->SetBackVector(element, point);
}

void XIIlib::UnitManager::FlatTileState()
{
	map_board->AllFlat();
	map_board->Update();
}

void XIIlib::UnitManager::ChangeAttackValidTile(const Math::Point2& element, int type_positioning)
{
	map_board->ChangeAttackValidTile(element,type_positioning);
}

void XIIlib::UnitManager::SetOnTile(const Math::Point2& element)
{
	map_board->SetOnTile(element);
}

bool XIIlib::UnitManager::IsAttackValid(const Math::Point2& element, int type_positioning) const
{
	return map_board->IsAttackValid(element, type_positioning);
}

Math::Point2 XIIlib::UnitManager::GetBackVector(const Math::Point2& element) const
{
	return map_board->GetBackVector(element);
}

bool XIIlib::UnitManager::GetOnTile(const Math::Point2& element) const
{
	return map_board->GetOnTile(element);
}

std::vector<int> XIIlib::UnitManager::GetOnUnitElements(const Math::Point2& point)
{
	std::vector<int> unit_elements;
	unit_elements.reserve(20);
	int i = 0;

	for (auto unit : units) {
		if (Math::MatchPoint2(unit->GetElementStock(), point)) {
			unit_elements.push_back(i);
		}
		i++;
	}
	return unit_elements;
}

int XIIlib::UnitManager::GetAllUnitCount() const
{
	return units.size();
}

void XIIlib::UnitManager::AddCountDead()
{
	deadCount++;
}

void XIIlib::UnitManager::ResetCount()
{
	deadCount = 0;
}

void XIIlib::UnitManager::ObjectUpdate()
{
	map_board->ObjectUpdate();

	for (auto unit : units)
	{
		unit->ObjectUpdate();
	}
}

void XIIlib::UnitManager::SetBoardBaseColor(float r, float g, float b)
{
	map_board->SetBaseColors(r, g, b);
}

void XIIlib::UnitManager::GrainCreate()
{
	// パーティクルの更新
	grainMs->Update();

	// 追加するパーティクルが無ければ即リターン
	if (hitPos.size() == 0)return;

	// あれば追加する
	// 粒子の追加
	for (auto points : hitPos) {
		// 出現点の取得
		Math::Vector3 addP = {
			Common::ConvertTilePosition(points.a),
			2.0f,
			Common::ConvertTilePosition(points.b)
		};

		// 粒子の移動ベクトルを算出し順次追加(計20個)
		for (int i = 0; i < 20; i++) {
			Math::Vector3 addV;
			const int rdist = 10;
			const int rdist2 = rdist * 2 + 1;
			float rX = static_cast<float>(rand()%rdist2 - rdist);
			float rZ = static_cast<float>(rand()%rdist2 - rdist);

			addV = { rX,0.1f,rZ };
			addV.normalize();
			grainMs->Add(addV, addP);
		}
	}
	
	// 登録の解除
	hitPos.clear();
}

bool XIIlib::UnitManager::GetStoneOnTile(const Math::Point2& nextP)
{
	for (auto u : units) {
		// 障害物出なければスキップ
		if (u->GetID() != "Stone")continue;

		// 等しければそのマスに存在する
		if (Math::MatchPoint2(u->GetElementStock(), nextP)) {
			return true;
		}
	}
	return false;
}
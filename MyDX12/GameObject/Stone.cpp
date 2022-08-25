#include "Stone.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../GameObject/AttackTimer.h"
#include "ModelLoader.h"


XIIlib::Stone::Stone()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 3;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Stone::~Stone()
{
	delete attackTimer;
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
	_hit_point = 3;
	// クラスネーム取得
	const type_info& id = typeid(Stone);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_GUARDMAN));
	object3d->scale = Math::Vector3({1.6f,1.6f,1.6f});
	object3d->position.y = 30.0f;
	dispATFlag = false;
	SetAttackTimer(3);
}

void XIIlib::Stone::Update()
{
	if (!fallFlag) {
		// X/Zの座標を代入しておく
		object3d->position.x = Common::ConvertTilePosition(element_stock.a);
		object3d->position.z = Common::ConvertTilePosition(element_stock.b);
		object3d->position.y -= 0.4f;

		if (object3d->position.y <= 1.0f) {
			object3d->position.y = 1.0f;
			fallFlag = true;
		}

		object3d->Update();
	}
	else if (fallFlag) {
		// 位置座標の更新
		object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	
		// 駒の行動
		Action();
		// タイマーの更新
		attackTimer->Timer();	
		object3d->Update();
	}

	// 攻撃当たっていなければそく返す
	if (!UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))return;
	Damage();

	if (nodamage)
	{
		nodamageTime--;
	}
	if (nodamageTime <= 0)
	{
		nodamage = false;
	}
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

void XIIlib::Stone::Damage()
{
	if (!nodamage)
	{
		SetHitDamage(1.0f);
		nodamageTime = defaultNodamageTime;
		nodamage = true;
	}
}
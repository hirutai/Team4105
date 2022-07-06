#include "King.h"
#include "../Input/KeyInput.h"
#include "Common.h"
#include "UnitManager.h"
#include "Rook.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../3D/BillObj.h"
#include "../Tool/Easing.h"

XIIlib::King::King()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::King::~King()
{
	delete attackAreasBillboard;
	delete object3d;
	delete daiza;
}

std::shared_ptr<XIIlib::King> XIIlib::King::CreateKing(int point_x, int point_z)
{
	std::shared_ptr<King> pawn = std::make_shared<King>();
	pawn.get()->SetElementStock(point_x, point_z);
	pawn.get()->Initialize();

	return std::move(pawn);
}

void XIIlib::King::Initialize()
{
	// 特になし
	_hit_point = 2;
	//行動タイマーとその遅延分の数値
	moveCount = 0;
	moveLag = 20;

	// クラスネーム取得
	const type_info& id = typeid(King);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::MINE;

	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();

	CreateAttackArea();

	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Bat_1"));
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});
	daiza = Object3D::Create(Model::CreateFromOBJ("daiza"));

	attackAreasBillboard = BillObj::Create({0,-1,0},"swing_L.png");
	const float mullPower = 0.8f;
	attackAreasBillboard->SetSize({19.2f * mullPower,6.4f * mullPower });
	correctionAngle = 180.0f;
}

void XIIlib::King::Update()
{
	// 位置座標の更新
	object3d->position = { Common::ConvertTilePosition(element_stock.a),2.0f, Common::ConvertTilePosition(element_stock.b) };
	daiza->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
	
	if (!determinateMoveAction) {
		Action();

		pos = object3d->position;
	}
	else {
		const float maxTime = 1.0f/3.0f;
		movingTimer += (1.0f / 40.0f);
		Math::Vector3 nowP = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
		Math::Vector3 nextP = { Common::ConvertTilePosition(nextPoint.a),1.0f, Common::ConvertTilePosition(nextPoint.b) };
		Math::Vector3 v = nextP - nowP;

		bool isVx = false, isVz = false;
		if (v.x < 0.0f) {
			isVx = true;
			v.x *= -1.0f;
		}
		if (v.z < 0.0f) {
			isVz = true;
			v.z *= -1.0f;
		}

		float resultX = Easing::InOutCubic(movingTimer, 0.0f, v.x, maxTime);
		float resultZ = Easing::InOutCubic(movingTimer, 0.0f, v.z, maxTime);
		if (isVx)resultX *= -1.0f;
		if (isVz)resultZ *= -1.0f;

		object3d->position.x = pos.x + resultX;
		object3d->position.z = pos.z + resultZ;
		daiza->position.x = object3d->position.x;
		daiza->position.z = object3d->position.z;

		if (movingTimer >= maxTime) {
			determinateMoveAction = false;
			element_stock = nextPoint;
			movingTimer = 0.0f;
			nextPoint = Math::Point2(0, 0);
			pos = Math::Vector3();
		}
	}

	if (moveCount <= 0) {
		if (type_attack != AREA::NONE) {
			std::vector<Math::Point2> container = attack_area[(int)type_attack];
			int add_element = 0;
			moveCount = moveLag;
			Math::Vector2 at[3];
			for (int i = 0; i < 3; i++) {
				container[i] += element_stock;
				if ((container[i].a < 8 && container[i].a >= 0) && (container[i].b < 8 && container[i].b >= 0)) {
					audio_->PlaySE("swing.wav");
					UnitManager::GetInstance()->ChangeAttackValidTile(container[i], (int)type);
					Math::Point2 vec_point = container[i] - element_stock;
					UnitManager::GetInstance()->SetBackVector(container[i], vec_point * 2);
				}
			}
			isDrawAtArea = true;
		}
	}

	if (isDrawAtArea) {
		attackAreasBillboard->SetPosition(attackAreasBillboard->GetPosition().x,2.0f, attackAreasBillboard->GetPosition().z);
		drawAtArea++;
		if (drawAtArea == 10) {
			drawAtArea = 0;
			isDrawAtArea = false;
		}
	}
	if (is_hit) {
		damage_counter++;

		if (damage_counter > 20) {
			is_hit = 0;
			damage_counter = 0;
		}
	}

	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::BOSS))
	{
		OnDead();
	}
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::BOSS_KNOCKBACK))
	{
		//OnDead();
		element_stock.b -= 3;
		PlayerKnockBack();
	}

	object3d->Update();
	daiza->Update();
}

void XIIlib::King::Draw()
{
	object3d->Draw();
	daiza->Draw();
}

void XIIlib::King::OriginBillDraw()
{
	//if (type_attack == AREA::NONE)return;

	if (isDrawAtArea) // 攻撃中
	{
		attackAreasBillboard->Draw();
	}
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

	if (KeyInput::GetInstance()->Push(DIK_W)) {

		next_state += move_vec[0];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			nextPoint = next_state;
			moveCount = moveLag;
			// 移動量を取得
			Math::Point2 v = nextPoint - element_stock;
			//移動量から角度を求めて設定
			Direction(v);
			// 移動ますが決定されました。
			determinateMoveAction = true;
		}
	}
	else if (KeyInput::GetInstance()->Push(DIK_S)) {
		next_state += move_vec[1];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			nextPoint = next_state;
			moveCount = moveLag;
			// 移動量を取得
			Math::Point2 v = nextPoint - element_stock;
			//移動量から角度を求めて設定
			Direction(v);
			// 移動ますが決定されました。
			determinateMoveAction = true;
		}
	}

	if (KeyInput::GetInstance()->Push(DIK_A)) {
		next_state += move_vec[2];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			nextPoint = next_state;
			moveCount = moveLag;
			// 移動量を取得
			Math::Point2 v = nextPoint - element_stock;
			//移動量から角度を求めて設定
			Direction(v);
			// 移動ますが決定されました。
			determinateMoveAction = true;
		}
	}
	else if (KeyInput::GetInstance()->Push(DIK_D)) {
		next_state += move_vec[3];
		if (!UnitManager::GetInstance()->AllOnUnit(next_state)) {
			nextPoint = next_state;
			moveCount = moveLag;
			// 移動量を取得
			Math::Point2 v = nextPoint - element_stock;
			//移動量から角度を求めて設定
			Direction(v);
			// 移動ますが決定されました。
			determinateMoveAction = true;
		}
	}

	nextPoint = Common::OffsetTilePosition2(nextPoint);
}

void XIIlib::King::Attack()
{
	Math::Point2 tmpAttackArea;
	
	if (KeyInput::GetInstance()->Push(DIK_UP)) {
		type_attack = AREA::UP;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = 180.0f;
		attackAreasBillboard->SetRotation(0,0,0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);
	}
	else if (KeyInput::GetInstance()->Push(DIK_LEFT)) {
		type_attack = AREA::LEFT;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = 90.0f;
		attackAreasBillboard->SetRotation(0, -90, 0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);
	}
	else if (KeyInput::GetInstance()->Push(DIK_DOWN)) {
		type_attack = AREA::DOWN;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = 0.0f;
		attackAreasBillboard->SetRotation(0, 180, 0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);
	}
	else if (KeyInput::GetInstance()->Push(DIK_RIGHT)) {
		type_attack = AREA::RIGHT;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = -90.0f;
		attackAreasBillboard->SetRotation(0, 90, 0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);
	}
	else {
		type_attack = AREA::NONE;
	}
}

void XIIlib::King::Action()
{
	if (moveCount <= 0) {
		Move();
	}
	else
	{
		moveCount--;
	}
	Attack();
}

bool XIIlib::King::AttackAreaExists()
{
	return false;
}

void XIIlib::King::IniState()
{
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

void XIIlib::King::ObjectUpdate()
{
	object3d->Update();
	daiza->Update();
}
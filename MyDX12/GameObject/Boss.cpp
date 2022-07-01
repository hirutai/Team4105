#include "Boss.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/DebugText.h"
#include "../Tool/Easing.h"

XIIlib::Boss::Boss()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = defaultHp;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Boss::~Boss()
{
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Boss> XIIlib::Boss::Create(int point_x, int point_z)
{
	std::shared_ptr<Boss> rook = std::make_shared<Boss>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Boss::Initialize()
{
	// 特になし
	_hit_point = defaultHp;

	// クラスネーム取得
	const type_info& id = typeid(Boss);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();

	SetAttackTimer(2);

	nextPoint = { 0,0 };
}

void XIIlib::Boss::Update()
{
	// 位置座標の更新
	object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };

	if (!determinateMoveAction) {
		// 駒の行動
		Action();

		// タイマーの更新
		attackTimer->Timer();

		pos = object3d->position;
	}
	else {
		const float maxTime = 1.0f;
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

		if (movingTimer >= maxTime) {
			determinateMoveAction = false;
			element_stock = nextPoint;
			movingTimer = 0.0f;
			nextPoint = Math::Point2(0, 0);
			pos = Math::Vector3();
		}
	}
	object3d->Update();

	// 座標設定
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Boss::Action()
{
	// 未来への栄光のロードを初期化
	nextPoint = { 0,0 };

	// 範囲に入ってるかのチェック
	if (AttackAreaExists())
	{
		Math::Point2 dif = kingPos - element_stock;
		Math::Point2 norm(0, 0);
		if (dif.a != 0) { norm.a = dif.a / abs(dif.a); }
		else { norm.a = dif.a; }
		if (dif.b != 0) { norm.b = dif.b / abs(dif.b); }
		else { norm.b = dif.b; }

		if (dif.a == 0)
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.b))return;
		}
		else
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.a))return;
		}

		if (isAttack == false)
		{
			isAttack = true;
			preElement_stock = kingPos;
		}
	}
	else
	{
		Move();
	}

	if (attackTimer->SizeZeroFlag())
	{
		notAttackflag = TRUE;
	}

	if (isAttack == true && notAttackflag == true)
	{
		point_attack = preElement_stock;
		//攻撃マスの色を変える
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
		if (type_attack != AREA::NONE) {
			std::vector<Math::Point2> container = attack_area[(int)type_attack];
			int add_element = 0;
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
		}
	}
	else
	{
		isAttack = false;
		//移動範囲の色付け
		AttackAreaDraw();
	}

	// ノックバック
	// 攻撃当たっていなければそく返す
	if (UnitManager::GetInstance()->IsAttackValid(element_stock, (int)_PositionType::MINE))
	{
		_hit_point--;
	}
}

void XIIlib::Boss::Attack()
{
	//色を変える
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		//Math::Point2 tmpAttackArea;

		/*if (KeyInput::GetInstance()->Push(DIK_UP)) {
			type_attack = AREA::UP;
			tmpAttackArea = attack_area[(int)type_attack][1];
			object3d->rotation.y = 180.0f;
			attackAreasBillboard->SetRotation(0, 0, 0);
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
			attackAreasBillboard->SetPosition(
				object3d->position.x,
				-1.0f,
				object3d->position.z
			);
		}*/

		/*type_attack = AREA::DOWN;
		tmpAttackArea = attack_area[(int)type_attack][1];
		object3d->rotation.y = 0.0f;
		attackAreasBillboard->SetRotation(0, 180, 0);
		attackAreasBillboard->SetPosition(
			Common::ConvertTilePosition(element_stock.a + tmpAttackArea.a),
			-1.0f, Common::ConvertTilePosition(element_stock.b + tmpAttackArea.b)
		);*/
		// 攻撃
		nextPoint = preElement_stock;
		audio_->PlaySE("yankeeVoice.wav");
		IniState();

		// 移動ますが決定されました。
		determinateMoveAction = true;
	}
}

void XIIlib::Boss::Move()
{
	//攻撃フラグ
	if (isAttack == true)return;
	//移動までのインターバル
	if (!attackTimer->SizeZeroFlag())return;
	//ヤンキーの座標
	Math::Point2 temp = element_stock;
	nextPoint = element_stock;
	notAttackflag = TRUE;


	//プレイヤーの座標 ー ヤンキーの座標
	Math::Point2 dif = kingPos - element_stock;

	// 自分とキングの間を1マスづつ調べる
	if (dif.b < 0 && dif.a == 0)//キングより下にいるかを調べる
	{
		//上に１進む
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;

		nextPoint.b -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b > 0 && dif.a == 0)//キングより上にいるかを調べる
	{
		//下に1進む
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.b += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.a < 0 && dif.b == 0)//キングより左にいるかを調べる
	{
		//右に１進む
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	else if (dif.a > 0 && dif.b == 0)// 0より大きければKingより右にいる
	{
		//左に１進む
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b <= 0 && dif.a <= 0)//キングより左下にいるかを調べる
	{
		//右上に1進む
		temp.a -= 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		nextPoint.b -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b > 0 && dif.a <= 0)//キングより左上にいるかを調べる
	{
		//右下に1進む
		temp.a -= 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		nextPoint.b += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b <= 0 && dif.a > 0)//キングより右下にいるかを調べる
	{
		//左上に1進む
		temp.a += 1;
		temp.b -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		nextPoint.b -= 1;
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b > 0 && dif.a >= 0)//キングより右上にいるかを調べる
	{
		//左下に1進む
		temp.a += 1;
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		nextPoint.b += 1;
		audio_->PlaySE("yankeeVoice.wav");
	}

	// 移動ますが決定されました。
	determinateMoveAction = true;
}

bool XIIlib::Boss::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	if (kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b ||     //右
		kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b + 1 || //右上
		kingPos.a == element_stock.a + 1 && kingPos.b == element_stock.b - 1 || //右下
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b ||     //左
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b - 1 || //左下
		kingPos.a == element_stock.a - 1 && kingPos.b == element_stock.b + 1 || //左上
		kingPos.a == element_stock.a && kingPos.b == element_stock.b + 1 ||     //上
		kingPos.a == element_stock.a && kingPos.b == element_stock.b - 1        //下
		)return true;
	return false;
}

void XIIlib::Boss::AttackAreaDraw()
{
	//左方向に駒があるか

	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b)) || element_stock.a - 1 < 0)
	{

	}
	//なかったら移動範囲描画
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b));
	}
	//右方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b)) || element_stock.a + 1 > 7)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b));
	}
	//下方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b - 1)) || element_stock.b - 1 < 0)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b - 1));
	}
	//上方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a, element_stock.b + 1)) || element_stock.b + 1 > 7)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a, element_stock.b + 1));
	}
	//左下方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b - 1)) || element_stock.a - 1 < 0 || element_stock.b - 1 < 0)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b - 1));
	}
	//右下方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b - 1)) || element_stock.a + 1 > 7 || element_stock.b - 1 < 0)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b - 1));
	}
	//左上方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - 1, element_stock.b + 1)) || element_stock.a - 1 < 0 || element_stock.b + 1 > 7)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - 1, element_stock.b + 1));
	}
	//右上方向に駒があるか
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b + 1)) || element_stock.a + 1 > 7 || element_stock.b + 1 > 7)
	{

	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b + 1));
	}
}

void XIIlib::Boss::IniState()
{
	isAttack = false;
}

void XIIlib::Boss::CreateAttackArea()
{

}

bool XIIlib::Boss::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

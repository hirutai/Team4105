#include "Yankee.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/DebugText.h"

XIIlib::Yankee::Yankee()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 2;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Yankee::~Yankee()
{
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Yankee> XIIlib::Yankee::Create(int point_x, int point_z)
{
	std::shared_ptr<Yankee> rook = std::make_shared<Yankee>();
	rook.get()->SetElementStock(point_x, point_z);
	rook.get()->Initialize();

	return std::move(rook);
}

void XIIlib::Yankee::Initialize()
{
	// 特になし
	_hit_point = 2;

	// クラスネーム取得
	const type_info& id = typeid(Yankee);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(Model::CreateFromOBJ("Badboy_Enemy"));
	object3d->scale = Math::Vector3({2.0f,2.0f,2.0f});
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();

	SetAttackTimer(5);

	nextPoint = { 0,0 };
}

void XIIlib::Yankee::Update()
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
		// モーション処理
		Motion();
	}
	object3d->Update();

	// 座標設定
	attackTimer->SetPosition(object3d->position);
}

void XIIlib::Yankee::Action()
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
	}
	else
	{
		isAttack = false;
		//移動範囲の色付け
		AttackAreaDraw();
	}

	// ノックバック(共通処理)
	KnockBack();
}

void XIIlib::Yankee::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		// 攻撃
		nextPoint = preElement_stock;
		Math::Point2 addResult = element_stock - nextPoint;
		
		if (Math::MatchPoint2(addResult,pointArray[static_cast<int>(RotState::UP_LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_LEFT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::UP)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::UP_RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN_RIGHT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_RIGHT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::DOWN_LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_LEFT)];
		}
		else if (Math::MatchPoint2(addResult, pointArray[static_cast<int>(RotState::LEFT)])) {
			object3d->rotation.y = rotArray[static_cast<int>(RotState::LEFT)];
		}

		audio_->PlaySE("yankeeVoice.wav");
		IniState();

		// 移動ますが決定されました。
		determinateMoveAction = true;
	}
}

void XIIlib::Yankee::Move()
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
		object3d->rotation.y = rotArray[static_cast<int>(RotState::UP)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.b > 0 && dif.a == 0)//キングより上にいるかを調べる
	{
		//下に1進む
		temp.b += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.b += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	// 自分とキングの間を1マスづつ調べる
	else if (dif.a < 0 && dif.b == 0)//キングより左にいるかを調べる
	{
		//右に１進む
		temp.a -= 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a -= 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::RIGHT)];
		audio_->PlaySE("yankeeVoice.wav");
	}
	else if (dif.a > 0 && dif.b == 0)// 0より大きければKingより右にいる
	{
		//左に１進む
		temp.a += 1;
		if (ThreeCheckArea(temp))return;
		nextPoint.a += 1;
		object3d->rotation.y = rotArray[static_cast<int>(RotState::LEFT)];
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
		object3d->rotation.y = rotArray[static_cast<int>(RotState::UP_RIGHT)];
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
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_RIGHT)];
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
		object3d->rotation.y =
			rotArray[static_cast<int>(RotState::UP_LEFT)];
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
		object3d->rotation.y = rotArray[static_cast<int>(RotState::DOWN_LEFT)];
		audio_->PlaySE("yankeeVoice.wav");
	}

	// 移動ますが決定されました。
	determinateMoveAction = true;
}

bool XIIlib::Yankee::AttackAreaExists()
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

void XIIlib::Yankee::AttackAreaDraw()
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
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a , element_stock.b - 1));
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
	if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + 1, element_stock.b +1)) || element_stock.a + 1 > 7 || element_stock.b + 1 > 7)
	{
	
	}
	else
	{
		AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + 1, element_stock.b + 1));
	}
}

void XIIlib::Yankee::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Yankee::CreateAttackArea()
{

}

bool XIIlib::Yankee::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

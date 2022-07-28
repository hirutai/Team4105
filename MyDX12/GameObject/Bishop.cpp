#include "Bishop.h"
#include "King.h"
#include "Common.h"
#include "UnitManager.h"
#include "../Tool/Messenger.h"
#include "../3D/Object3D.h"
#include "../Audio/Audio.h"
#include "../GameObject/AttackTimer.h"
#include "../Tool/Easing.h"
#include "ModelLoader.h"
#include "SmokeManager.h"

XIIlib::Bishop::Bishop()
{
	// 各ステータスの初期化
	_cost = 0;
	_hit_point = 3;
	_attack_point = 2;
	_defense_point = 1;
}

XIIlib::Bishop::~Bishop()
{
	delete sManager;
	delete attackTimer;
	delete object3d;
}

std::shared_ptr<XIIlib::Bishop> XIIlib::Bishop::Create(int point_x, int point_z)
{
	std::shared_ptr<Bishop> bishop = std::make_shared<Bishop>();
	bishop.get()->SetElementStock(point_x, point_z);
	bishop.get()->Initialize();

	return std::move(bishop);
}

void XIIlib::Bishop::Initialize()
{
	// 特になし
	_hit_point = 3;

	// クラスネーム取得
	const type_info& id = typeid(Bishop);
	std::string path = id.name();
	ID = Common::SeparateFilePath(path).second;
	type = _PositionType::ENEMY;
	CreateAttackArea();
	object3d = Object3D::Create(ModelLoader::GetInstance()->GetModel(MODEL_BISHOP));
	object3d->scale = Math::Vector3({ 2.0f,2.0f,2.0f });
	object3d->rotation.y = 45.0f;
	object3d->position.y = 30.0f;
	// Audioの初期化
	audio_ = UnitManager::GetInstance()->GetAudio();
	correctionAngle = 90.0f;

	SetAttackTimer(countingNum);

	intTimeCount = 3;
	sManager = SmokeManager::Create();
}

void XIIlib::Bishop::Update()
{
	if (!fallFlag) {
		FallAction();
	}
	else if (fallFlag) {
		// 位置座標の更新
		object3d->position = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
		if (!determinateMoveAction) {
			// 駒の行動
			Action();
			// タイマーの更新
			attackTimer->Timer();

			pos = object3d->position;
			// エフェクトの設定
			intTimeCount = 3;
			sManager->AllClear();
		}
		if (determinateMoveAction) {
			// モーション処理
			Motion();
			// アドで動かす
			intTimeCount--;
			if (intTimeCount < 0) {
				Math::Vector3 nowP = { Common::ConvertTilePosition(element_stock.a),1.0f, Common::ConvertTilePosition(element_stock.b) };
				Math::Vector3 nextP = { Common::ConvertTilePosition(nextPoint.a),1.0f, Common::ConvertTilePosition(nextPoint.b) };
				Math::Vector3 v = nowP - nextP;
				v.normalize();
				float rnd_scale = static_cast<float>(rand() % 10 + 5) / 100.0f;
				float rnd_mullValue = static_cast<float>(rand() % 5 + 5) / 50.0f;
				sManager->Add(0.05f, rnd_scale, v * rnd_mullValue + Math::Vector3(0, 0.05f, 0), object3d->position + Math::Vector3(0, 2, 0));
				intTimeCount = 3;
			}
			sManager->Update();
		}

		object3d->Update();
		// 座標設定
		attackTimer->SetPosition(object3d->position + Math::Vector3(0.0f, 0.0f, -2.0f));
	}
}

void XIIlib::Bishop::OriginBillDraw()
{
	sManager->Draw();
}

void XIIlib::Bishop::Action()
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

		if (dif.a == dif.b)
		{
			// kingのポイション分を引いてfor文で調べる
			if (MoveAreaCheck(element_stock, norm, dif.b))return;
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

	if (isAttack == TRUE && notAttackflag == TRUE)
	{
		point_attack = preElement_stock;
		UnitManager::GetInstance()->ChangeAttackValidTile(point_attack, (int)type);
		Attack();
	}
	else
	{
		AttackAreaDraw();
		isAttack = false;
	}

	// ノックバック(共通処理)
	KnockBack();
}

void XIIlib::Bishop::Attack()
{
	// カウントを減らす
	attackInterval--;
	//色を変える
	if (attackTimer->SizeZeroFlag())
	{
		Math::Point2 dif = preElement_stock - element_stock;
		Math::Point2 temp = element_stock;

		Math::Point2 temp_norm;
		if (dif.a == 0) { temp_norm.a = dif.a; }
		else { temp_norm.a = dif.a / abs(dif.a); }
		if (dif.b == 0) { temp_norm.b = dif.b; }
		else { temp_norm.b = dif.b / abs(dif.b); }

		// 自分とキングの間を1マスづつ調べる
		for (int i = 0; i < abs(dif.a) - 1; ++i)
		{
			temp += temp_norm;
			if (UnitManager::GetInstance()->AllOnUnit(temp))
			{
				IniState();
				return;
			}
		}
		// 攻撃
		nextPoint = preElement_stock;
		// 移動量を取得
		Math::Point2 v = preElement_stock - element_stock;
		//移動量から角度を求めて設定
		Direction(v);

		audio_->PlaySE("pararira.wav");
		IniState();
		
		// 移動ますが決定されました。
		determinateMoveAction = true;
	}
}

void XIIlib::Bishop::Move()
{

	if (isAttack == true)return;
	if (!attackTimer->SizeZeroFlag())return;

	//notAttackflag = TRUE;

	//Math::Point2 dif = kingPos - element_stock;
	Math::Point2 temp = element_stock;
	nextPoint = element_stock;
	// もうすでに範囲上にいたら動かない
	//if (abs(dif.a) == abs(dif.b))return;
	//3マス以下しか動けない時の移動用乱数
	int BishopjMin = jMin;
	int BishopjMax = jMax;
	tileRand = 1;
	//Switch文用の乱数
	int BishopSwitchiMin = SwitchRandiMin;
	int BishopSwitchiMax = SwitchRandiMax;
	SwitchRand = SwitchRandiMin + (int)(rand() * (SwitchRandiMax - SwitchRandiMin + 1) / (1 + RAND_MAX));

	switch (SwitchRand)
	{
	case 0:
		//左下方向			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		temp.b -= tileRand;
		if (temp.a <= -1 && temp.b <= -1)
		{
			nextPoint.a = 0;
			nextPoint.b = 0;
		}
		else if(temp.a <= -1)
		{
			nextPoint.b = temp.b - temp.a;
			nextPoint.a = 0;
		}
		else if (temp.b <= -1)
		{
			nextPoint.a = temp.a - temp.b;
			nextPoint.b = 0;
		}
		else
		{
			nextPoint = temp;
		}
		audio_->PlaySE("pararira.wav");
		break;
	case 1:
		//右下方向			
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;
		temp.b -= tileRand;
		
		if (temp.a >= 8 && temp.b <= -1)
		{
			nextPoint.a = 7;
			nextPoint.b = 0;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint)) 
			{
				nextPoint = temp;
			}
		}
		else if (temp.a >= 8)
		{
			nextPoint.b = temp.b + (temp.a - 7);
			nextPoint.a = 7;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else if (temp.b <= -1)
		{
			nextPoint.a = temp.a + temp.b;
			nextPoint.b = 0;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else
		{
			nextPoint = temp;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		audio_->PlaySE("pararira.wav");
		break;
	case 2:
		//左上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a -= tileRand;
		temp.b += tileRand;
		if (temp.a <= -1 && temp.b >= 8)
		{
			nextPoint.a = 0;
			nextPoint.b = 7;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else if (temp.a <= -1)
		{
			nextPoint.b = temp.b + temp.a;
			nextPoint.a = 0;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else if (temp.b >= 8)
		{
			nextPoint.a = temp.a - (temp.b - 7);
			nextPoint.b = 7;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else
		{
			nextPoint = temp;
		}
		audio_->PlaySE("pararira.wav");
		break;
	case 3:
		//右上方向
		tileRand = jMin + (int)(rand() * (jMax - jMin + 1) / (1 + RAND_MAX));

		temp.a += tileRand;
		temp.b += tileRand;
		if (temp.a >= 8 && temp.b >= 8)
		{
			nextPoint.a = 7;
			nextPoint.b = 7;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else if (temp.a >= 8)
		{
			nextPoint.a = 7;
			nextPoint.b = temp.b - (temp.a - 7);
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else if (temp.b >= 8)
		{
			nextPoint.a = temp.a - (temp.b - 7);
			nextPoint.b = 7;
			if (AttackAreaManager::GetInstance()->CheckMoveAreas(nextPoint))
			{
				nextPoint = temp;
			}
		}
		else
		{
			nextPoint = temp;
		}
		audio_->PlaySE("pararira.wav");
		break;
	}
	// 移動量を取得
	Math::Point2 v = nextPoint - element_stock;
	//移動量から角度を求めて設定
	Direction(v);

	AttackAreaManager::GetInstance()->SetMoveAreas(nextPoint);

	// 移動ますが決定されました。
	determinateMoveAction = true;
	return;
}

bool XIIlib::Bishop::AttackAreaExists()
{
	int element = UnitManager::GetInstance()->GetUnitIDElements("King");
	if (element != -1)
	{
		std::shared_ptr<King> p_king =
			std::dynamic_pointer_cast<King>(UnitManager::GetInstance()->GetUnit(element));
		kingPos = p_king->GetElementStock();
	}
	// 範囲に入ってるかのチェック
	Math::Point2 dif = kingPos - element_stock;
	// xとzの絶対値が一緒だったら攻撃範囲にいる範囲
	if (abs(dif.a) == abs(dif.b))return true;
	return false;
}

void XIIlib::Bishop::AttackAreaDraw()
{
	//左下
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b - i)) || element_stock.a - i < 0 || element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b - i));
			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b - i), 3);
		}
	}
	//右上
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + i, element_stock.b + i)) || element_stock.a + i > 7 || element_stock.b + i > 7)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b + i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b + i), 3);
		}
	}
	//右下
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a + i, element_stock.b - i)) || element_stock.a + i > 7 || element_stock.b - i < 0)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a + i, element_stock.b - i));

			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a + i, element_stock.b - i), 3);
		}
	}
	//左上
	for (int i = 1; i < 8; i++)
	{
		if (UnitManager::GetInstance()->AllOnUnit(Math::Point2(element_stock.a - i, element_stock.b + i)) || element_stock.a - i < 0 || element_stock.b + i > 7)
		{
			break;
		}
		else
		{
			AttackAreaManager::GetInstance()->SetAttackAreas(Math::Point2(element_stock.a - i, element_stock.b + i));
			//UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(element_stock.a - i, element_stock.b + i), 3);
		}
	}
}

void XIIlib::Bishop::IniState()
{
	isAttack = false;
	attackInterval = 180;
}

void XIIlib::Bishop::CreateAttackArea()
{

}

bool XIIlib::Bishop::MoveAreaCheck(Math::Point2 crPos, Math::Point2 vec, int tileNum)
{
	Math::Point2 pos = crPos;
	for (int i = 0; i < abs(tileNum) - 1; ++i)
	{
		pos += vec;
		if (UnitManager::GetInstance()->AllOnUnit(pos))return true;
	}
	return false;
}

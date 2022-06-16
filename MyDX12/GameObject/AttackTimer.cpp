#include "AttackTimer.h"

#include "../3D/BillObj.h"

using namespace XIIlib;
using namespace Math;

AttackTimer::AttackTimer(float countingNum_)
{
	countingNum = countingNum_; // カウントしたい数の入力
	decNum = baseDecNum / countingNum; //  // 減らす量の計算
	timerNum = 0;
	currentSize = maxSize;
}

void AttackTimer::Initialize()
{
	timerBar = BillObj::Create({}, "timer_bar.png"); // バーのオブジェクトの作成
	timerBar->SetAnchorPoint({ 0.0f, 0.5f });
	timerBar->SetSize(barSize); // バーのサイズの設定
	timerEdge = BillObj::Create({}, "timer_edge.png"); // 縁のオブジェクトの作成
	timerEdge->SetSize(edgeSize); // 縁のサイズの設定
}

void AttackTimer::Timer()
{
	if (SizeZeroFlag()) // サイズが０になった時
	{
		timerNum = 0; // タイマーをリセット
		currentSize = maxSize; // サイズをリセット
	}

	timerNum++; // タイマーカウント中

	currentSize = maxSize - decNum * timerNum; // 現在のサイズを計算

	barSize.x = currentSize; // バーのサイズの設定

	timerBar->SetSize(barSize); // バーのサイズの反映
}

void AttackTimer::Draw()
{
	timerBar->Draw(); // バーの座標の描画
	timerEdge->Draw(); // 縁の座標の描画
}

void AttackTimer::SetPosition(const Vector3& position)
{
	timerBar->SetPosition(position.x - 1.5f, position.y + 4, position.z - 1); // バーの座標の設定
	timerEdge->SetPosition(position.x, position.y + 4, position.z - 1); // 縁の座標の設定
}

bool XIIlib::AttackTimer::SizeZeroFlag()
{
	return currentSize <= 0;
}

#include "AttackTimer.h"

#include "../2D/Sprite.h"

using namespace XIIlib;

void AttackTimer::Initialize(const int& timerTexNum, const int& barTexNum)
{
	timer_bar = Sprite::Create(barTexNum, { 20.0f, 150.0f }); // タイマーバー画像の作成
}

void AttackTimer::Timer()
{
	if (currentSize <= 0) // サイズが０になった時
	{
		timerNum = 0; // タイマーをリセット
		currentSize = maxSize; // サイズをリセット
	}

	timerNum++; // タイマー

	currentSize = maxSize - decNum * timerNum; // 現在のサイズを計算

	timer_bar->SetSize({ currentSize, 50.0f }); // サイズの設定
}

void AttackTimer::Draw()
{
	timer_bar->Draw();
}
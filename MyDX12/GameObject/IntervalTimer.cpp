#include "IntervalTimer.h" 
#include "../2D/Sprite.h"

using namespace XIIlib;

void IntervalTimer::Initialize(const int& timerTexNum, const int& barTexNum)
{
	timer = Sprite::Create(timerTexNum, { 10.0f, 100.0f }); // タイマー画像の作成
	timer_bar = Sprite::Create(barTexNum, { 20.0f, 150.0f }); // タイマーバー画像の作成
}

void IntervalTimer::Timer()
{
	if (currentSize <= 0) // サイズが０になった時
	{
		timerNum = 0; // タイマーをリセット
		currentSize = maxSize; // サイズをリセット
	}

	timerNum++; // タイマー

	currentSize = maxSize - intCountingNum * timerNum; // 現在のサイズを計算

	timer_bar->SetSize({ currentSize, 50.0f });
}

void IntervalTimer::Draw()
{
	timer_bar->Draw();
	timer->Draw();
}
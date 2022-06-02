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
	if (timerNum == 0) // タイマーが０になった時
	{
		timerNum = intCountingNum; // タイマーをリセット
	}

	timerNum--;

	timer_bar->SetSize({ (float)timerNum, 50.0f });
}

void IntervalTimer::Draw()
{
	timer_bar->Draw();
	timer->Draw();
}
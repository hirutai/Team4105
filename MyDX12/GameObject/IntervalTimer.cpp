#include "IntervalTimer.h" 
#include "../2D/Sprite.h"

using namespace XIIlib;

void IntervalTimer::Initialize(const int& timerTexNum, const int& barTexNum)
{
	timer = Sprite::Create(timerTexNum, { 10.0f, 100.0f }); // �^�C�}�[�摜�̍쐬
	timer_bar = Sprite::Create(barTexNum, { 20.0f, 150.0f }); // �^�C�}�[�o�[�摜�̍쐬
}

void IntervalTimer::Timer()
{
	if (timerNum == 0) // �^�C�}�[���O�ɂȂ�����
	{
		timerNum = intCountingNum; // �^�C�}�[�����Z�b�g
	}

	timerNum--;

	timer_bar->SetSize({ (float)timerNum, 50.0f });
}

void IntervalTimer::Draw()
{
	timer_bar->Draw();
	timer->Draw();
}
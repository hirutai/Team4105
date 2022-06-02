#include "IntervalTimer.h"
#include "../Tool/DigitalNumberText.h"
using namespace XIIlib;
void IntervalTimer::Initialize(int textureNumber)
{
	timerNum = DigitalNumberText::GetInstance();
	timerNum->Initialize(textureNumber);
}

void IntervalTimer::Timer()
{
	if (internalCounter <= ENDCOUNTER) // �����J�E���g���I��������
	{
		internalCounter = INITIALCOUNTER;
		displayCounter = MAXNUM;
	}

	internalCounter--; // �����J�E���g�X�^�[�g

	if (internalCounter <= INITIALCOUNTER - 120) // �O�`�U�O
	{
		displayCounter = DISPLAYZERO; // �O��\��
	}
	else if (internalCounter > INITIALCOUNTER - 120 && internalCounter <= INITIALCOUNTER - 60) // �U�P�`�P�Q�O
	{
		displayCounter = DISPLAYONE; // �P��\��
	}
	else if (internalCounter > INITIALCOUNTER - 60 && internalCounter < INITIALCOUNTER) // �P�Q�P�`�P�W�O
	{
		displayCounter = DISPLAYTWO; // �Q��\��
	}

	timerNum->Print(displayCounter, POSX, POSY, SCALE);
}

void IntervalTimer::Draw()
{
	timerNum->DrawAll();
}

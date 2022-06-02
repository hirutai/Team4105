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
	if (internalCounter <= ENDCOUNTER) // “à•”ƒJƒEƒ“ƒg‚ªI—¹‚µ‚½Žž
	{
		internalCounter = INITIALCOUNTER;
		displayCounter = MAXNUM;
	}

	internalCounter--; // “à•”ƒJƒEƒ“ƒgƒXƒ^[ƒg

	if (internalCounter <= INITIALCOUNTER - 120) // ‚O`‚U‚O
	{
		displayCounter = DISPLAYZERO; // ‚O‚ð•\Ž¦
	}
	else if (internalCounter > INITIALCOUNTER - 120 && internalCounter <= INITIALCOUNTER - 60) // ‚U‚P`‚P‚Q‚O
	{
		displayCounter = DISPLAYONE; // ‚P‚ð•\Ž¦
	}
	else if (internalCounter > INITIALCOUNTER - 60 && internalCounter < INITIALCOUNTER) // ‚P‚Q‚P`‚P‚W‚O
	{
		displayCounter = DISPLAYTWO; // ‚Q‚ð•\Ž¦
	}

	timerNum->Print(displayCounter, POSX, POSY, SCALE);
}

void IntervalTimer::Draw()
{
	timerNum->DrawAll();
}

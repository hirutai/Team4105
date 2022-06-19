#include "Audio.h"
#include "Sound.h"

XIIlib::Audio::Audio()
{
	// Audio‚Ì‰Šú‰»
	se = new Sound();
	se->Initialize();

	bgm = new Sound();
	bgm->Initialize();
}

XIIlib::Audio::~Audio()
{
	delete bgm;
	delete se;
}

void XIIlib::Audio::PlaySE(std::string wav,float volume)
{
	std::string result = "Resources/SE/" + wav;
	se->LoadWavFile(result.c_str());
	se->PlayWave(0, volume);
}

void XIIlib::Audio::PlayBGM(std::string wav)
{
	std::string result = "Resources/BGM/" + wav;
	bgm->LoadWavFile(result.c_str());
	bgm->PlayWave(255, bgm_volume);
}

void XIIlib::Audio::StopBGM()
{
	bgm->StopWave();
}

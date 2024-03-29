#pragma once
#include <string>
class Sound;

namespace XIIlib {
	class Audio {
	private:
		// SE�̉���
		float se_volume = 0.05f;
		// BGM�̉���
		float bgm_volume = 0.05f;

		// SE
		Sound* se = nullptr;
		// BGM
		Sound* bgm = nullptr;
	public:
		Audio();
		~Audio();

		void PlaySE(std::string wav,float volume = 0.05f);
		void PlayBGM(std::string wav);

		void StopBGM();
		void StopSE();
	};
}
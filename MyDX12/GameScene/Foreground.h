#pragma once
#include <vector>

class Sprite;

namespace XIIlib {

	class Foreground {
	private:
		// �e�N�X�`���֘A
		std::vector<Sprite*> foreground_tex;
		std::vector<std::vector<unsigned int>> foreground;
		std::vector<float> time_foreground;
		// ���Ԑ���p
		std::vector<float> time_curtain;

	public:
		Foreground();
		~Foreground();

		void Initialize();
		unsigned int ChangeFront(unsigned int _change);

		void Draw();

	};
}
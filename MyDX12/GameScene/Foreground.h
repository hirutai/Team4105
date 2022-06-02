#pragma once
#include <vector>

class Sprite;

namespace XIIlib {

	class Foreground {
	private:
		// テクスチャ関連
		std::vector<Sprite*> foreground_tex;
		std::vector<std::vector<unsigned int>> foreground;
		std::vector<float> time_foreground;
		// 時間制御用
		std::vector<float> time_curtain;

	public:
		Foreground();
		~Foreground();

		void Initialize();
		unsigned int ChangeFront(unsigned int _change);

		void Draw();

	};
}
#pragma once
#include <vector>

namespace XIIlib{
	class TexMoveAction;

	class ChainSprite {
	private:
		std::vector<TexMoveAction*> pTexs;

		int activeNum = 0;
	public:
		static ChainSprite* Create();

		ChainSprite();
		~ChainSprite();

		void Update();

		void Draw();

		void AddMoji(float posX, float posY, float finishSize, int texNum);
	};
}
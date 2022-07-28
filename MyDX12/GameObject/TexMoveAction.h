#pragma once
class Sprite;

namespace XIIlib {

	class TexMoveAction {
	private:
		Sprite* tex;
		const float scale_xy = 960.0f;
		const float alpha = 1.0f;
		float finishSize = 120.0f;

		bool isFinish = false;

		float countFrame = 0.0f;

	public:
		static TexMoveAction* Create(float posX, float posY, float finishSize,int texNum);

		TexMoveAction(float posX,float posY, float finishSize, int texNum);
		~TexMoveAction();

		void Initialise();

		void Update();

		void Draw();

		bool GetFinish()const { return isFinish; }
	};

}
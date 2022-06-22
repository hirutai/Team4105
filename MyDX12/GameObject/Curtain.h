#pragma once
class Sprite;

namespace XIIlib {
	enum TexState{
		OPEN = 0,
		ACTIVE,
		CLOSE
	};

	class Curtain {
	private:
		TexState state = OPEN;
		float time = 0.0f;
		Sprite* tex = nullptr;
		const float maxTime = 0.25f;
		const float texSize = 128.0f;

	public:
		static Curtain* Create(unsigned int textureNum, float x, float y);

		Curtain(unsigned int textureNum,float x,float y);
		~Curtain();

		void Initialize();// èâä˙âª
		void Draw(); // ï`âÊ

		void AddFrame();
		void ResetTime();
		void Open();
		void Close();
		void SetState(const TexState& cState);
		void SetSize(float x, float y);
		void SetAlpha(float a);

		TexState GetState()const { return state;}
		float GetTime()const { return time; }
	};

}
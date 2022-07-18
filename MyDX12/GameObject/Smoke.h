#pragma once
#include "../Struct/Math.h"

namespace XIIlib {

	class BillObj;

	// ���P�u���b�N���̃N���X
	class Smoke {
	private:// �ϐ�
		float addColor = 0.0f;
		float addScale = 0.0f;
		Math::Vector3 addV;

		BillObj* bill = nullptr;

	private:// �֐�
		void Initialize(float c,float s,const Math::Vector3& v, const Math::Vector3& pos);

	public:
		static Smoke* Create(float c, float s, const Math::Vector3& v, const Math::Vector3& pos);

		Smoke();
		~Smoke();

		void Update();

		void Draw();

		float GetAlpha()const;
	};

}
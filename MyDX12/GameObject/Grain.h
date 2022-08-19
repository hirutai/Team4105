#pragma once
#include "../Struct/Math.h"

namespace XIIlib {

	class BillObj;

	class Grain {
	private:
		float addColor = 0.0f;
		float addScale = 0.0f;
		Math::Vector3 addV;

		BillObj* bill = nullptr;
	private:
		void Initialize(const Math::Vector3& v, const Math::Vector3& pos);

	public:
		static Grain* Create(const Math::Vector3& v, const Math::Vector3& pos);

		Grain();
		~Grain();

		void Update();

		void Draw();

		float GetAlpha()const;
	};

}
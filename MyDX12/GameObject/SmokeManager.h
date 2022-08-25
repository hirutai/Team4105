#pragma once
#include <vector>
#include "../Struct/Math.h"

namespace XIIlib {

	class InstBill;

	class SmokeManager {
	public:
		struct DataAset {
			Math::Vector3 position;
			Math::Vector3 vec;
			float addScale, scale,addColor,color,a;
		};
	private:
		std::vector<DataAset> vcon;

		InstBill* iBill = nullptr;
	public:
		static SmokeManager* Create();

		SmokeManager();
		~SmokeManager();

		void Initialize();
		void Add(float c, float s, const Math::Vector3& v, const Math::Vector3& pos);
		void Update();
		void Draw();

		void AllClear();
	};

}
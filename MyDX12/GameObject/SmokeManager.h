#pragma once
#include <vector>
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	class Smoke;

	class SmokeManager {
	private:
		std::vector<std::unique_ptr<Smoke>> vcon;
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
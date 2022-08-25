#pragma once
#include <vector>
#include "../Struct/Math.h"

namespace XIIlib {

	class InstBill;

	class GrainManager {
	public:
		struct DataAset {
			Math::Vector3 position;
			Math::Vector3 vec;
			float addScale, scale, r, g, b, a;
		};
	private:
		std::vector<DataAset> vcon;

		InstBill* iBill = nullptr;
	public:
		static GrainManager* Create();

		GrainManager();
		~GrainManager();

		void Initialize();
		void Add(const Math::Vector3& v, const Math::Vector3& pos);
		void Update();
		void Draw();

		void AllClear();
	};

}
#pragma once
#include <vector>
#include <memory>
#include "../Struct/Math.h"

namespace XIIlib {

	class Grain;

	class GrainManager {
	private:
		std::vector<std::unique_ptr<Grain>> vcon;
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
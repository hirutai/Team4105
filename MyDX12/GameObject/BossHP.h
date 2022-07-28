#pragma once
#include <memory>

namespace XIIlib {
	class HPBar;
	class BossHP  {
	private:
		BossHP();
		~BossHP();
		const int defaultHp = 1;
		float bossHP = 1.0f;

		bool nodamage = false;
		int nodamageTime = 0;
		int defaultNodamageTime = 11;

	public:
		BossHP(const BossHP& obj) = delete;
		BossHP& operator = (const BossHP& obj) = delete;
		static BossHP* GetInstance();

		int GetBossHP() { return bossHP; }
		void SetBossHP(float HP) { bossHP = HP; }

		void Initialize(); // 初期化
		void Update(); 

		void Damage();
	};
}
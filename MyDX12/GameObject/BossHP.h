#pragma once
#include <memory>

namespace XIIlib {
	class HPBar;
	class BossHP  {
	private:
		BossHP();
		~BossHP();
		const int defaultHP = 30;
		int bossHP = 30;

		bool nodamage = false;
		int nodamageTime = 0;
		int defaultNodamageTime = 11;

	public:
		BossHP(const BossHP& obj) = delete;
		BossHP& operator = (const BossHP& obj) = delete;
		static BossHP* GetInstance();

		int GetBossHP() { return bossHP; }
		void SetBossHP(int HP) { bossHP = HP; }

		int GetDefaultBossHP() { return defaultHP; }

		void Initialize(); // 初期化
		void Update(); 

		void Damage();
	};
}
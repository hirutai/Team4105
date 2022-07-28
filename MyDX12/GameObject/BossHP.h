#pragma once
#include <memory>

namespace XIIlib {
	class HPBar;
	class BossHP  {
	private:
		BossHP();
		~BossHP();
		const int defaultHp = 30;
		float bossHP = 30.0f;

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
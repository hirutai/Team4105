#pragma once
#include <memory>

namespace XIIlib {
	class BossHP  {
	private:
		BossHP();
		~BossHP();
		const int defaultHp = 50;
		int bossHP = 50;

		bool nodamage = false;
		int nodamageTime = 0;
		int defaultNodamageTime = 11;

	public:
		BossHP(const BossHP& obj) = delete;
		BossHP& operator = (const BossHP& obj) = delete;
		static BossHP* GetInstance();

		int GetBossHP() { return bossHP; }
		void SetBossHP(int HP) { bossHP = HP; }

		void Initialize(); // èâä˙âª
		void Update(); 

		void Damage();
	};
}
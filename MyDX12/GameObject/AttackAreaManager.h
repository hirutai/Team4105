#pragma once
#include"../Struct/Point2.h"


namespace XIIlib {
    class AttackAreaManager
    {
    private:
        AttackAreaManager();
        ~AttackAreaManager();

        int attackAreas[8][8];

    public:
        AttackAreaManager(const AttackAreaManager& obj) = delete;
        AttackAreaManager& operator = (const AttackAreaManager& obj) = delete;

        static AttackAreaManager* GetInstance();

        void Initialize();
        void Draw();
        void SetAttackAreas(Math::Point2 attackArea);
    };
}

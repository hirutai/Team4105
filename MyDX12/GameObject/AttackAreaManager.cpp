#include "AttackAreaManager.h"
#include"UnitManager.h"


XIIlib::AttackAreaManager::AttackAreaManager() {}

XIIlib::AttackAreaManager::~AttackAreaManager()
{

}
XIIlib::AttackAreaManager* XIIlib::AttackAreaManager::GetInstance()
{
	static XIIlib::AttackAreaManager instance;
	return &instance;
}

void XIIlib::AttackAreaManager::Initialize()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			attackAreas[i][j] = 0;
		}
	}
}

void XIIlib::AttackAreaManager::Draw()
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++) {
			if(attackAreas[i][j] ==1)
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i,j), 3);
			else if(attackAreas[i][j] == 2)
			{
				UnitManager::GetInstance()->ChangeAttackValidTile(Math::Point2(i, j), 2);
			}
		}
	}
	Initialize();
}

void XIIlib::AttackAreaManager::SetAttackAreas(Math::Point2 attackArea)
{
	attackAreas[attackArea.a][attackArea.b]++;
	if (attackAreas[attackArea.a][attackArea.b] >= 2)attackAreas[attackArea.a][attackArea.b] = 2;
}

#include"BossHP.h"
#include "HPBar.h"

XIIlib::BossHP::BossHP() {}

XIIlib::BossHP::~BossHP(){}

XIIlib::BossHP* XIIlib::BossHP::GetInstance()
{
	static XIIlib::BossHP instance;
	return &instance;
}

void XIIlib::BossHP::Initialize()
{
	bossHP = defaultHp;
}

void XIIlib::BossHP::Update()
{
	if (nodamage)
	{
		nodamageTime--;
	}
	if (nodamageTime <= 0)
	{
		nodamage = false;
	}
}

void XIIlib::BossHP::Damage()
{
	if (!nodamage)
	{
		bossHP--;
		HPBar::GetInstance()->Timer(2);
		nodamageTime = defaultNodamageTime;
		nodamage = true;
	}
}
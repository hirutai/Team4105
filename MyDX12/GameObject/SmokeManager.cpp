#include "SmokeManager.h"
#include "Smoke.h"
#include <algorithm>

XIIlib::SmokeManager::SmokeManager() {}

XIIlib::SmokeManager::~SmokeManager()
{
}

XIIlib::SmokeManager* XIIlib::SmokeManager::Create()
{
	SmokeManager* pSmokeManager = new SmokeManager();
	if (pSmokeManager == nullptr) {
		return nullptr;
	}
	pSmokeManager->Initialize();
	return pSmokeManager;
}

void XIIlib::SmokeManager::Initialize()
{
	vcon.reserve(100);
}

void XIIlib::SmokeManager::Add(float c, float s, const Math::Vector3& v, const Math::Vector3& pos)
{
	std::unique_ptr<Smoke> uSmoke;
	uSmoke.reset(Smoke::Create(c,s,v,pos));

	vcon.push_back(std::move(uSmoke));
}

void XIIlib::SmokeManager::Update()
{
	if (vcon.size() == 0)return;
	for (auto& u : vcon) {
		u->Update();
	}

	// アルファ値が0以下の物を排除
	auto removeIt = std::remove_if(vcon.begin(), vcon.end(), [&](std::unique_ptr<Smoke>& itr)
		{
			return itr->GetAlpha() <= 0.0f;
		}
	);

	vcon.erase(removeIt, vcon.end());
}

void XIIlib::SmokeManager::Draw()
{
	if (vcon.size() == 0)return;

	for (auto& u : vcon) {
		u->Draw();
	}
}

void XIIlib::SmokeManager::AllClear()
{
	if (vcon.size() == 0)return;
	vcon.clear();
}

#include "GrainManager.h"
#include "Grain.h"
#include <algorithm>

XIIlib::GrainManager::GrainManager() {}

XIIlib::GrainManager::~GrainManager()
{
}

XIIlib::GrainManager* XIIlib::GrainManager::Create()
{
	GrainManager* pGrainManager = new GrainManager();
	if (pGrainManager == nullptr) {
		return nullptr;
	}
	pGrainManager->Initialize();
	return pGrainManager;
}

void XIIlib::GrainManager::Initialize()
{
	vcon.reserve(100);
}

void XIIlib::GrainManager::Add(const Math::Vector3& v, const Math::Vector3& pos)
{
	std::unique_ptr<Grain> uSmoke;
	float rV = static_cast<float>(rand() % 10 + 1)/10.0f;
	uSmoke.reset(Grain::Create(v * rV, pos));

	vcon.push_back(std::move(uSmoke));
}

void XIIlib::GrainManager::Update()
{
	if (vcon.size() == 0)return;
	for (auto& u : vcon) {
		u->Update();
	}

	// アルファ値が0以下の物を排除
	auto removeIt = std::remove_if(vcon.begin(), vcon.end(), [&](std::unique_ptr<Grain>& itr)
		{
			return itr->GetAlpha() <= 0.0f;
		}
	);

	vcon.erase(removeIt, vcon.end());
}

void XIIlib::GrainManager::Draw()
{
	if (vcon.size() == 0)return;

	for (auto& u : vcon) {
		u->Draw();
	}
}

void XIIlib::GrainManager::AllClear()
{
	if (vcon.size() == 0)return;
	vcon.clear();
}

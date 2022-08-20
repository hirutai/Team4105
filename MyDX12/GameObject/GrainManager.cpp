#include "GrainManager.h"
#include "../3D/InstBill.h"
#include <algorithm>

XIIlib::GrainManager::GrainManager() {}

XIIlib::GrainManager::~GrainManager()
{
	delete iBill;
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
	vcon.reserve(sizeof(DataAset) * 100);
	iBill = InstBill::Create("");
}

void XIIlib::GrainManager::Add(const Math::Vector3& v, const Math::Vector3& pos)
{
	float rV = static_cast<float>(rand() % 10 + 1)/10.0f;
	float addRand = static_cast<float>(rand() % 5 + 1) / 100.0f;
	DataAset data = {pos,v * rV,
		addRand,	// addScale
		0.1f,		// scale
		25.5f,0.0f,0.0f,255.0f	// color
	};

	vcon.push_back(data);
}

void XIIlib::GrainManager::Update()
{
	if (vcon.size() == 0)return;
	for (auto& u : vcon) {
		u.position += u.vec;
		u.scale += u.addScale;
		u.r += 15.3f;
		u.g += 1.275f;
		u.b += 0.6375f;
		u.a += -2.55f;
	}

	// アルファ値が0以下の物を排除
	auto removeIt = std::remove_if(vcon.begin(), vcon.end(), [](DataAset& itr)
		{
			return itr.a <= 25.5f * 5.0f;
		}
	);

	vcon.erase(removeIt, vcon.end());

	for (auto& u : vcon) {
		iBill->DrawBillBox(u.position, u.scale, u.r, u.g, u.b, u.a);
	}
}

void XIIlib::GrainManager::Draw()
{
	iBill->Update();

	InstBill::PreDraw();
	iBill->Draw();
	InstBill::PostDraw();
}

void XIIlib::GrainManager::AllClear()
{
	if (vcon.size() == 0)return;
	vcon.clear();
}

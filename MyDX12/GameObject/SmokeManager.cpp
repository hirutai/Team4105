#include "SmokeManager.h"
#include "../3D/InstBill.h"
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
	vcon.reserve(sizeof(DataAset) * 10);
	iBill = InstBill::Create("");
}

void XIIlib::SmokeManager::Add(float c, float s, const Math::Vector3& v, const Math::Vector3& pos)
{
	DataAset data = { pos,v,
		s,	// addScale
		0.1f,		// scale
		c * 255.0f,25.5f,255.0f	// color
	};

	vcon.push_back(data);
}

void XIIlib::SmokeManager::Update()
{
	if (vcon.size() == 0)return;
	for (auto& u : vcon) {
		u.position += u.vec;
		u.scale += u.addScale;
		u.color += u.addColor;
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
		iBill->DrawBillBox(u.position, u.scale, u.color, u.color, u.color, u.a);
	}
}

void XIIlib::SmokeManager::Draw()
{
	iBill->Update();

	InstBill::PreDraw();
	iBill->Draw();
	InstBill::PostDraw();
}

void XIIlib::SmokeManager::AllClear()
{
	if (vcon.size() == 0)return;
	vcon.clear();
}

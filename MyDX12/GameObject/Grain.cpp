#include "Grain.h"

#include "../3D/BillObj.h"

XIIlib::Grain::Grain() {}

XIIlib::Grain::~Grain()
{
	delete bill;
}

void XIIlib::Grain::Initialize(const Math::Vector3& v, const Math::Vector3& pos) {
	// ‰Šú’l‚Ìİ’è
	// scale = (0.06 <= s <= 0.15)
	float addRand = static_cast<float>(rand() % 5 + 1)/100.0f;
	addScale = addRand;
	addColor = 0.06f;
	addV = v;

	bill = BillObj::Create(pos, "");
	bill->SetSize({ 1.0f, 1.0f });
	bill->SetScale(0.1f, 0.1f, 0.1f);
	bill->SetColor(0.1f, 0,0, 1);
}

XIIlib::Grain* XIIlib::Grain::Create(const Math::Vector3& v, const Math::Vector3& pos)
{
	Grain* pGrain = new Grain();

	if (pGrain == nullptr) {
		return nullptr;
	}
	pGrain->Initialize(v, pos);

	return pGrain;
}

void XIIlib::Grain::Update()
{
	bill->AddPosition(addV);
	bill->AddScale(addScale, addScale, addScale);
	const float subAlpha = -0.01f;
	bill->AddColor(addColor,0.005f, 0.0025f, subAlpha);
}

void XIIlib::Grain::Draw()
{
	bill->Draw();
}

float XIIlib::Grain::GetAlpha() const
{
	return bill->GetAlpha();
}

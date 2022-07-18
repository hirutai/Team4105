#include "Smoke.h"
#include "../3D/BillObj.h"

XIIlib::Smoke::Smoke() {}

XIIlib::Smoke::~Smoke()
{
	delete bill;
}

void XIIlib::Smoke::Initialize(float c, float s, const Math::Vector3& v,const Math::Vector3& pos){
	// ‰Šú’l‚ÌÝ’è
	addColor = c;
	addScale = s;
	addV = v;

	bill = BillObj::Create(pos,"");
	bill->SetSize({ 1.0f, 1.0f });
	bill->SetScale(0.1f, 0.1f, 0.1f);
	bill->SetColor(0.1f, 0.1f, 0.1f, 1);
}

XIIlib::Smoke* XIIlib::Smoke::Create(float c, float s, const Math::Vector3& v, const Math::Vector3& pos)
{
	Smoke* pSmoke = new Smoke();

	if (pSmoke == nullptr) {
		return nullptr;
	}
	pSmoke->Initialize(c,s,v,pos);

	return pSmoke;
}

void XIIlib::Smoke::Update()
{
	bill->AddPosition(addV);
	bill->AddScale(addScale,addScale,addScale);
	const float subAlpha = -0.01f;
	bill->AddColor(addColor, addColor,addColor, subAlpha);
}

void XIIlib::Smoke::Draw()
{
	bill->Draw();
}

float XIIlib::Smoke::GetAlpha() const
{
	return bill->GetAlpha();
}

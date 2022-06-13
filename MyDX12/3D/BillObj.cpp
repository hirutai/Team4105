#include "BillObj.h"
#include "../Pipeline/Pipeline.h"

bool XIIlib::BillObj::StaticInitialize()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline();
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "NORMAL",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	// シェーダー新たに作れよ自分
	//plData->AddShader(XIILib::VS, "OBJVertexShader.hlsl");
	//plData->AddShader(XIILib::PS, "OBJPixelShader.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);

	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);
	plData->AddRootParam(2, XIILib::CBV);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
	return false;
}

bool XIIlib::BillObj::LoadTexture(UINT texnumber, const wchar_t* filename)
{
	return false;
}

void XIIlib::BillObj::PreDraw()
{
}

void XIIlib::BillObj::PostDraw()
{
}

XIIlib::BillObj* XIIlib::BillObj::Create()
{
	return nullptr;
}

XIIlib::BillObj::BillObj()
{
}

bool XIIlib::BillObj::Initialize()
{
	return false;
}

void XIIlib::BillObj::SetRotation(float rotation)
{
}

void XIIlib::BillObj::SetPosition(Math::Vector3 position)
{
}

void XIIlib::BillObj::SetSize(Math::Vector2 size)
{
}

void XIIlib::BillObj::SetAnchorPoint(Math::Vector2 anchorpoint)
{
}

void XIIlib::BillObj::SetColor(float r, float g, float b, float a)
{
}

void XIIlib::BillObj::SetIsFlipX(bool isFlipX)
{
}

void XIIlib::BillObj::SetIsFlipY(bool isFlipY)
{
}

void XIIlib::BillObj::SetTextureRect(Math::Vector2 texBase, Math::Vector2 texSize)
{
}

void XIIlib::BillObj::Draw()
{
}

void XIIlib::BillObj::TransferVertices()
{
}

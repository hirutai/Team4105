#include "Circle.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Math;

ID3D12Device* Circle::device = nullptr;
ID3D12GraphicsCommandList* Circle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Circle::rootSignature;
ComPtr<ID3D12PipelineState> Circle::pipelineState;
DirectX::XMMATRIX Circle::matProjection;
ComPtr<ID3D12DescriptorHeap> Circle::descHeap;

bool Circle::StaticInitialize()
{
	Circle::device = DirectX12::GetDevice();

	CreateGraphicsPipelineState();

	// 射影行列計算
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, WindowsApp::window_width,
		WindowsApp::window_height, 0.0f,
		0.0f, 1.0f);

	HRESULT result;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));

	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

void Circle::PreDraw()
{
	assert(Circle::cmdList == nullptr);
	// コマンドリストをセット
	Circle::cmdList = DirectX12::GetCmdList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状を設定D3D_PRIMITIVE_TOPOLOGY_POINT
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void Circle::PostDraw()
{
	// コマンドリストを解除
	Circle::cmdList = nullptr;
}

Circle* Circle::Create()
{
	Circle* line = new Circle();

	if (line == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!line->Initialize()) {
		delete line;
		assert(0);
		return nullptr;
	}

	return line;
}

// 静的メンバ変数の実体
Circle::Circle()
{
	device = DirectX12::GetDevice();
}

void Circle::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1, XIILib::CULL_NONE, XIILib::MASK_DEBUG, XIILib::COMP_ALWAYS);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });

	plData->AddShader(XIILib::VS, "PointVertexShader.hlsl");
	plData->AddShader(XIILib::PS, "PointPixelShader.hlsl");

	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void Circle::CreateVertexBuffer()
{
	HRESULT result;
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosUv) * (vertNum + 1));

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * (vertNum * 3));
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(VertexPosUv);

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Circle::CreateConstBuffer()
{
	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CBVIDParameter) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	device->CreateConstantBufferView(&cbvDesc, descHeap->GetCPUDescriptorHandleForHeapStart());
}

bool Circle::Initialize()
{
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void Circle::DrawCircle(float x, float y, float rad, float r, float g, float b, float a)
{
	datas.push_back({ { x,y }, { Math::Transform(255, 1,r),Math::Transform(255, 1,g),Math::Transform(255, 1,b),Math::Transform(255, 1,a) },rad });
}

void Circle::Draw()
{
	TransferCBVInstance();

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	// インデックスバッファの設定
	cmdList->IASetIndexBuffer(&this->ibView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)vertNum * 3, datas.size(), 0, 0, 0);
	ClearCircle();
}

void Circle::TransferVertices()
{
	HRESULT result;

	// 頂点データ
	VertexPosUv vertices[vertNum + 1];
	float rad = 360.0f/vertNum;

	for (int i = 0; i < vertNum; i++)
	{
		vertices[i].pos = {cos(Math::ConvertToRadians(rad * i)),sin(Math::ConvertToRadians(rad * i)) ,0.0f};
	}
	vertices[vertNum].pos = { 0,0,0 };

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	unsigned short indices[vertNum * 3];
	for (int i = 0; i < vertNum * 3; i += 3)
	{
		if (i == 0)
		{
			indices[i] = 0;
			indices[i + 1] = 1;
		}
		else
		{
			indices[i] = indices[i - 3] + 1;
			indices[i + 1] = indices[i - 2] + 1;
		}

		indices[i + 2] = vertNum;
	}
	indices[vertNum * 3 - 2] = 0;

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		memcpy(indexMap, indices, sizeof(indices));
		indexBuff->Unmap(0, nullptr);
	}
}

void Circle::TransferCBVInstance()
{
	// 定数バッファにデータ転送
	CBVIDParameter* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		for (int i = 0; i < dataIndexCount; i++)
		{
			// ワールド行列の更新
			XMMATRIX matScale, matTrans;
			this->matWorld = XMMatrixIdentity();
			XMFLOAT4 colorMap = { 0,0,0,0 };

			if (datas.size() >= i + 1)
			{
				matScale = XMMatrixScaling(datas[i].radian, datas[i].radian, 0.0f);
				matTrans = XMMatrixTranslation(datas[i].position.x, datas[i].position.y, 0.0f);
				this->matWorld *= matScale;
				this->matWorld *= matTrans;
				colorMap = datas[i].color;
				constMap->data[i].active = 1;
			}
			else if (datas.size() < i + 1)
			{
				constMap->data[i].active = 0;
			}

			constMap->data[i].mat = this->matWorld * matProjection;
			constMap->data[i].color = colorMap;
		}
		this->constBuff->Unmap(0, nullptr);
	}
}
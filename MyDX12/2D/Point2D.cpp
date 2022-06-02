#include "Point2D.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Math;

ID3D12Device* Point2D::device = nullptr;
ID3D12GraphicsCommandList* Point2D::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Point2D::rootSignature;
ComPtr<ID3D12PipelineState> Point2D::pipelineState;
DirectX::XMMATRIX Point2D::matProjection;
ComPtr<ID3D12DescriptorHeap> Point2D::descHeap;

bool Point2D::StaticInitialize()
{
	Point2D::device = DirectX12::GetDevice();

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

void Point2D::PreDraw()
{
	assert(Point2D::cmdList == nullptr);
	// コマンドリストをセット
	Point2D::cmdList = DirectX12::GetCmdList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状を設定D3D_PRIMITIVE_TOPOLOGY_POINT
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
}

void Point2D::PostDraw()
{


	// コマンドリストを解除
	Point2D::cmdList = nullptr;
}

Point2D* Point2D::Create()
{
	Point2D* line = new Point2D();

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
Point2D::Point2D()
	: point({0,0,0})
{
	device = DirectX12::GetDevice();
}

void Point2D::CreateGraphicsPipelineState()
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

void Point2D::CreateVertexBuffer()
{
	HRESULT result;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * vertNum;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void Point2D::CreateConstBuffer()
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

bool Point2D::Initialize()
{
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void Point2D::Draw()
{
	TransferCBVInstance();

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// 描画コマンド
	cmdList->DrawInstanced(1, pDatas.size(), 0, 0);
}

void Point2D::TransferVertices()
{
	HRESULT result;

	// 頂点データ
	VertexPosUv vertices[vertNum] = {
		point, // 点
	};

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void Point2D::TransferCBVInstance()
{
	// 定数バッファにデータ転送
	CBVIDParameter* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		for (int i = 0; i < dataIndexCount; i++)
		{
			// ワールド行列の更新
			this->matWorld = XMMatrixIdentity();
			XMFLOAT4 colorMap = { 0,0,0,0 };
			
			if (pDatas.size() >= i + 1)
			{
				this->matWorld *= XMMatrixTranslation(pDatas[i].position.x, pDatas[i].position.y, 0.0f);
				colorMap = pDatas[i].color;
				constMap->data[i].active = 1;
			}
			else if (pDatas.size() < i + 1)
			{
				constMap->data[i].active = 0;
			}

			constMap->data[i].mat = this->matWorld * matProjection;
			constMap->data[i].color = colorMap;
		}
		this->constBuff->Unmap(0, nullptr);
	}
}

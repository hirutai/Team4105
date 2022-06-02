#include "CollBox.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* CollisionBox::device = nullptr;
ID3D12GraphicsCommandList* CollisionBox::cmdList = nullptr;
ComPtr<ID3D12RootSignature> CollisionBox::rootSignature;
ComPtr<ID3D12PipelineState> CollisionBox::pipelineState;
XMMATRIX CollisionBox::matProjection;
ComPtr<ID3D12DescriptorHeap> CollisionBox::descHeap;
DebugCamera* CollisionBox::d_camera = nullptr;

bool CollisionBox::StaticInitialize()
{
	CollisionBox::device = DirectX12::GetDevice();
	CollisionBox::d_camera = d_camera;

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

void CollisionBox::PreDraw()
{
	assert(CollisionBox::cmdList == nullptr);
	// コマンドリストをセット
	CollisionBox::cmdList = DirectX12::GetCmdList();

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CollisionBox::PostDraw()
{
	// コマンドリストを解除
	CollisionBox::cmdList = nullptr;
}

CollisionBox* CollisionBox::Create(const Math::Vector3& start, const Math::Vector3& end)
{
	CollisionBox* collisionbox = new CollisionBox(start, end);

	if (collisionbox == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!collisionbox->Initialize()) {
		delete collisionbox;
		assert(0);
		return nullptr;
	}

	return collisionbox;
}

// 静的メンバ変数の実体
CollisionBox::CollisionBox(const Math::Vector3& start, const Math::Vector3& end)
	:start(start), end(end)
{
	position = { 0,0 ,0 };
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	device = DirectX12::GetDevice();
}

void CollisionBox::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });

	plData->AddShader(XIILib::VS, "CapsuleVS.hlsl");
	plData->AddShader(XIILib::PS, "CapsulePS.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void CollisionBox::CreateVertexBuffer()
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

void CollisionBox::CreateConstBuffer()
{
	HRESULT result;

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CbData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	device->CreateConstantBufferView(&cbvDesc, descHeap->GetCPUDescriptorHandleForHeapStart());
}

bool CollisionBox::Initialize()
{
	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateIndexBuffer();

	CreateConstBuffer();

	return true;
}

void CollisionBox::Update()
{
	assert(d_camera);

	XMMATRIX matScl,matRot,matTrans;
	matScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScl;
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	const XMMATRIX& matViewProjection = d_camera->GetMatViewProjection();

	// 定数バッファにデータ転送
	CbData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->mat = matWorld;
		constMap->viewproj = matViewProjection;
		constMap->color = color;
		constBuff->Unmap(0, nullptr);
	}
}

void CollisionBox::Draw()
{
	Update();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);
	// インデックスの要素数を取得
	const int index_count = sizeof(unsigned short) * (vertNum / 2) * 3;
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)index_count, 1, 0, 0, 0);
}

namespace
{
	Math::Vector3 SetVertex(const Math::Vector3& o, const Math::Vector3& v)
	{
		Math::Vector3 verticle = { o + v };
		return verticle;
	}
}

void CollisionBox::TransferVertices()
{
	HRESULT result;
	//     p5--------p7
	//   ／|       ／|
	// p1--------p3  |
	// |   |     |   |
	// |   p4----|---p6
	// | ／      | ／
	// p0--------p2
	Math::Vector3 p0 = start;	// min
	Math::Vector3 p1 = Math::Vector3(start.x,end.y,  start.z);
	Math::Vector3 p2 = Math::Vector3(end.x,  start.y,start.z);
	Math::Vector3 p3 = Math::Vector3(end.x, end.y,start.z);
	Math::Vector3 p4 = Math::Vector3(start.x,start.y,end.z);
	Math::Vector3 p5 = Math::Vector3(start.x,end.y,  end.z);
	Math::Vector3 p6 = Math::Vector3(end.x,  start.y,end.z);
	Math::Vector3 p7 = end;		// max

	// p0,p1,p2,p3 前
	// p3,p7,p2,p6 右
	// p6,p7,p4,p5 後
	// p5,p1,p4,p0 左
	// p0,p2,p4,p6 下
	// p1,p3,p5,p7 上

	std::vector<VertexPosUv> vertices;
	vertices.reserve(vertNum);
	// 前
	vertices.push_back({ p0 });//0
	vertices.push_back({ p1 });//1
	vertices.push_back({ p2 });//2
	vertices.push_back({ p3 });//3
	// 後
	vertices.push_back({ p4 });//4
	vertices.push_back({ p5 });//5
	vertices.push_back({ p6 });//6
	vertices.push_back({ p7 });//7
	// 左
	vertices.push_back({ p4 });//8
	vertices.push_back({ p5 });//9
	vertices.push_back({ p0 });//10
	vertices.push_back({ p1 });//11
	// 右
	vertices.push_back({ p2 });//12
	vertices.push_back({ p3 });//13
	vertices.push_back({ p6 });//14
	vertices.push_back({ p7 });//15
	// 上
	vertices.push_back({ p1 });//16
	vertices.push_back({ p5 });//17
	vertices.push_back({ p3 });//18
	vertices.push_back({ p7 });//19
	// 下
	vertices.push_back({ p4 });//20
	vertices.push_back({ p0 });//21
	vertices.push_back({ p6 });//22
	vertices.push_back({ p2 });//23
	// 計24個

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void CollisionBox::CreateIndexBuffer()
{
	HRESULT result;
	// インデックスの要素数を取得
	const int index_count = sizeof(unsigned short) * (vertNum / 2) * 3;

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * index_count),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	std::vector<unsigned short> indices;
	indices.reserve(index_count);
	// インデックスの個々の要素を格納(LIST)
	indices.push_back(1); indices.push_back(2); indices.push_back(0);
	indices.push_back(1); indices.push_back(3); indices.push_back(2);

	indices.push_back(7); indices.push_back(4); indices.push_back(6);
	indices.push_back(7); indices.push_back(5); indices.push_back(4);

	indices.push_back(9); indices.push_back(10); indices.push_back(8);
	indices.push_back(9); indices.push_back(11); indices.push_back(10);

	indices.push_back(13); indices.push_back(14); indices.push_back(12);
	indices.push_back(13); indices.push_back(15); indices.push_back(14);

	indices.push_back(17); indices.push_back(18); indices.push_back(16);
	indices.push_back(17); indices.push_back(19); indices.push_back(18);

	indices.push_back(21); indices.push_back(22); indices.push_back(20);
	indices.push_back(21); indices.push_back(23); indices.push_back(22);

	// STRIP版(要素数 : 24)
	//indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(3);
	//indices.push_back(13); indices.push_back(15); indices.push_back(12); indices.push_back(14);
	//indices.push_back(6); indices.push_back(7); indices.push_back(4); indices.push_back(5);
	//indices.push_back(9); indices.push_back(11); indices.push_back(8); indices.push_back(10);
	//indices.push_back(21); indices.push_back(23); indices.push_back(20); indices.push_back(22);
	//indices.push_back(16); indices.push_back(18); indices.push_back(17); indices.push_back(19);
	// 0,1,2,3
	// 13,15,12,14
	// 6,7,4,5
	// 9,11,8,10
	// 21,23,20,22
	// 16,18,17,19


	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(unsigned short) * index_count;
}

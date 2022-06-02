#include "CollCapsule.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* CollisionCapsule::device = nullptr;
ID3D12GraphicsCommandList* CollisionCapsule::cmdList = nullptr;
ComPtr<ID3D12RootSignature> CollisionCapsule::rootSignature;
ComPtr<ID3D12PipelineState> CollisionCapsule::pipelineState;
XMMATRIX CollisionCapsule::matProjection;
ComPtr<ID3D12DescriptorHeap> CollisionCapsule::descHeap;
DebugCamera* CollisionCapsule::d_camera = nullptr;

bool CollisionCapsule::StaticInitialize()
{
	CollisionCapsule::device = DirectX12::GetDevice();
	CollisionCapsule::d_camera = d_camera;

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

void CollisionCapsule::PreDraw()
{
	assert(CollisionCapsule::cmdList == nullptr);
	// コマンドリストをセット
	CollisionCapsule::cmdList = DirectX12::GetCmdList();

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CollisionCapsule::PostDraw()
{
	// コマンドリストを解除
	CollisionCapsule::cmdList = nullptr;
}

CollisionCapsule* CollisionCapsule::Create(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices)
{
	CollisionCapsule* collisionCapsule = new CollisionCapsule(start, end,radius,slices,slices);

	if (collisionCapsule == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!collisionCapsule->Initialize()) {
		delete collisionCapsule;
		assert(0);
		return nullptr;
	}

	return collisionCapsule;
}

// 静的メンバ変数の実体
CollisionCapsule::CollisionCapsule(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices, int stacks_1_2)
	:start(start), end(end),radius(radius),slices(slices),stacks_1_2(stacks_1_2)
{
	position = { 0,0 ,0 };
	device = DirectX12::GetDevice();
}

void CollisionCapsule::CreateGraphicsPipelineState()
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

void CollisionCapsule::CreateVertexBuffer()
{
	HRESULT result;

	// 頂点数
	int numberOfVB = (1 + slices * stacks_1_2) * 2;

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * numberOfVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	TransferVertices();

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * numberOfVB;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void CollisionCapsule::CreateConstBuffer()
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

bool CollisionCapsule::Initialize()
{
	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateIndexBuffer();

	CreateConstBuffer();

	return true;
}

void CollisionCapsule::Update()
{
	assert(d_camera);

	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // 変形をリセット
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

void CollisionCapsule::Draw()
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
	// インデックス数
	int numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	int numberOfIB = numberOfTriangle * 3;
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)numberOfIB, 1, 0, 0, 0);
}

namespace
{
	Math::Vector3 SetVertex(const Math::Vector3& o, const Math::Vector3& v)
	{
		Math::Vector3 verticle = { o + v };
		return verticle;
	}
}

void CollisionCapsule::TransferVertices()
{
	HRESULT result;

	// 分割数が無効な値でないかチェック
	if (slices <= 0)assert(0);
	float invSlices = 1.0f / slices;
	if (stacks_1_2 <= 0)assert(0);
	float invStacks = 1.0f / stacks_1_2;
	// 半径が無効な値でないかチェック
	if (radius <= 0.0f)assert(0);

	// カプセルの単位ベクトル作成
	Math::Vector3 uz = end - start;// vz : P2からP1へ向かう向きのベクトル
	float length = uz.dot(uz);// おそらく二乗しなければならない
	if (length == 0.0f)assert(0);

	// 仮置きのベクトル
	Math::Vector3 v = { 1,1,0 };

	Math::Vector3 uy;
	uy = uz.cross(v);
	length = uy.length();
	if (length == 0.0f)assert(0);

	Math::Vector3 ux;
	ux = uz.cross(uy);

	// uxの正規化
	ux = ux.normalize();
    // uxに-半径をかけてuxに格納
    ux = ux * -radius;
    // uyの正規化
	uy = uy.normalize();
    // uyに半径をかけてuxに格納
	uy = uy * radius;
    // uzの正規化
	uz = uz.normalize();
    // uzに半径をかけてuxに格納
	uz = uz * radius;

	//頂点バッファの数を取得
	int numberOfVB = (1 + slices * stacks_1_2) * 2;
	// 頂点バッファの作成
	std::vector<VertexPosUv> vertices;
	vertices.reserve(numberOfVB);

	vertices.push_back({ SetVertex(end, uz) });
	const float PI = 3.14f;
	for (int i = 0; i < stacks_1_2; i++)
	{// PI = π
		float t = (PI * 0.5f) * invSlices * (i + 1);
		float r = sinf(t);
		Math::Vector3 vx = ux * r;
		Math::Vector3 vy = uy * r;
		Math::Vector3 vz = uz * cosf(t);
		for (int j = 0; j < slices; j++)
		{
			float u = (PI * 2.0f) * invSlices * j;
			vertices.push_back({ SetVertex(end, vx * cosf(u) + vy * sinf(u) + vz) });
		}
	}

	//P2側の計算
	for (int i = 0; i < stacks_1_2; i++)
	{// PI = π
		float t = (PI * 0.5f) * invSlices * i;
		float r = cosf(t);
		Math::Vector3 vx = ux * r;
		Math::Vector3 vy = uy * r;
		Math::Vector3 vz = uz * -sinf(t);
		for (int j = 0; j < slices; j++)
		{
			float u = (PI * 2.0f) * invSlices * j;
			vertices.push_back({ SetVertex(start, vx * cosf(u) + vy * sinf(u) + vz) });
		}
	}
	vertices.push_back({ SetVertex(start, -uz) });

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void CollisionCapsule::CreateIndexBuffer()
{
	HRESULT result;
	int numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	int numberOfIB = numberOfTriangle * 3;
	int numberOfVB = (1 + slices * stacks_1_2) * 2;
	int sizeIB = static_cast<UINT>(sizeof(unsigned short) * numberOfIB);
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

	std::vector<unsigned short> indices;
	indices.reserve(numberOfIB);

	// P1側端点周りの計算
	for (int j = 0; j < slices; j++)
	{
		indices.push_back(0);
		indices.push_back(1 + j);
		indices.push_back(1 + (j + 1) % slices);
	}

	// P1-P2間の計算
	int imax = stacks_1_2 * 2 - 1;
	for (int i = 0; i < imax; i++)
	{
		int ibase = 1 + slices * i;
		for (int j = 0; j < slices; j++)
		{
			int jnext = (j + 1) % slices;
			indices.push_back(ibase + j);
			indices.push_back(ibase + j + slices);
			indices.push_back(ibase + jnext);

			indices.push_back(ibase + jnext);
			indices.push_back(ibase + j + slices);
			indices.push_back(ibase + jnext + slices);
		}
	}

	// P2側端点周りの計算
	int ibase = 1 + slices * imax;
	int ilast = numberOfVB - 1;
	for (int j = 0; j < slices; j++)
	{
		indices.push_back(ibase + j);
		indices.push_back(ilast);
		indices.push_back(ibase + (j + 1) % slices);
	}

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
	ibView.SizeInBytes = sizeIB;
}

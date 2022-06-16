#include "BillObj.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace XIIlib;

ID3D12Device* BillObj::device = nullptr;
ID3D12GraphicsCommandList* BillObj::cmdList = nullptr;
ComPtr<ID3D12RootSignature> BillObj::rootSignature;
ComPtr<ID3D12PipelineState> BillObj::pipelineState;
DebugCamera* BillObj::d_camera = nullptr;

bool BillObj::StaticInitialize()
{
	BillObj::device = DirectX12::GetDevice();
	BillObj::d_camera = d_camera;

	CreateGraphicsPipelineState();

	return true;
}

void BillObj::PreDraw()
{
	assert(BillObj::cmdList == nullptr);
	// コマンドリストをセット
	BillObj::cmdList = DirectX12::GetCmdList();

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void BillObj::PostDraw()
{
	// コマンドリストを解除
	BillObj::cmdList = nullptr;
}

BillObj* BillObj::Create(const Math::Vector3& position, const std::string& textureFilename)
{
	BillObj* collisionbox = new BillObj(position, textureFilename);

	if (collisionbox == nullptr) {
		return nullptr;
	}

	return collisionbox;
}

// 静的メンバ変数の実体
BillObj::BillObj(const Math::Vector3& position, const std::string& textureFilename)
{
	this->position = position;
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	device = DirectX12::GetDevice();

	if (!Initialize()) {
		assert(0);
	}

	if (!LoadTexture(textureFilename)) {
		assert(0);
	}
}

bool BillObj::LoadTexture(const std::string& textureFilename) {
	std::string path = textureFilename;
	// テクスチャなし
	if (path.size() == 0) {
		path = "white1x1.png";
	}

	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(),0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(),0, descriptorHandleIncrementSize);

	HRESULT result = S_FALSE;

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	const std::string directoryPath = "Resources/";

	// ファイルパスを結合
	std::string filepath = directoryPath + path;
	wchar_t wfilepath[128];

	// ユニコード文字列に変換
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// テクスチャ用バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		assert(0);
	}

	// テクスチャバッファにデータ転送
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		assert(0);
	}

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		cpuDescHandleSRV
	);

	return true;
}

void BillObj::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	plData->AddShader(XIILib::VS, "BillGaugeVS.hlsl");
	plData->AddShader(XIILib::PS, "BillGaugePS.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void BillObj::CreateVertexBuffer()
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

void BillObj::CreateConstBuffer()
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
}

bool BillObj::Initialize()
{
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

	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void BillObj::Update()
{
	assert(d_camera);

	XMMATRIX matScl, matRot, matTrans;
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
		constMap->matbillboard = d_camera->GetMatBillboard();
		constMap->color = color;
		constBuff->Unmap(0, nullptr);
	}
}

void BillObj::Draw()
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
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// 描画コマンド
	cmdList->DrawInstanced(vertNum, 1, 0, 0);
}

void BillObj::TransferVertices()
{
	HRESULT result;
	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (isFlipX)
	{// 左右入れ替え
		left = -left;
		right = -right;
	}

	if (isFlipY)
	{// 上下入れ替え
		top = -top;
		bottom = -bottom;
	}

	std::vector<VertexPosUv> vertices;
	vertices.resize(vertNum);
	
	vertices[0].pos = { left,	top,	0.0f };
	vertices[1].pos = { left,	bottom,	0.0f };
	vertices[2].pos = { right,	top,	0.0f };
	vertices[3].pos = { right,	bottom,	0.0f };

	vertices[0].uv = { 0,1 }; // 左上
	vertices[1].uv = { 0,0 }; // 左下
	vertices[2].uv = { 1,1 }; // 右上
	vertices[3].uv = { 1,0 }; // 右下

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void XIIlib::BillObj::SetSize(Math::Vector2 size)
{
	this->size = size;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void XIIlib::BillObj::SetAnchorPoint(Math::Vector2 anchorpoint)
{
	this->anchorpoint = anchorpoint;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void XIIlib::BillObj::SetIsFlipX(bool isFlipX)
{
	this->isFlipX = isFlipX;

	// 頂点バッファへのデータ転送
	TransferVertices();
}

void XIIlib::BillObj::SetIsFlipY(bool isFlipY)
{
	this->isFlipY = isFlipY;

	// 頂点バッファへのデータ転送
	TransferVertices();
}
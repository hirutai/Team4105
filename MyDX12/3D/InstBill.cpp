#include "InstBill.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace XIIlib;

ID3D12Device* InstBill::device = nullptr;
ID3D12GraphicsCommandList* InstBill::cmdList = nullptr;
ComPtr<ID3D12RootSignature> InstBill::rootSignature;
ComPtr<ID3D12PipelineState> InstBill::pipelineState;
DebugCamera* InstBill::d_camera = nullptr;

bool InstBill::StaticInitialize()
{
	InstBill::device = DirectX12::GetDevice();
	InstBill::d_camera = d_camera;

	CreateGraphicsPipelineState();

	return true;
}

void InstBill::PreDraw()
{
	assert(InstBill::cmdList == nullptr);
	// コマンドリストをセット
	InstBill::cmdList = DirectX12::GetCmdList();

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void InstBill::PostDraw()
{
	// コマンドリストを解除
	InstBill::cmdList = nullptr;
}

InstBill* InstBill::Create(const std::string& textureFilename)
{
	InstBill* collisionbox = new InstBill(textureFilename);

	if (collisionbox == nullptr) {
		return nullptr;
	}

	return collisionbox;
}

// 静的メンバ変数の実体
InstBill::InstBill(const std::string& textureFilename)
{
	device = DirectX12::GetDevice();

	if (!Initialize()) {
		assert(0);
	}

	if (!LoadTexture(textureFilename)) {
		assert(0);
	}
}

bool InstBill::LoadTexture(const std::string& textureFilename) {
	std::string path = textureFilename;
	// テクスチャなし
	if (path.size() == 0) {
		path = "white1x1.png";
	}

	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

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

void InstBill::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	plData->AddShader(XIILib::VS, "InstanceVS.hlsl");
	plData->AddShader(XIILib::PS, "InstancePS.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void InstBill::CreateVertexBuffer()
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

void InstBill::CreateConstBuffer()
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
}

bool InstBill::Initialize()
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

	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void InstBill::Update()
{
	assert(d_camera);

	CBVIDParameter* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->matbillboard = d_camera->GetMatBillboard();
		constMap->viewproj = d_camera->GetMatViewProjection();
		for (int i = 0; i < dataIndexCount; i++)
		{
			// ワールド行列の更新
			XMMATRIX matScale, matTrans;
			this->matWorld = XMMatrixIdentity();
			XMFLOAT4 colorMap = { 0,0,0,0 };

			if (iData.size() >= i + 1)
			{
				matScale = XMMatrixScaling(iData[i].scale, iData[i].scale, iData[i].scale);
				matTrans = XMMatrixTranslation(iData[i].position.x, iData[i].position.y, iData[i].position.z);
				this->matWorld *= matScale;
				this->matWorld *= matTrans;
				colorMap = iData[i].color;
				constMap->data[i].active = 1;
			}
			else if (iData.size() < i + 1)
			{
				constMap->data[i].active = 0;
			}

			constMap->data[i].mat = this->matWorld;
			constMap->data[i].color = colorMap;
		}

		this->constBuff->Unmap(0, nullptr);
	}
}

void InstBill::Draw()
{
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
	cmdList->DrawInstanced(vertNum, iData.size(), 0, 0);
	// 描画毎にデータを空にしておく
	ClearData();
}

void XIIlib::InstBill::DrawBillBox(Math::Vector3 position, float scale, float r, float g, float b, float a)
{
	iData.push_back({ position,scale, { Math::Transform(255, 1,r),Math::Transform(255, 1,g),Math::Transform(255, 1,b),Math::Transform(255, 1,a) }});
}

void InstBill::TransferVertices()
{
	HRESULT result;

	std::vector<VertexPosUv> vertices;
	vertices.resize(vertNum);

	const float h = 0.5f;
	vertices[0].pos = { -h,	-h,	0.0f };
	vertices[1].pos = { -h,	 h,	0.0f };
	vertices[2].pos = {  h,	-h,	0.0f };
	vertices[3].pos = {  h,	 h,	0.0f };

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

void XIIlib::InstBill::ReLoadTexture(const std::string& textureFilename)
{
	// もう一度読み込む
	LoadTexture(textureFilename);
}
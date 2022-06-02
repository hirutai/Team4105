#include "TextureInstance.h"
#include "../Pipeline/Pipeline.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Math;

ID3D12Device* TextureInstance::device = nullptr;
ID3D12GraphicsCommandList* TextureInstance::cmdList = nullptr;
ComPtr<ID3D12RootSignature> TextureInstance::rootSignature;
ComPtr<ID3D12PipelineState> TextureInstance::pipelineState;
DirectX::XMMATRIX TextureInstance::matProjection;
ComPtr<ID3D12DescriptorHeap> TextureInstance::descHeap;
ComPtr<ID3D12Resource>TextureInstance::whiteTexBuff;

namespace
{
void
PrintErrorBlob(ID3DBlob* blob)
{
	assert(blob);
	std::string err;
	err.resize(blob->GetBufferSize());
	copy_n((char*)blob->GetBufferPointer(), err.size(), err.begin());
}
}

bool TextureInstance::StaticInitialize()
{
	TextureInstance::device = DirectX12::GetDevice();

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

	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	std::string filepath = "Resources/white1x1.png";
	wchar_t wfilepath[128];

	// ユニコード文字列に変換
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
		return false;
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
		IID_PPV_ARGS(&whiteTexBuff));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// テクスチャバッファにデータ転送
	result = whiteTexBuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}

void TextureInstance::PreDraw()
{
	assert(TextureInstance::cmdList == nullptr);
	// コマンドリストをセット
	TextureInstance::cmdList = DirectX12::GetCmdList();

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineState.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// プリミティブ形状を設定D3D_PRIMITIVE_TOPOLOGY_POINT
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void TextureInstance::PostDraw()
{
	// コマンドリストを解除
	TextureInstance::cmdList = nullptr;
}

TextureInstance* TextureInstance::Create()
{
	TextureInstance* texIST = new TextureInstance();

	if (texIST == nullptr) {
		return nullptr;
	}

	if (!texIST->Initialize()) {
		delete texIST;
		assert(0);
		return nullptr;
	}

	return texIST;
}

// 静的メンバ変数の実体
TextureInstance::TextureInstance()
{
	texBuff = whiteTexBuff;
}

bool TextureInstance::CheckShaderCompileResultTIS(HRESULT result, ID3DBlob* error)
{
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			::OutputDebugStringA("ファイルが見当たりません");
		}
		else
		{
			std::string errstr;
			errstr.resize(error->GetBufferSize());

			std::copy_n((char*)error->GetBufferPointer(),
				error->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		return false;
	}
	else
	{
		return true;
	}
}

void TextureInstance::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1,XIILib::CULL_NONE,XIILib::MASK_DEBUG,XIILib::COMP_ALWAYS);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	plData->AddShader(XIILib::VS, "TIST_VS.hlsl");
	plData->AddShader(XIILib::PS, "TIST_PS.hlsl");

	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void TextureInstance::CreateVertexBuffer()
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

void TextureInstance::CreateConstBuffer()
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

bool TextureInstance::Initialize()
{
	SetSRV();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void TextureInstance::DrawTexture(float x, float y, float rot, float sx, float sy, float r, float g, float b, float a)
{
	IMG_DATA imgData;
	imgData.position = { x,y };
	imgData.rotation = rot;
	imgData.scale = { sx,sy };
	imgData.color = { Math::Transform(255, 1,r),Math::Transform(255, 1,g),Math::Transform(255, 1,b),Math::Transform(255, 1,a) };
	datas.push_back(imgData);
}

void TextureInstance::DrawTexture(Vector2 pos, float rot, Vector2 scale, Vector4 color)
{
	IMG_DATA imgData;
	imgData.position = pos;
	imgData.rotation = rot;
	imgData.scale = scale;
	imgData.color = { Math::Transform(255, 1,color.x),Math::Transform(255, 1,color.y),Math::Transform(255, 1,color.z),Math::Transform(255, 1,color.w) };
	datas.push_back(imgData);
}

void TextureInstance::DrawTexture(Math::IMG_DATA data)
{
	IMG_DATA imgData = data;
	imgData.color = { Math::Transform(255, 1,imgData.color.x),
		Math::Transform(255, 1,imgData.color.y),
		Math::Transform(255, 1,imgData.color.z),
		Math::Transform(255, 1,imgData.color.w) };
	datas.push_back(imgData);
}

void TextureInstance::Draw()
{
	TransferCBVInstance();

	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(),0,
			device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	// 描画コマンド
	cmdList->DrawInstanced((UINT)vertNum, datas.size(), 0, 0);
	ClearTextureInstance();
}

void TextureInstance::SetTexture(Texture& tex)
{
	texBuff = tex.Get();
	texSize.x = tex.GetWidth();
	texSize.y = tex.GetHeight();

	StreamTVBuff();
}

void TextureInstance::SetSRV()
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texBuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	);
}

void TextureInstance::TransferVertices()
{
	HRESULT result;

	// 左下、左上、右下、右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.x) * size.y;
	float bottom = (1.0f - anchorpoint.x) * size.y;
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

	// 頂点データ
	VertexPosUv vertices[vertNum];

	vertices[LB].pos = { left,	bottom,	0.0f }; // 左下
	vertices[LT].pos = { left,	top,	0.0f }; // 左上
	vertices[RB].pos = { right,	bottom,	0.0f }; // 右下
	vertices[RT].pos = { right,	top,	0.0f }; // 右上

	// テクスチャ情報取得
	if (texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	// 頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}

void TextureInstance::StreamTVBuff()
{
	SetSRV();

	TransferVertices();
}

void TextureInstance::TransferCBVInstance()
{
	// 定数バッファにデータ転送
	CBVIDParameter* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		for (int i = 0; i < datas.size(); i++)
		{
			// ワールド行列の更新
			XMMATRIX matWorld,matRot,matScale, matTrans;
			matWorld = XMMatrixIdentity();
			XMFLOAT4 colorMap = { 0,0,0,0 };

			if (datas.size() >= i + 1)
			{
				matRot = XMMatrixRotationZ(XMConvertToRadians(datas[i].rotation));
				matScale = XMMatrixScaling(datas[i].scale.x, datas[i].scale.y, 0.0f);
				matTrans = XMMatrixTranslation(datas[i].position.x, datas[i].position.y, 0.0f);
				matWorld *= matScale;
				matWorld *= matRot;
				matWorld *= matTrans;
				colorMap = { datas[i].color.x,datas[i].color.y,datas[i].color.z,datas[i].color.w };
				constMap->data[i].active = 1;
			}
			else if (datas.size() < i + 1)
			{
				constMap->data[i].active = 0;
			}

			constMap->data[i].mat = matWorld * matProjection;
			constMap->data[i].color = colorMap;
		}
		this->constBuff->Unmap(0, nullptr);
	}
}
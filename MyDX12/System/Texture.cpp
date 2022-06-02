#include "../Base/DirectX12.h"
#include "Texture.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

Texture::Texture(std::string filepath)
{
	TexInitFromPNG(filepath);
}
Texture::~Texture()
{
	if (m_texture) {
		m_texture->Release();
	}
}

void Texture::TexInitFromD3D_RESOURCE(ID3D12Resource* texture)
{
	if (m_texture) {
		m_texture->Release();
	}
	m_texture = texture;
	m_texture->AddRef();
	m_texDesc = m_texture->GetDesc();
}

void Texture::TexInitFromPNG(std::string filepath)
{
	wchar_t wfilepath[128];

	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));
	LoadTexFromPNG(wfilepath);
}

void Texture::LoadTexFromPNG(const wchar_t* filepath)
{
	auto device = DirectX12::GetDevice();

	HRESULT result;
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filepath,
		WIC_FLAGS_NONE,
		&metadata,
		scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	ID3D12Resource* texture;
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texture));

	result = texture->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);

	m_texture = texture;
	m_texDesc = m_texture->GetDesc();
}

void Texture::RegistSRV(D3D12_CPU_DESCRIPTOR_HANDLE descHandle, int bufferNo)
{
	if (m_texture) {
		auto device = DirectX12::GetDevice();
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.Format = m_texDesc.Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_texDesc.MipLevels;
		device->CreateShaderResourceView(m_texture, &srvDesc, descHandle);
	}
}
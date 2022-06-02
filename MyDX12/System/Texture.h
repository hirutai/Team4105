#pragma once
#include <d3d12.h>
#include <string>

class Texture
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture(){}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Texture();

	/// <summary>
	/// ファイルからテクスチャをロードするコンストラクタ
	/// </summary>
	/// <param name="filepath">ロードするテクスチャのファイルパス。</param>
	explicit Texture(std::string filepath);

	/// <summary>
	/// PNGファイルからテクスチャを初期化する。
	/// </summary>
	/// <param name="filepath">ロードするテクスチャのファイルパス。</param>
	void TexInitFromPNG(std::string filepath);
	/// <summary>
	/// D3Dリソースからテクスチャを初期化する。
	/// </summary>
	/// <param name="resrource">D3Dリソース。</param>
	void TexInitFromD3D_RESOURCE(ID3D12Resource* texture);

	/// <summary>
	/// SRVに登録。
	/// </summary>
	/// <param name="descHandle"></param>
	void RegistSRV(D3D12_CPU_DESCRIPTOR_HANDLE descHandle, int bufferNo);

	/// <summary>
	/// テクスチャが有効か判定。
	/// </summary>
	/// <returns>trueが返ってきたら有効。</returns>
	bool IsValid() const
	{
		return m_texture != nullptr;
	}

	ID3D12Resource* Get()
	{
		return m_texture;
	}
	/// <summary>
	/// テクスチャの幅を取得。
	/// </summary>
	/// <returns></returns>
	int GetWidth() const
	{
		return static_cast<int>(m_texDesc.Width);
	}
	/// <summary>
	/// テクスチャの高さを取得。
	/// </summary>
	/// <returns></returns>
	int GetHeight() const
	{
		return static_cast<int>(m_texDesc.Height);
	}
	/// <summary>
	/// テクスチャのフォーマットを取得。
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetFormat() const
	{
		return m_texDesc.Format;
	}

private:
	/// <summary>
	/// PNGファイルからテクスチャをロード。
	/// </summary>
	/// <param name="filepath">ファイルパス。</param>
	void LoadTexFromPNG(const wchar_t* filepath);

private:
	ID3D12Resource* m_texture = nullptr;	//テクスチャ。
	D3D12_RESOURCE_DESC m_texDesc;	//テクスチャ情報
};
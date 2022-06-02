#pragma once
#include "Texture.h"

class RenderTarget
{
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~RenderTarget();

	/// <summary>
	/// レンダーターゲットの作成。
	/// </summary>
	/// <param name="w">レンダリングターゲットの幅</param>
	/// <param name="h">レンダリングターゲットの高さ</param>
	/// <param name="mipLevel">ミップマップレベル。0を指定した場合はミップマップがサポートされているGPUでは1*1ピクセルまでのミップマップが作成される。</param>
	/// <param name="arraySize">テクスチャ配列のサイズ</param>
	/// <param name="colorFormat">カラーバッファのフォーマット。</param>
	/// <param name="depthStencilFormat">深度ステンシルバッファのフォーマット。</param>
	/// <returns>trueが返ってきたら作成成功</returns>
	bool Create(
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT colorFormat,
		DXGI_FORMAT depthStencilFormat,
		float clearColor[4] = nullptr
	);

	/// <summary>
	/// CPU側のレンダリングターゲットのディスクリプタハンドルを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCpuDescriptorHandle() const
	{
		return m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// CPU側のデプスステンシルバッファのディスクリプタハンドルを取得。
	/// </summary>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCpuDescriptorHandle() const
	{
		return m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	/// <summary>
	/// レンダリングターゲットとなるテクスチャを取得。
	/// </summary>
	/// <returns></returns>
	Texture& GetRenderTargetTexture()
	{
		return m_renderTargetTexture;
	}

	/// <summary>
	/// デプスステンシルバッファが存在しているか判定
	/// </summary>
	/// <returns></returns>
	bool IsExsitDepthStencilBuffer() const
	{
		return m_depthStencilTexture;
	}

	/// <summary>
	/// レンダリングターゲットの幅を取得。
	/// </summary>
	/// <returns></returns>
	int GetWidth() const
	{
		return m_width;
	}

	/// <summary>
	/// レンダリングターゲットの高さを取得。
	/// </summary>
	/// <returns></returns>
	int GetHeight() const
	{
		return m_height;
	}

	/// <summary>
	/// カラーバッファのフォーマットを取得。
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetColorBufferFormat() const
	{
		return m_renderTargetTexture.GetFormat();
	}

	const float* GetRTVClearColor() const
	{
		return m_rtvClearColor;
	}

	float GetDSVClearValue() const
	{
		return m_dsvClearValue;
	}

private:
	/// <summary>
	/// ディスクリプタヒープを作成。
	/// </summary>
	bool CreateDescriptorHeap(ID3D12Device*& d3dDevice);

	/// <summary>
	/// レンダリングターゲットとなるテクスチャを作成。
	/// </summary>
	bool CreateRenderTargetTexture(
		ID3D12Device*& d3dDevice,
		int w,
		int h,
		int mipLevel,
		int arraySize,
		DXGI_FORMAT format,
		float clearColor[4]
	);

	/// <summary>
	/// 深度ステンシルバッファとなるテクスチャを作成。
	/// </summary>
	bool CreateDepthStencilTexture(
		ID3D12Device*& d3dDevice,
		int w,
		int h,
		DXGI_FORMAT format);

	/// <summary>
	/// ディスクリプタの作成。
	/// </summary>
	void CreateDescriptor(ID3D12Device*& d3dDevice);

private:
	Texture m_renderTargetTexture;
	ID3D12Resource* m_renderTargetTextureDx12 = nullptr;	//レンダリングターゲットとなるテクスチャ。
	ID3D12Resource* m_depthStencilTexture = nullptr;		//深度ステンシルバッファとなるテクスチャ。
	ID3D12DescriptorHeap* m_rtvHeap = nullptr;				//RTV用のディスクリプタヒープ。
	ID3D12DescriptorHeap* m_dsvHeap = nullptr;				//深度ステンシルバッファビューのディスクリプタヒープ。
	UINT m_rtvDescriptorSize = 0;							//フレームバッファのディスクリプタのサイズ。
	UINT m_dsvDescriptorSize = 0;							//深度ステンシルバッファのディスクリプタのサイズ。
	int m_width = 0;										//レンダリングターゲットの幅。
	int m_height = 0;										//レンダリングターゲットの高さ。
	float m_rtvClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };	//レンダリングターゲットビューのクリアカラー。
	float m_dsvClearValue = 1.0f;							//DSVのクリアカラー。
};
#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
class PostEffect
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	// 画面クリアカラー
	static const float clearColor[4];

	// 頂点数
	static const int vertNum = 4;

public: // サブクラス
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz座標
		Math::Vector2 uv;  // uv座標
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D変換行列
		float _Time;
		float _density;
		int _type;
	};
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name = "cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name = "cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);
	void PostLDrawScene(ID3D12GraphicsCommandList* cmdList);

	void SetResourceBarrier(ID3D12GraphicsCommandList* cmdList, ComPtr<ID3D12PipelineState> pipelineState, ComPtr<ID3D12RootSignature> rootSignature);

	void RetResourceBarrier(ID3D12GraphicsCommandList* cmdList);

	void CreateVertexBuffer();

	void CreateConstBuffer();

	void CreateTexBuffer();

	void CreateSRV();

	void CreateRTV();

	void CreateDepthBuffer();

	void CreateDSV();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void TransferVertices();

	// リサイズ
	void SetSize(const Math::Vector2& wh) { size = wh; TransferVertices();}
	void SetSize(const float wid, const float hig) { size = {wid,hig}; TransferVertices();}

	// 座標のSeter
	void SetPosition(const Math::Vector2& pos) { position = pos;}
	void SetPosition(const float x, const float y) { position = { x,y };}

	void SetAncPoint(const Math::Vector2& ap) { anchorpoint = ap; TransferVertices();}
	void SetAncPoint(const float x, const float y) { anchorpoint = { x,y }; TransferVertices();}

	void SetDensity(float dens) { density = dens; }
	void SetTypeEffect(int typ) { postType = typ; }

	void SetZBuffer(ComPtr<ID3D12Resource> pZ) { zTex = pZ; }

private:
	ID3D12Device* device = nullptr;
	// 時間
	float _time = 0.0f;
	float density = 0.0f;
	int postType = 0;
	// テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	// SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

	// 深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	// 深度テクスチャ
	ComPtr<ID3D12Resource> sMapTex;
	// Zテクスチャ
	ComPtr<ID3D12Resource> zTex;
	// RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	// DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;

	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// Z軸回りの回転角
	float rotation = 0.0f;
	// 座標
	Math::Vector2 position{};
	// スプライト幅、高さ
	Math::Vector2 size = { 100.0f, 100.0f };
	// アンカーポイント
	Math::Vector2 anchorpoint = { 0, 0 };
	// ワールド行列
	DirectX::XMMATRIX matWorld{};
	// 射影行列
	DirectX::XMMATRIX matProjection;
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;

	// パイプラインステートオブジェクト
	ComPtr<ID3D12PipelineState> pipelineState;
	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;
};
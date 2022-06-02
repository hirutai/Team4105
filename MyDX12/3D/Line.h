#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class DebugCamera;

class Line
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	// 頂点数
	static const int vertNum = 2;

public: // サブクラス
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz座標
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D変換行列
		DirectX::XMFLOAT3 xm_start;
		DirectX::XMMATRIX viewproj;
		DirectX::XMFLOAT3 xm_end;
	};

private:
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelineState;
	// 射影行列
	static DirectX::XMMATRIX matProjection;
	// デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	// カメラ
	static DebugCamera* d_camera;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipelineState();

	/// <summary>
	/// カメラのセット
	/// </summary>
	static void SetDebugCamera(DebugCamera* d_camera)
	{
		Line::d_camera = d_camera;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	static Line* Create(const Math::Vector3& start, const Math::Vector3& end);

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Line(const Math::Vector3& start, const Math::Vector3& end);

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	void Update();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void Draw();

	void TransferVertices();

	void SetStP(const Math::Vector3& st) { start = st; TransferVertices(); }
	void SetStP(const float x, const float y, const float z) { start = {x,y,z}; TransferVertices(); }

	void SetEdP(const Math::Vector3& ed) { end = ed; TransferVertices(); }
	void SetEdP(const float x, const float y, const float z) { end = { x,y ,z}; TransferVertices(); }

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};

	// 座標
	Math::Vector3 position{};

	Math::Vector3 start = Math::Vector3();
	Math::Vector3 end = Math::Vector3();
	// ワールド行列
	DirectX::XMMATRIX matWorld{};
};

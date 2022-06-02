#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class DebugCamera;

class CollisionCircle
{
public: // サブクラス
	struct VertexPosUv
	{
		Math::Vector3 pos; // xyz座標
	};

	struct CbData
	{
		DirectX::XMMATRIX mat;		// 3D変換行列
		DirectX::XMMATRIX viewproj;
		DirectX::XMFLOAT4 color;
	};

private:
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// ルートシグネチャ
	static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	// パイプラインステートオブジェクト
	static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
	// 射影行列
	static DirectX::XMMATRIX matProjection;
	// デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
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
		CollisionCircle::d_camera = d_camera;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	static CollisionCircle* Create(float radius = 1.0f,int v_count = 8);

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionCircle(float radius, int v_count);

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

	void CreateIndexBuffer();

	void SetMesh();

	void SetVertexCount(int count_v);

	void SetPosition(float x, float y, float z) { position = { x,y,z }; }

	void SetScale(float sx, float sy, float sz) { scale = { sx,sy,sz }; }

	void SetColor(float r, float g, float b, float a) { color = { r,g,b,a }; }

	float GetRadius()const { return radius; }

	Math::Vector3 GetPosition()const { return position; }

private:
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};

	unsigned int vert_count = 8;

	// 座標
	Math::Vector3 position{};

	DirectX::XMFLOAT4 color;
	float radius = 0.0f;
	Math::Vector3 scale{};
	// ワールド行列
	DirectX::XMMATRIX matWorld{};
};
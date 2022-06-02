#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"

class DebugCamera;

class CollisionCapsule
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
		CollisionCapsule::d_camera = d_camera;
	}

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	static CollisionCapsule* Create(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices);

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CollisionCapsule(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices, int stacks_1_2);

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

	void SetPosition(float x, float y, float z) { position = { x,y,z }; }

	void SetStP(const Math::Vector3& st) { start = st; TransferVertices(); }
	void SetStP(const float x, const float y, const float z) { start = { x,y,z }; TransferVertices(); }

	void SetEdP(const Math::Vector3& ed) { end = ed; TransferVertices(); }
	void SetEdP(const float x, const float y, const float z) { end = { x,y ,z }; TransferVertices(); }

	void SetColor(float r, float g, float b, float a) { color = { r,g,b,a }; }

	Math::Vector3 GetStartPoint()const { return start + position; }
	Math::Vector3 GetEndPoint()const { return end + position;}
	float GetSlices()const { return slices; }
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

	// 座標
	Math::Vector3 position{};

	Math::Vector3 start = Math::Vector3();
	Math::Vector3 end = Math::Vector3();
	DirectX::XMFLOAT4 color;
	int slices;
	int stacks_1_2;
	float radius = 0.0f;
	// ワールド行列
	DirectX::XMMATRIX matWorld{};
};

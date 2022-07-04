#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
class DebugCamera;

namespace XIIlib {

	// 頂点変動型オブジェクト
	class BillObj
	{
	private:
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
			DirectX::XMMATRIX viewproj;
			DirectX::XMMATRIX matbillboard;
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
			BillObj::d_camera = d_camera;
		}

		/// <summary>
		/// 描画前処理
		/// </summary>
		static void PreDraw();

		/// <summary>
		/// 描画後処理
		/// </summary>
		static void PostDraw();

		static BillObj* Create(const Math::Vector3& position, const std::string& textureFilename,bool outBill = false);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		BillObj(const Math::Vector3& position, const std::string& textureFilename, bool outBill);

		bool LoadTexture(const std::string& textureFilename);

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

		void SetPosition(float x, float y, float z) { position = { x,y,z }; }

		void SetColor(float r, float g, float b, float a) { color = { r,g,b,a }; }
		void SetScale(float x, float y, float z) { scale = { x,y,z }; }
		void SetRotation(float x, float y, float z) { rotation = { x,y,z }; }

		void SetSize(Math::Vector2 size);
		void SetAnchorPoint(Math::Vector2 anchorpoint);
		void SetIsFlipX(bool isFlipX);
		void SetIsFlipY(bool isFlipY);

		void ReLoadTexture(const std::string& textureFilename);

		Math::Vector3 GetPosition()const { return position; }

	private:
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// シェーダリソースビューのハンドル(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// デスクリプタヒープ
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		// テクスチャバッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
		UINT descriptorHandleIncrementSize;

		// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		// 定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		// インデックスバッファビュー
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		// 座標
		Math::Vector3 position{};
		Math::Vector3 scale{};
		Math::Vector3 rotation{};

		DirectX::XMFLOAT4 color;
		// ワールド行列
		DirectX::XMMATRIX matWorld{};

		// スプライト幅、高さ
		Math::Vector2 size = { 5.0f, 5.0f };
		// アンカーポイント
		Math::Vector2 anchorpoint = { 0.5f, 0.5f };
		// 左右反転
		bool isFlipX = false;
		// 上下反転
		bool isFlipY = false;

		// ビルボード行列をかけるかかけないか
		bool outBillboard = false;
	};
}
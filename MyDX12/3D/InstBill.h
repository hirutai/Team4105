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
	class InstBill
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
			DirectX::XMFLOAT4 color;
			unsigned int active;
		};

		static const int dataIndexCount = 500;
		struct CBVIDParameter {
			DirectX::XMMATRIX viewproj;
			DirectX::XMMATRIX matbillboard;
			CbData data[dataIndexCount];
		};

		struct InstanceData
		{
			Math::Vector3 position{};
			float scale = 0.0f;
			DirectX::XMFLOAT4 color{};
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
			InstBill::d_camera = d_camera;
		}

		/// <summary>
		/// 描画前処理
		/// </summary>
		static void PreDraw();

		/// <summary>
		/// 描画後処理
		/// </summary>
		static void PostDraw();

		static InstBill* Create(const std::string& textureFilename);

	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		InstBill(const std::string& textureFilename);

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

		void DrawBillBox(Math::Vector3 position,float scale, float r, float g, float b, float a);

		void TransferVertices();
		void ClearData() { 
			if (iData.size() != 0){
				iData.clear();
				const int sizeAll = 500;
				iData.reserve(sizeof(InstanceData) * sizeAll);
			}
		}

		void ReLoadTexture(const std::string& textureFilename);

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
		std::vector<InstanceData> iData;
		// ワールド行列
		DirectX::XMMATRIX matWorld{};

		// スプライト幅、高さ
		Math::Vector2 size = { 5.0f, 5.0f };
	};
}
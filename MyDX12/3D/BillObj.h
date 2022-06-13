#pragma once
#include "../Struct/ConstBufferData.h"
#include "../Struct/Math.h"

namespace XIIlib{

	// 頂点変動型オブジェクト
	class BillObj {
	public: // サブクラス
		struct VertexPosUv
		{
			Math::Vector3 pos; // xyz座標
			Math::Vector2 uv;  // uv座標
		};
	public: // 静的メンバ関数
		static bool StaticInitialize();

		static bool LoadTexture(UINT texnumber, const wchar_t* filename);

		static void PreDraw();

		static void PostDraw();

		static BillObj* Create();

	private: // 静的メンバ変数
		// テクスチャの最大枚数
		static const int srvCount = 128;
		// 頂点数
		static const int vertNum = 4;
		// デバイス
		static ID3D12Device* device;
		// デスクリプタサイズ
		static UINT descriptorHandleIncrementSize;
		// コマンドリスト
		static ID3D12GraphicsCommandList* cmdList;
		// ルートシグネチャ
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		// パイプラインステートオブジェクト
		static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		// デスクリプタヒープ
		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		// テクスチャバッファ
		static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[srvCount];
	public: // メンバ関数
		BillObj();

		bool Initialize();

		void SetRotation(float rotation);

		void SetPosition(Math::Vector3 position);

		void SetSize(Math::Vector2 size);

		void SetAnchorPoint(Math::Vector2 anchorpoint);

		void SetColor(float r, float g, float b, float a);

		void SetIsFlipX(bool isFlipX);

		void SetIsFlipY(bool isFlipY);

		void SetTextureRect(Math::Vector2 texBase, Math::Vector2 texSize);

		void Draw();

	private: // メンバ変数
// 頂点バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		// 定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

		// テクスチャ番号
		UINT texNumber = 0;
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
		// 色
		DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
		// 左右反転
		bool isFlipX = false;
		// 上下反転
		bool isFlipY = false;
		// テクスチャ始点
		Math::Vector2 texBase = { 0, 0 };
		// テクスチャ幅、高さ
		Math::Vector2 texSize = { 100.0f, 100.0f };

	private:
		/// <summary>
		/// 頂点データ転送
		/// </summary>
		void TransferVertices();
	};

}
#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
#include "../System/Texture.h"

class TextureInstance
{
private: // Alias
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

public: // Sub
	struct VertexPosUv
	{
		Vector3 pos;
		Vector2 uv;
	};

	struct CbData // 個別の情報
	{
		DirectX::XMMATRIX mat;
		DirectX::XMFLOAT4 color;
		unsigned int active;
	};

	static const int dataIndexCount = 500;

	struct CBVIDParameter
	{
		CbData data[dataIndexCount];
	};

	struct SpData
	{
		Vector2 position{};
		DirectX::XMFLOAT4 color{};
		float rotation = 0.0f;
		Vector2 scale{};
	};

private:
	// 頂点数
	static const int vertNum = 4;

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
	static ComPtr<ID3D12Resource>whiteTexBuff;

public: // 静的メンバ関数
/// <summary>
/// 静的初期化
/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreateGraphicsPipelineState();

	static bool CheckShaderCompileResultTIS(HRESULT result, ID3DBlob* error);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	static TextureInstance* Create();

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureInstance();

	void CreateVertexBuffer();

	void CreateConstBuffer();

	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	void DrawTexture(float x, float y, float rot = 0.0f,float sx = 1.0f,float sy = 1.0f, float r = 255.0f, float g = 255.0f, float b = 255.0f, float a = 255.0f);
	void DrawTexture(Vector2 pos, float rot = 0.0f, Vector2 scale = { 1,1 }, Math::Vector4 color = {255,255,255,255 });
	void DrawTexture(Math::IMG_DATA data);

	void Draw();

	void SetTexture(Texture& tex);
	void SetSRV();
	void TransferVertices();
	void StreamTVBuff();
	void TransferCBVInstance();
	void ClearTextureInstance() { if (datas.size() != 0)datas.clear(); }

	void SetSize(Vector2 size)
	{
		this->size = size;

		TransferVertices();
	}

	void SetAnchorPoint(Vector2 anchorpoint)
	{
		this->anchorpoint = anchorpoint;

		TransferVertices();
	}

	void SetIsFlipX(bool isFlipX)
	{
		this->isFlipX = isFlipX;

		TransferVertices();
	}

	void SetIsFlipY(bool isFlipY)
	{
		this->isFlipY = isFlipY;

		TransferVertices();
	}

	void SetTextureRect(Vector2 texBase, Vector2 texSize)
	{
		this->texBase = texBase;
		this->texSize = texSize;

		TransferVertices();
	}

	Vector2 GetRectSize()
	{
		return texSize;
	}

private:
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// 定数バッファ(Instance用)
	ComPtr<ID3D12Resource> constBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ComPtr<ID3D12Resource> texBuff;

	// 座標
	std::vector<Math::IMG_DATA> datas;
	
	// スプライト幅、高さ
	Vector2 size = { 100.0f, 100.0f };
	// アンカーポイント
	Vector2 anchorpoint = { 0, 0 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	Vector2 texBase = { 0, 0 };
	// テクスチャ幅、高さ
	Vector2 texSize = { 100.0f, 100.0f };
};
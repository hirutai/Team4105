#pragma once
#include "../Struct/ConstBufferData.h"
#include "../Pipeline/Pipeline.h"
#include "../Struct/Math.h"
#include "../System/Texture.h"

class Sprite
{
private: // エイリアス
	using Vector2 = Math::Vector2;
	using Vector3 = Math::Vector3;

public: // サブクラス
	struct VertexPosUv
	{
		Vector3 pos; // xyz座標
		Vector2 uv;  // uv座標
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static bool StaticInitialize();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static bool LoadTexture(UINT texnumber, const wchar_t*filename);
	static bool LoadTexture(UINT texnumber, Texture texture);

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// スプライト生成
	/// </summary>
	static Sprite* Create(UINT texNumber, Vector2 position, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 }, Vector2 anchorpoint = { 0.0f, 0.0f }, bool isFlipX = false, bool isFlipY = false);

private: // 静的メンバ変数
	// テクスチャの最大枚数
	static const int srvCount = 512;
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
	// 射影行列
	static DirectX::XMMATRIX matProjection;
	// デスクリプタヒープ
	static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
	// テクスチャバッファ
	static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[srvCount];

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Sprite(UINT texNumber, Vector2 position, Vector2 size, DirectX::XMFLOAT4 color, Vector2 anchorpoint, bool isFlipX, bool isFlipY);
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize();
	/// <summary>
	/// 角度の設定
	/// </summary>
	void SetRotation(float rotation);
	/// <summary>
	/// 座標の取得
	/// </summary>
	const Vector2& GetPosition() { return position; }
	/// <summary>
	/// 座標の設定
	/// </summary>
	void SetPosition(Vector2 position);
	/// <summary>
	/// サイズの設定
	/// </summary>
	void SetSize(Vector2 size);
	/// <summary>
	/// アンカーポイントの設定
	/// </summary>
	void SetAnchorPoint(Vector2 anchorpoint);

	// RGB値の設定
	void SetColorRGB(float r, float g, float b);
	// α値の設定
	void SetAlpha(float alpha);
	// 色の四要素の設定
	void SetColor(float r,float g,float b,float a);

	/// <summary>
	/// 左右反転の設定
	/// </summary>
	void SetIsFlipX(bool isFlipX);

	/// <summary>
	/// 上下反転の設定
	/// </summary>
	void SetIsFlipY(bool isFlipY);

	/// <summary>
	/// テクスチャ範囲設定
	/// </summary>
	void SetTextureRect(Vector2 texBase, Vector2 texSize);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	Vector2 GetDefault()const { return defaultSize; }

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
	Vector2 position{};
	// スプライト幅、高さ
	Vector2 size = { 100.0f, 100.0f };
	Vector2 defaultSize = { 100.0f, 100.0f };
	// アンカーポイント
	Vector2 anchorpoint = { 0, 0 };
	// ワールド行列
	DirectX::XMMATRIX matWorld{};
	// 色
	DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
	// 左右反転
	bool isFlipX = false;
	// 上下反転
	bool isFlipY = false;
	// テクスチャ始点
	Vector2 texBase = { 0, 0 };
	// テクスチャ幅、高さ
	Vector2 texSize = { 100.0f, 100.0f };

private:
	/// <summary>
	/// 頂点データ転送
	/// </summary>
	void TransferVertices();
};


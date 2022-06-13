#pragma once
#include "Model.h"
#include "../Pipeline/Pipeline.h"
#include "../Light/LightGroup.h"
#include "../Struct/ConstBufferDataB0.h"

class DebugCamera;

enum struct SMOOTH
{
	_OUT,
	_IN,
};

class Object3D
{
private: // エイリアス
	using Vector3 = Math::Vector3;

public:
	struct PipelineSet
	{
		// ルートシグネチャ
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
		// パイプラインステートオブジェクト
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	};

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// グラフィックパイプラインの生成
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// カメラのセット
	/// </summary>
	static void SetDebugCamera(DebugCamera* d_camera) { Object3D::d_camera = d_camera; }

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	static Object3D* Create(Model* model = nullptr, float elec_is = 0.0f);

	/// <summary>
	/// ライトグループのセット
	/// </summary>
	/// <param name="lightGroup">ライトグループ</param>
	static void SetLightGroup(LightGroup* lightGroup) {
		Object3D::lightGroup = lightGroup;
	}

private: // 静的メンバ変数
	// デバイス
	static ID3D12Device* device;
	// コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	// パイプライン
	static PipelineSet pipelineSet;
	// カメラ
	static DebugCamera* d_camera;
	// ライト
	static LightGroup* lightGroup;

public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Object3D() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Object3D();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	//bool Initialize();
	virtual bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	//void Update();
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	//void Draw();
	virtual void Draw();

	/// <summary>
	/// 行列の更新
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// ワールド行列の取得
	/// </summary>
	/// <returns>ワールド行列</returns>
	const DirectX::XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// モデルのセット
	/// </summary>
	/// <param name="model">モデル</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// ビルボードフラグのセット
	/// </summary>
	/// <param name="isBillboard">ビルボードか</param>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// シェーダータイプのセット
	/// </summary>
	/// <param name="_isElec">シェーダータイプ(電気のみの0orその他)</param>
	void SetShaderType(float shader_type) { this->_isElec = shader_type; }

	/// <summary>
	/// モデルを取得
	/// </summary>
	/// <returns>モデル</returns>
	inline Model* GetModel() { return model; }

//private: // メンバ変数
protected: // メンバ変数
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0; // 定数バッファ
	
	// クラス名 (デバッグ用)
	const char* name = nullptr;

	// ローカルワールド変換行列
	DirectX::XMMATRIX matWorld;
	// 親オブジェクト
	Object3D* parent = nullptr;
	// モデル
	Model* model = nullptr;
	// ビルボード
	bool isBillboard = false;

	// シェーダー用タイマー
	float shaderTimer = 0.0f;
	float _isElec = 0.0f;
	//// ネームタグ
	//NameTag tag = NONE_NAME;

public:
	// ローカルスケール
	Vector3 scale = { 1,1,1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation = { 0,0,0 };
	// ローカル座標
	Vector3 position = { 0,0,0 };
	// RGB
	Vector3 color = { 1,1,1 };
	// 透過値
	float alpha = 1.0f;

	Vector3 prePos = { 0,0,0 };
};
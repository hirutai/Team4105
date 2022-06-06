#pragma once
#include "../Base/DirectX12.h"
#include "Mesh.h"

class Model
{
private:
	static const std::string baseDirectory;

private: // 静的メンバ変数
	static ID3D12Device* device;
	// デスクリプタサイズ
	static UINT descriptorHandleIncrementSize;

public: // 静的メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// OBJファイルからメッシュ生成
	/// </summary>
	static Model* CreateFromOBJ(const std::string& modelname, bool smoothing = false);

public: // メンバ関数
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& modelname, bool smoothing);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// メッシュコンテナを取得
	/// </summary>
	/// <returns>メッシュコンテナ</returns>
	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }

	void LoadRendText(Texture texture);

private: // メンバ変数
	// 名前
	std::string name;
	// メッシュコンテナ
	std::vector<Mesh*> meshes;
	// マテリアルコンテナ
	std::unordered_map<std::string, Material*> materials;
	// デフォルトマテリアル
	Material* defaultMaterial = nullptr;
	// デスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

private: // メンバ関数
	/// <summary>
	/// マテリアル読み込み
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string & filename);

	/// <summary>
	/// マテリアル登録
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// デスクリプタヒープの生成
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTextures();
};


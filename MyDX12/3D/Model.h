#pragma once
#include "../Base/DirectX12.h"
#include "Mesh.h"

class Model
{
private:
	static const std::string baseDirectory;

private: // �ÓI�����o�ϐ�
	static ID3D12Device* device;
	// �f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;

public: // �ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// OBJ�t�@�C�����烁�b�V������
	/// </summary>
	static Model* CreateFromOBJ(const std::string& modelname, bool smoothing = false);

public: // �����o�֐�
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Model();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(const std::string& modelname, bool smoothing);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���b�V���R���e�i���擾
	/// </summary>
	/// <returns>���b�V���R���e�i</returns>
	inline const std::vector<Mesh*>& GetMeshes() { return meshes; }

	void LoadRendText(Texture texture);

private: // �����o�ϐ�
	// ���O
	std::string name;
	// ���b�V���R���e�i
	std::vector<Mesh*> meshes;
	// �}�e���A���R���e�i
	std::unordered_map<std::string, Material*> materials;
	// �f�t�H���g�}�e���A��
	Material* defaultMaterial = nullptr;
	// �f�X�N���v�^�q�[�v
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;

private: // �����o�֐�
	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	void LoadMaterial(const std::string& directoryPath, const std::string & filename);

	/// <summary>
	/// �}�e���A���o�^
	/// </summary>
	void AddMaterial(Material* material);

	/// <summary>
	/// �f�X�N���v�^�q�[�v�̐���
	/// </summary>
	void CreateDescriptorHeap();

	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTextures();
};


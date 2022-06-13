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
private: // �G�C���A�X
	using Vector3 = Math::Vector3;

public:
	struct PipelineSet
	{
		// ���[�g�V�O�l�`��
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	};

public: // �ÓI�����o�֐�
	/// <summary>
	/// �ÓI������
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// �O���t�B�b�N�p�C�v���C���̐���
	/// </summary>
	static void CreateGraphicsPipeline();

	/// <summary>
	/// �J�����̃Z�b�g
	/// </summary>
	static void SetDebugCamera(DebugCamera* d_camera) { Object3D::d_camera = d_camera; }

	/// <summary>
	/// �`��O����
	/// </summary>
	static void PreDraw();

	/// <summary>
	/// �`��㏈��
	/// </summary>
	static void PostDraw();

	/// <summary>
	/// 3D�I�u�W�F�N�g����
	/// </summary>
	static Object3D* Create(Model* model = nullptr, float elec_is = 0.0f);

	/// <summary>
	/// ���C�g�O���[�v�̃Z�b�g
	/// </summary>
	/// <param name="lightGroup">���C�g�O���[�v</param>
	static void SetLightGroup(LightGroup* lightGroup) {
		Object3D::lightGroup = lightGroup;
	}

private: // �ÓI�����o�ϐ�
	// �f�o�C�X
	static ID3D12Device* device;
	// �R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	// �p�C�v���C��
	static PipelineSet pipelineSet;
	// �J����
	static DebugCamera* d_camera;
	// ���C�g
	static LightGroup* lightGroup;

public: // �����o�֐�
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3D() = default;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Object3D();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	//bool Initialize();
	virtual bool Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	//void Update();
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	//void Draw();
	virtual void Draw();

	/// <summary>
	/// �s��̍X�V
	/// </summary>
	void UpdateWorldMatrix();

	/// <summary>
	/// ���[���h�s��̎擾
	/// </summary>
	/// <returns>���[���h�s��</returns>
	const DirectX::XMMATRIX& GetMatWorld() { return matWorld; }

	/// <summary>
	/// ���f���̃Z�b�g
	/// </summary>
	/// <param name="model">���f��</param>
	void SetModel(Model* model) { this->model = model; }

	/// <summary>
	/// �r���{�[�h�t���O�̃Z�b�g
	/// </summary>
	/// <param name="isBillboard">�r���{�[�h��</param>
	void SetBillboard(bool isBillboard) { this->isBillboard = isBillboard; }

	/// <summary>
	/// �V�F�[�_�[�^�C�v�̃Z�b�g
	/// </summary>
	/// <param name="_isElec">�V�F�[�_�[�^�C�v(�d�C�݂̂�0or���̑�)</param>
	void SetShaderType(float shader_type) { this->_isElec = shader_type; }

	/// <summary>
	/// ���f�����擾
	/// </summary>
	/// <returns>���f��</returns>
	inline Model* GetModel() { return model; }

//private: // �����o�ϐ�
protected: // �����o�ϐ�
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuffB0; // �萔�o�b�t�@
	
	// �N���X�� (�f�o�b�O�p)
	const char* name = nullptr;

	// ���[�J�����[���h�ϊ��s��
	DirectX::XMMATRIX matWorld;
	// �e�I�u�W�F�N�g
	Object3D* parent = nullptr;
	// ���f��
	Model* model = nullptr;
	// �r���{�[�h
	bool isBillboard = false;

	// �V�F�[�_�[�p�^�C�}�[
	float shaderTimer = 0.0f;
	float _isElec = 0.0f;
	//// �l�[���^�O
	//NameTag tag = NONE_NAME;

public:
	// ���[�J���X�P�[��
	Vector3 scale = { 1,1,1 };
	// X,Y,Z�����̃��[�J����]�p
	Vector3 rotation = { 0,0,0 };
	// ���[�J�����W
	Vector3 position = { 0,0,0 };
	// RGB
	Vector3 color = { 1,1,1 };
	// ���ߒl
	float alpha = 1.0f;

	Vector3 prePos = { 0,0,0 };
};
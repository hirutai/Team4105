#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include "../Base/DirectX12.h"
#include "../Struct/Math.h"
class DebugCamera;

namespace XIIlib {

	// ���_�ϓ��^�I�u�W�F�N�g
	class InstBill
	{
	private:
		// ���_��
		static const int vertNum = 4;

	public: // �T�u�N���X
		struct VertexPosUv
		{
			Math::Vector3 pos; // xyz���W
			Math::Vector2 uv;  // uv���W
		};

		struct CbData
		{
			DirectX::XMMATRIX mat;		// 3D�ϊ��s��
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
		// �f�o�C�X
		static ID3D12Device* device;
		// �R�}���h���X�g
		static ID3D12GraphicsCommandList* cmdList;
		// ���[�g�V�O�l�`��
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		// �J����
		static DebugCamera* d_camera;

	public: // �ÓI�����o�֐�
		/// <summary>
		/// �ÓI������
		/// </summary>
		static bool StaticInitialize();

		/// <summary>
		/// �p�C�v���C������
		/// </summary>
		static void CreateGraphicsPipelineState();

		/// <summary>
		/// �J�����̃Z�b�g
		/// </summary>
		static void SetDebugCamera(DebugCamera* d_camera)
		{
			InstBill::d_camera = d_camera;
		}

		/// <summary>
		/// �`��O����
		/// </summary>
		static void PreDraw();

		/// <summary>
		/// �`��㏈��
		/// </summary>
		static void PostDraw();

		static InstBill* Create(const std::string& textureFilename);

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		InstBill(const std::string& textureFilename);

		bool LoadTexture(const std::string& textureFilename);

		void CreateVertexBuffer();

		void CreateConstBuffer();

		/// <summary>
		/// ������
		/// </summary>
		bool Initialize();

		void Update();

		/// <summary>
		/// �`��R�}���h�̔��s
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
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
		// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
		CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
		// �f�X�N���v�^�q�[�v
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		// �e�N�X�`���o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> texbuff;
		UINT descriptorHandleIncrementSize;

		// ���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		// �萔�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		// �C���f�b�N�X�o�b�t�@�r���[
		D3D12_INDEX_BUFFER_VIEW ibView = {};

		// ���W
		std::vector<InstanceData> iData;
		// ���[���h�s��
		DirectX::XMMATRIX matWorld{};

		// �X�v���C�g���A����
		Math::Vector2 size = { 5.0f, 5.0f };
	};
}
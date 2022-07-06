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
	class BillObj
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
			DirectX::XMMATRIX viewproj;
			DirectX::XMMATRIX matbillboard;
			DirectX::XMFLOAT4 color;
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
			BillObj::d_camera = d_camera;
		}

		/// <summary>
		/// �`��O����
		/// </summary>
		static void PreDraw();

		/// <summary>
		/// �`��㏈��
		/// </summary>
		static void PostDraw();

		static BillObj* Create(const Math::Vector3& position, const std::string& textureFilename,bool outBill = false);

	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		BillObj(const Math::Vector3& position, const std::string& textureFilename, bool outBill);

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
		Math::Vector3 position{};
		Math::Vector3 scale{};
		Math::Vector3 rotation{};

		DirectX::XMFLOAT4 color;
		// ���[���h�s��
		DirectX::XMMATRIX matWorld{};

		// �X�v���C�g���A����
		Math::Vector2 size = { 5.0f, 5.0f };
		// �A���J�[�|�C���g
		Math::Vector2 anchorpoint = { 0.5f, 0.5f };
		// ���E���]
		bool isFlipX = false;
		// �㉺���]
		bool isFlipY = false;

		// �r���{�[�h�s��������邩�����Ȃ���
		bool outBillboard = false;
	};
}
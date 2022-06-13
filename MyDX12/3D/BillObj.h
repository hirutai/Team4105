#pragma once
#include "../Struct/ConstBufferData.h"
#include "../Struct/Math.h"

namespace XIIlib{

	// ���_�ϓ��^�I�u�W�F�N�g
	class BillObj {
	public: // �T�u�N���X
		struct VertexPosUv
		{
			Math::Vector3 pos; // xyz���W
			Math::Vector2 uv;  // uv���W
		};
	public: // �ÓI�����o�֐�
		static bool StaticInitialize();

		static bool LoadTexture(UINT texnumber, const wchar_t* filename);

		static void PreDraw();

		static void PostDraw();

		static BillObj* Create();

	private: // �ÓI�����o�ϐ�
		// �e�N�X�`���̍ő喇��
		static const int srvCount = 128;
		// ���_��
		static const int vertNum = 4;
		// �f�o�C�X
		static ID3D12Device* device;
		// �f�X�N���v�^�T�C�Y
		static UINT descriptorHandleIncrementSize;
		// �R�}���h���X�g
		static ID3D12GraphicsCommandList* cmdList;
		// ���[�g�V�O�l�`��
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
		// �p�C�v���C���X�e�[�g�I�u�W�F�N�g
		static Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState;
		// �f�X�N���v�^�q�[�v
		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descHeap;
		// �e�N�X�`���o�b�t�@
		static Microsoft::WRL::ComPtr<ID3D12Resource> texBuff[srvCount];
	public: // �����o�֐�
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

	private: // �����o�ϐ�
// ���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;
		// �萔�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuff;
		// ���_�o�b�t�@�r���[
		D3D12_VERTEX_BUFFER_VIEW spriteVBView{};

		// �e�N�X�`���ԍ�
		UINT texNumber = 0;
		// Z�����̉�]�p
		float rotation = 0.0f;
		// ���W
		Math::Vector2 position{};
		// �X�v���C�g���A����
		Math::Vector2 size = { 100.0f, 100.0f };
		// �A���J�[�|�C���g
		Math::Vector2 anchorpoint = { 0, 0 };
		// ���[���h�s��
		DirectX::XMMATRIX matWorld{};
		// �F
		DirectX::XMFLOAT4 color = { 1, 1, 1, 1 };
		// ���E���]
		bool isFlipX = false;
		// �㉺���]
		bool isFlipY = false;
		// �e�N�X�`���n�_
		Math::Vector2 texBase = { 0, 0 };
		// �e�N�X�`�����A����
		Math::Vector2 texSize = { 100.0f, 100.0f };

	private:
		/// <summary>
		/// ���_�f�[�^�]��
		/// </summary>
		void TransferVertices();
	};

}
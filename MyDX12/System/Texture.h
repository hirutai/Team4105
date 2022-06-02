#pragma once
#include <d3d12.h>
#include <string>

class Texture
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Texture(){}

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Texture();

	/// <summary>
	/// �t�@�C������e�N�X�`�������[�h����R���X�g���N�^
	/// </summary>
	/// <param name="filepath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
	explicit Texture(std::string filepath);

	/// <summary>
	/// PNG�t�@�C������e�N�X�`��������������B
	/// </summary>
	/// <param name="filepath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
	void TexInitFromPNG(std::string filepath);
	/// <summary>
	/// D3D���\�[�X����e�N�X�`��������������B
	/// </summary>
	/// <param name="resrource">D3D���\�[�X�B</param>
	void TexInitFromD3D_RESOURCE(ID3D12Resource* texture);

	/// <summary>
	/// SRV�ɓo�^�B
	/// </summary>
	/// <param name="descHandle"></param>
	void RegistSRV(D3D12_CPU_DESCRIPTOR_HANDLE descHandle, int bufferNo);

	/// <summary>
	/// �e�N�X�`�����L��������B
	/// </summary>
	/// <returns>true���Ԃ��Ă�����L���B</returns>
	bool IsValid() const
	{
		return m_texture != nullptr;
	}

	ID3D12Resource* Get()
	{
		return m_texture;
	}
	/// <summary>
	/// �e�N�X�`���̕����擾�B
	/// </summary>
	/// <returns></returns>
	int GetWidth() const
	{
		return static_cast<int>(m_texDesc.Width);
	}
	/// <summary>
	/// �e�N�X�`���̍������擾�B
	/// </summary>
	/// <returns></returns>
	int GetHeight() const
	{
		return static_cast<int>(m_texDesc.Height);
	}
	/// <summary>
	/// �e�N�X�`���̃t�H�[�}�b�g���擾�B
	/// </summary>
	/// <returns></returns>
	DXGI_FORMAT GetFormat() const
	{
		return m_texDesc.Format;
	}

private:
	/// <summary>
	/// PNG�t�@�C������e�N�X�`�������[�h�B
	/// </summary>
	/// <param name="filepath">�t�@�C���p�X�B</param>
	void LoadTexFromPNG(const wchar_t* filepath);

private:
	ID3D12Resource* m_texture = nullptr;	//�e�N�X�`���B
	D3D12_RESOURCE_DESC m_texDesc;	//�e�N�X�`�����
};
#include "InstBill.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace XIIlib;

ID3D12Device* InstBill::device = nullptr;
ID3D12GraphicsCommandList* InstBill::cmdList = nullptr;
ComPtr<ID3D12RootSignature> InstBill::rootSignature;
ComPtr<ID3D12PipelineState> InstBill::pipelineState;
DebugCamera* InstBill::d_camera = nullptr;

bool InstBill::StaticInitialize()
{
	InstBill::device = DirectX12::GetDevice();
	InstBill::d_camera = d_camera;

	CreateGraphicsPipelineState();

	return true;
}

void InstBill::PreDraw()
{
	assert(InstBill::cmdList == nullptr);
	// �R�}���h���X�g���Z�b�g
	InstBill::cmdList = DirectX12::GetCmdList();

	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void InstBill::PostDraw()
{
	// �R�}���h���X�g������
	InstBill::cmdList = nullptr;
}

InstBill* InstBill::Create(const std::string& textureFilename)
{
	InstBill* collisionbox = new InstBill(textureFilename);

	if (collisionbox == nullptr) {
		return nullptr;
	}

	return collisionbox;
}

// �ÓI�����o�ϐ��̎���
InstBill::InstBill(const std::string& textureFilename)
{
	device = DirectX12::GetDevice();

	if (!Initialize()) {
		assert(0);
	}

	if (!LoadTexture(textureFilename)) {
		assert(0);
	}
}

bool InstBill::LoadTexture(const std::string& textureFilename) {
	std::string path = textureFilename;
	// �e�N�X�`���Ȃ�
	if (path.size() == 0) {
		path = "white1x1.png";
	}

	cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	HRESULT result = S_FALSE;

	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	const std::string directoryPath = "Resources/";

	// �t�@�C���p�X������
	std::string filepath = directoryPath + path;
	wchar_t wfilepath[128];

	// ���j�R�[�h������ɕϊ�
	MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);

	// �e�N�X�`���p�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&texbuff));
	if (FAILED(result)) {
		assert(0);
	}

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);
	if (FAILED(result)) {
		assert(0);
	}

	// �V�F�[�_���\�[�X�r���[�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	device->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		cpuDescHandleSRV
	);

	return true;
}

void InstBill::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	plData->AddShader(XIILib::VS, "InstanceVS.hlsl");
	plData->AddShader(XIILib::PS, "InstancePS.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void InstBill::CreateVertexBuffer()
{
	HRESULT result;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertNum),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * vertNum;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void InstBill::CreateConstBuffer()
{
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CBVIDParameter) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));
}

bool InstBill::Initialize()
{
	HRESULT result;

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));

	if (FAILED(result)) {
		assert(0);
		return false;
	}

	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void InstBill::Update()
{
	assert(d_camera);

	CBVIDParameter* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->matbillboard = d_camera->GetMatBillboard();
		constMap->viewproj = d_camera->GetMatViewProjection();
		for (int i = 0; i < dataIndexCount; i++)
		{
			// ���[���h�s��̍X�V
			XMMATRIX matScale, matTrans;
			this->matWorld = XMMatrixIdentity();
			XMFLOAT4 colorMap = { 0,0,0,0 };

			if (iData.size() >= i + 1)
			{
				matScale = XMMatrixScaling(iData[i].scale, iData[i].scale, iData[i].scale);
				matTrans = XMMatrixTranslation(iData[i].position.x, iData[i].position.y, iData[i].position.z);
				this->matWorld *= matScale;
				this->matWorld *= matTrans;
				colorMap = iData[i].color;
				constMap->data[i].active = 1;
			}
			else if (iData.size() < i + 1)
			{
				constMap->data[i].active = 0;
			}

			constMap->data[i].mat = this->matWorld;
			constMap->data[i].color = colorMap;
		}

		this->constBuff->Unmap(0, nullptr);
	}
}

void InstBill::Draw()
{
	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandleSRV);
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// �`��R�}���h
	cmdList->DrawInstanced(vertNum, iData.size(), 0, 0);
	// �`�斈�Ƀf�[�^����ɂ��Ă���
	ClearData();
}

void XIIlib::InstBill::DrawBillBox(Math::Vector3 position, float scale, float r, float g, float b, float a)
{
	iData.push_back({ position,scale, { Math::Transform(255, 1,r),Math::Transform(255, 1,g),Math::Transform(255, 1,b),Math::Transform(255, 1,a) }});
}

void InstBill::TransferVertices()
{
	HRESULT result;

	std::vector<VertexPosUv> vertices;
	vertices.resize(vertNum);

	const float h = 0.5f;
	vertices[0].pos = { -h,	-h,	0.0f };
	vertices[1].pos = { -h,	 h,	0.0f };
	vertices[2].pos = {  h,	-h,	0.0f };
	vertices[3].pos = {  h,	 h,	0.0f };

	vertices[0].uv = { 0,1 }; // ����
	vertices[1].uv = { 0,0 }; // ����
	vertices[2].uv = { 1,1 }; // �E��
	vertices[3].uv = { 1,0 }; // �E��

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void XIIlib::InstBill::ReLoadTexture(const std::string& textureFilename)
{
	// ������x�ǂݍ���
	LoadTexture(textureFilename);
}
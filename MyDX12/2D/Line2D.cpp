#include "Line2D.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Math;

ID3D12Device* Line2D::device = nullptr;
ID3D12GraphicsCommandList* Line2D::cmdList = nullptr;
ComPtr<ID3D12RootSignature> Line2D::rootSignature;
ComPtr<ID3D12PipelineState> Line2D::pipelineState;
DirectX::XMMATRIX Line2D::matProjection;
ComPtr<ID3D12DescriptorHeap> Line2D::descHeap;

bool Line2D::StaticInitialize()
{
	Line2D::device = DirectX12::GetDevice();

	CreateGraphicsPipelineState();

	// �ˉe�s��v�Z
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, WindowsApp::window_width,
		WindowsApp::window_height, 0.0f,
		0.0f, 1.0f);

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

	return true;
}

void Line2D::PreDraw()
{
	assert(Line2D::cmdList == nullptr);
	// �R�}���h���X�g���Z�b�g
	Line2D::cmdList = DirectX12::GetCmdList();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
}

void Line2D::PostDraw()
{
	// �R�}���h���X�g������
	Line2D::cmdList = nullptr;
}

Line2D* Line2D::Create(Math::Vector2 start, Math::Vector2 end)
{
	Line2D* line = new Line2D(start, end);

	if (line == nullptr) {
		return nullptr;
	}

	// ������
	if (!line->Initialize()) {
		delete line;
		assert(0);
		return nullptr;
	}

	return line;
}

// �ÓI�����o�ϐ��̎���
Line2D::Line2D(Math::Vector2 start, Math::Vector2 end)
	:start(start), end(end)
{
	position = { 0,0 };
	device = DirectX12::GetDevice();
}

void Line2D::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1, XIILib::CULL_NONE, XIILib::MASK_DEBUG, XIILib::COMP_ALWAYS);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });

	plData->AddShader(XIILib::VS, "Line2DVertexShader.hlsl");
	plData->AddShader(XIILib::PS, "Line2DPixelShader.hlsl");

	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void Line2D::CreateVertexBuffer()
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

void Line2D::CreateConstBuffer()
{
	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(CbData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	device->CreateConstantBufferView(&cbvDesc, descHeap->GetCPUDescriptorHandleForHeapStart());
}

bool Line2D::Initialize()
{
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateConstBuffer();

	return true;
}

void Line2D::Draw()
{
	// ���[���h�s��̍X�V
	this->matWorld = XMMatrixIdentity();
	this->matWorld *= XMMatrixRotationZ(rotation);
	this->matWorld *= XMMatrixTranslation(position.x, position.y, 0.0f);

	// �萔�o�b�t�@�Ƀf�[�^�]��
	CbData* constMap = nullptr;
	HRESULT result = this->constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->mat = this->matWorld * matProjection;
		constMap->xm_start = { start.x,start.y };
		constMap->xm_end = { end.x,end.y };
		this->constBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	//cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());
	// �`��R�}���h
	cmdList->DrawInstanced(2, 1, 0, 0);
}

void Line2D::TransferVertices()
{
	HRESULT result;

	// ���_�f�[�^
	VertexPosUv vertices[vertNum] = {
		{{start,0.0f}}, // ����
		{{end,	0.0f}}, // ����
	};

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}
}
#include "CollCircle.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* CollisionCircle::device = nullptr;
ID3D12GraphicsCommandList*  CollisionCircle::cmdList = nullptr;
ComPtr<ID3D12RootSignature> CollisionCircle::rootSignature;
ComPtr<ID3D12PipelineState> CollisionCircle::pipelineState;
XMMATRIX CollisionCircle::matProjection;
ComPtr<ID3D12DescriptorHeap> CollisionCircle::descHeap;
DebugCamera* CollisionCircle::d_camera = nullptr;

bool CollisionCircle::StaticInitialize()
{
	CollisionCircle::device = DirectX12::GetDevice();
	CollisionCircle::d_camera = d_camera;

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

void CollisionCircle::PreDraw()
{
	assert(CollisionCircle::cmdList == nullptr);
	// �R�}���h���X�g���Z�b�g
	CollisionCircle::cmdList = DirectX12::GetCmdList();

	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CollisionCircle::PostDraw()
{
	// �R�}���h���X�g������
	CollisionCircle::cmdList = nullptr;
}

CollisionCircle* CollisionCircle::Create(float radius, int v_count)
{
	CollisionCircle* collisionCircle = new CollisionCircle(radius,v_count);

	if (collisionCircle== nullptr) {
		return nullptr;
	}

	// ������
	if (!collisionCircle->Initialize()) {
		delete collisionCircle;
		assert(0);
		return nullptr;
	}

	return collisionCircle;
}

// �ÓI�����o�ϐ��̎���
CollisionCircle::CollisionCircle(float radius, int v_count)
	:radius(radius),vert_count(v_count)
{
	position = { 0,0 ,0 };
	device = DirectX12::GetDevice();
}

void CollisionCircle::CreateGraphicsPipelineState()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline(1);
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });

	plData->AddShader(XIILib::VS, "CapsuleVS.hlsl");
	plData->AddShader(XIILib::PS, "CapsulePS.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);
	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV, XIILib::SRV_MULTI_ONE);

	plData->Init();

	rootSignature = plData->GetRootSignature();
	pipelineState = plData->GetPipelineState();

	delete plData;
}

void CollisionCircle::CreateVertexBuffer()
{
	HRESULT result;

	// ���_��
	int vertices_count = vert_count + 1;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * vertices_count),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * vertices_count;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void CollisionCircle::CreateConstBuffer()
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

bool CollisionCircle::Initialize()
{
	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateIndexBuffer();

	CreateConstBuffer();

	return true;
}

void CollisionCircle::Update()
{
	assert(d_camera);

	XMMATRIX matScl,matTrans;
	matScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScl;
	matWorld *= matTrans; // ���[���h�s��ɕ��s�ړ��𔽉f

	const XMMATRIX& matViewProjection = d_camera->GetMatViewProjection();

	// �萔�o�b�t�@�Ƀf�[�^�]��
	CbData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		constMap->mat = matWorld;
		constMap->viewproj = matViewProjection;
		constMap->color = color;
		constBuff->Unmap(0, nullptr);
	}
}

void CollisionCircle::Draw()
{
	Update();

	// �p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineState.Get());
	// ���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// �萔�o�b�t�@�r���[���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, this->constBuff->GetGPUVirtualAddress());

	ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
	// �f�X�N���v�^�q�[�v���Z�b�g
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// ���_�o�b�t�@�̐ݒ�
	cmdList->IASetVertexBuffers(0, 1, &this->vbView);
	// �C���f�b�N�X�o�b�t�@���Z�b�g
	cmdList->IASetIndexBuffer(&ibView);
	// �C���f�b�N�X��
	int index_num = vert_count * 3;
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)index_num, 1, 0, 0, 0);
}

namespace
{
	Math::Vector3 SetVertex(const Math::Vector3& o, const Math::Vector3& v)
	{
		Math::Vector3 verticle = { o + v };
		return verticle;
	}
}

void CollisionCircle::TransferVertices()
{
	HRESULT result;

	// ���_�o�b�t�@�̍쐬
	std::vector<VertexPosUv> vertices;
	vertices.reserve(vert_count + 1);
	const float PI = 3.14f;
	for (int i = 0; i < vert_count; i++)
	{
		float PI2 = PI * 2;
		Math::Vector3 point = {
			radius * sinf(PI2 / vert_count * i),
			0.0f,radius * cosf(PI2 / vert_count * i)
		};
		vertices.push_back({point});

	}
	vertices.push_back({ Math::Vector3(0,0,0) });// ���S

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void CollisionCircle::CreateIndexBuffer()
{
	HRESULT result;
	// ���_��
	int index_num = vert_count * 3;
	
	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * index_num),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	std::vector<unsigned short> indices;
	indices.reserve(index_num);
	for (int i = 0; i < vert_count; i++)
	{
		indices.push_back(vert_count);
		indices.push_back(i);
		indices.push_back(i + 1);
	}
	indices[index_num - 1] = 0;

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(unsigned short) * index_num;
}

void CollisionCircle::SetMesh()
{
	TransferVertices();
	CreateIndexBuffer();
}

void CollisionCircle::SetVertexCount(int count_v)
{
	vert_count = count_v;
	SetMesh();
}

#include "CollCapsule.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"
#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* CollisionCapsule::device = nullptr;
ID3D12GraphicsCommandList* CollisionCapsule::cmdList = nullptr;
ComPtr<ID3D12RootSignature> CollisionCapsule::rootSignature;
ComPtr<ID3D12PipelineState> CollisionCapsule::pipelineState;
XMMATRIX CollisionCapsule::matProjection;
ComPtr<ID3D12DescriptorHeap> CollisionCapsule::descHeap;
DebugCamera* CollisionCapsule::d_camera = nullptr;

bool CollisionCapsule::StaticInitialize()
{
	CollisionCapsule::device = DirectX12::GetDevice();
	CollisionCapsule::d_camera = d_camera;

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

void CollisionCapsule::PreDraw()
{
	assert(CollisionCapsule::cmdList == nullptr);
	// �R�}���h���X�g���Z�b�g
	CollisionCapsule::cmdList = DirectX12::GetCmdList();

	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CollisionCapsule::PostDraw()
{
	// �R�}���h���X�g������
	CollisionCapsule::cmdList = nullptr;
}

CollisionCapsule* CollisionCapsule::Create(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices)
{
	CollisionCapsule* collisionCapsule = new CollisionCapsule(start, end,radius,slices,slices);

	if (collisionCapsule == nullptr) {
		return nullptr;
	}

	// ������
	if (!collisionCapsule->Initialize()) {
		delete collisionCapsule;
		assert(0);
		return nullptr;
	}

	return collisionCapsule;
}

// �ÓI�����o�ϐ��̎���
CollisionCapsule::CollisionCapsule(const Math::Vector3& start, const Math::Vector3& end, float radius, int slices, int stacks_1_2)
	:start(start), end(end),radius(radius),slices(slices),stacks_1_2(stacks_1_2)
{
	position = { 0,0 ,0 };
	device = DirectX12::GetDevice();
}

void CollisionCapsule::CreateGraphicsPipelineState()
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

void CollisionCapsule::CreateVertexBuffer()
{
	HRESULT result;

	// ���_��
	int numberOfVB = (1 + slices * stacks_1_2) * 2;

	// ���_�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * numberOfVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	TransferVertices();

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(VertexPosUv) * numberOfVB;
	vbView.StrideInBytes = sizeof(VertexPosUv);
}

void CollisionCapsule::CreateConstBuffer()
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

bool CollisionCapsule::Initialize()
{
	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateIndexBuffer();

	CreateConstBuffer();

	return true;
}

void CollisionCapsule::Update()
{
	assert(d_camera);

	XMMATRIX matTrans;
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
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

void CollisionCapsule::Draw()
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
	int numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	int numberOfIB = numberOfTriangle * 3;
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)numberOfIB, 1, 0, 0, 0);
}

namespace
{
	Math::Vector3 SetVertex(const Math::Vector3& o, const Math::Vector3& v)
	{
		Math::Vector3 verticle = { o + v };
		return verticle;
	}
}

void CollisionCapsule::TransferVertices()
{
	HRESULT result;

	// �������������Ȓl�łȂ����`�F�b�N
	if (slices <= 0)assert(0);
	float invSlices = 1.0f / slices;
	if (stacks_1_2 <= 0)assert(0);
	float invStacks = 1.0f / stacks_1_2;
	// ���a�������Ȓl�łȂ����`�F�b�N
	if (radius <= 0.0f)assert(0);

	// �J�v�Z���̒P�ʃx�N�g���쐬
	Math::Vector3 uz = end - start;// vz : P2����P1�֌����������̃x�N�g��
	float length = uz.dot(uz);// �����炭��悵�Ȃ���΂Ȃ�Ȃ�
	if (length == 0.0f)assert(0);

	// ���u���̃x�N�g��
	Math::Vector3 v = { 1,1,0 };

	Math::Vector3 uy;
	uy = uz.cross(v);
	length = uy.length();
	if (length == 0.0f)assert(0);

	Math::Vector3 ux;
	ux = uz.cross(uy);

	// ux�̐��K��
	ux = ux.normalize();
    // ux��-���a��������ux�Ɋi�[
    ux = ux * -radius;
    // uy�̐��K��
	uy = uy.normalize();
    // uy�ɔ��a��������ux�Ɋi�[
	uy = uy * radius;
    // uz�̐��K��
	uz = uz.normalize();
    // uz�ɔ��a��������ux�Ɋi�[
	uz = uz * radius;

	//���_�o�b�t�@�̐����擾
	int numberOfVB = (1 + slices * stacks_1_2) * 2;
	// ���_�o�b�t�@�̍쐬
	std::vector<VertexPosUv> vertices;
	vertices.reserve(numberOfVB);

	vertices.push_back({ SetVertex(end, uz) });
	const float PI = 3.14f;
	for (int i = 0; i < stacks_1_2; i++)
	{// PI = ��
		float t = (PI * 0.5f) * invSlices * (i + 1);
		float r = sinf(t);
		Math::Vector3 vx = ux * r;
		Math::Vector3 vy = uy * r;
		Math::Vector3 vz = uz * cosf(t);
		for (int j = 0; j < slices; j++)
		{
			float u = (PI * 2.0f) * invSlices * j;
			vertices.push_back({ SetVertex(end, vx * cosf(u) + vy * sinf(u) + vz) });
		}
	}

	//P2���̌v�Z
	for (int i = 0; i < stacks_1_2; i++)
	{// PI = ��
		float t = (PI * 0.5f) * invSlices * i;
		float r = cosf(t);
		Math::Vector3 vx = ux * r;
		Math::Vector3 vy = uy * r;
		Math::Vector3 vz = uz * -sinf(t);
		for (int j = 0; j < slices; j++)
		{
			float u = (PI * 2.0f) * invSlices * j;
			vertices.push_back({ SetVertex(start, vx * cosf(u) + vy * sinf(u) + vz) });
		}
	}
	vertices.push_back({ SetVertex(start, -uz) });

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void CollisionCapsule::CreateIndexBuffer()
{
	HRESULT result;
	int numberOfTriangle = slices * stacks_1_2 * 2 * 2;
	int numberOfIB = numberOfTriangle * 3;
	int numberOfVB = (1 + slices * stacks_1_2) * 2;
	int sizeIB = static_cast<UINT>(sizeof(unsigned short) * numberOfIB);
	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	std::vector<unsigned short> indices;
	indices.reserve(numberOfIB);

	// P1���[�_����̌v�Z
	for (int j = 0; j < slices; j++)
	{
		indices.push_back(0);
		indices.push_back(1 + j);
		indices.push_back(1 + (j + 1) % slices);
	}

	// P1-P2�Ԃ̌v�Z
	int imax = stacks_1_2 * 2 - 1;
	for (int i = 0; i < imax; i++)
	{
		int ibase = 1 + slices * i;
		for (int j = 0; j < slices; j++)
		{
			int jnext = (j + 1) % slices;
			indices.push_back(ibase + j);
			indices.push_back(ibase + j + slices);
			indices.push_back(ibase + jnext);

			indices.push_back(ibase + jnext);
			indices.push_back(ibase + j + slices);
			indices.push_back(ibase + jnext + slices);
		}
	}

	// P2���[�_����̌v�Z
	int ibase = 1 + slices * imax;
	int ilast = numberOfVB - 1;
	for (int j = 0; j < slices; j++)
	{
		indices.push_back(ibase + j);
		indices.push_back(ilast);
		indices.push_back(ibase + (j + 1) % slices);
	}

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
	ibView.SizeInBytes = sizeIB;
}

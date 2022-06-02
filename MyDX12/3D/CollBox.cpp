#include "CollBox.h"
#include "../Base/WindowsApp.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include "../Pipeline/Pipeline.h"
#include "../Camera/DebugCamera.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

ID3D12Device* CollisionBox::device = nullptr;
ID3D12GraphicsCommandList* CollisionBox::cmdList = nullptr;
ComPtr<ID3D12RootSignature> CollisionBox::rootSignature;
ComPtr<ID3D12PipelineState> CollisionBox::pipelineState;
XMMATRIX CollisionBox::matProjection;
ComPtr<ID3D12DescriptorHeap> CollisionBox::descHeap;
DebugCamera* CollisionBox::d_camera = nullptr;

bool CollisionBox::StaticInitialize()
{
	CollisionBox::device = DirectX12::GetDevice();
	CollisionBox::d_camera = d_camera;

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

void CollisionBox::PreDraw()
{
	assert(CollisionBox::cmdList == nullptr);
	// �R�}���h���X�g���Z�b�g
	CollisionBox::cmdList = DirectX12::GetCmdList();

	// �v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void CollisionBox::PostDraw()
{
	// �R�}���h���X�g������
	CollisionBox::cmdList = nullptr;
}

CollisionBox* CollisionBox::Create(const Math::Vector3& start, const Math::Vector3& end)
{
	CollisionBox* collisionbox = new CollisionBox(start, end);

	if (collisionbox == nullptr) {
		return nullptr;
	}

	// ������
	if (!collisionbox->Initialize()) {
		delete collisionbox;
		assert(0);
		return nullptr;
	}

	return collisionbox;
}

// �ÓI�����o�ϐ��̎���
CollisionBox::CollisionBox(const Math::Vector3& start, const Math::Vector3& end)
	:start(start), end(end)
{
	position = { 0,0 ,0 };
	scale = { 1,1,1 };
	rotation = { 0,0,0 };
	device = DirectX12::GetDevice();
}

void CollisionBox::CreateGraphicsPipelineState()
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

void CollisionBox::CreateVertexBuffer()
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

void CollisionBox::CreateConstBuffer()
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

bool CollisionBox::Initialize()
{
	color = { 1,1,1,1 };
	CreateGraphicsPipelineState();

	CreateVertexBuffer();

	CreateIndexBuffer();

	CreateConstBuffer();

	return true;
}

void CollisionBox::Update()
{
	assert(d_camera);

	XMMATRIX matScl,matRot,matTrans;
	matScl = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // �ό`�����Z�b�g
	matWorld *= matScl;
	matWorld *= matRot; // ���[���h�s��ɉ�]�𔽉f
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

void CollisionBox::Draw()
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
	// �C���f�b�N�X�̗v�f�����擾
	const int index_count = sizeof(unsigned short) * (vertNum / 2) * 3;
	// �`��R�}���h
	cmdList->DrawIndexedInstanced((UINT)index_count, 1, 0, 0, 0);
}

namespace
{
	Math::Vector3 SetVertex(const Math::Vector3& o, const Math::Vector3& v)
	{
		Math::Vector3 verticle = { o + v };
		return verticle;
	}
}

void CollisionBox::TransferVertices()
{
	HRESULT result;
	//     p5--------p7
	//   �^|       �^|
	// p1--------p3  |
	// |   |     |   |
	// |   p4----|---p6
	// | �^      | �^
	// p0--------p2
	Math::Vector3 p0 = start;	// min
	Math::Vector3 p1 = Math::Vector3(start.x,end.y,  start.z);
	Math::Vector3 p2 = Math::Vector3(end.x,  start.y,start.z);
	Math::Vector3 p3 = Math::Vector3(end.x, end.y,start.z);
	Math::Vector3 p4 = Math::Vector3(start.x,start.y,end.z);
	Math::Vector3 p5 = Math::Vector3(start.x,end.y,  end.z);
	Math::Vector3 p6 = Math::Vector3(end.x,  start.y,end.z);
	Math::Vector3 p7 = end;		// max

	// p0,p1,p2,p3 �O
	// p3,p7,p2,p6 �E
	// p6,p7,p4,p5 ��
	// p5,p1,p4,p0 ��
	// p0,p2,p4,p6 ��
	// p1,p3,p5,p7 ��

	std::vector<VertexPosUv> vertices;
	vertices.reserve(vertNum);
	// �O
	vertices.push_back({ p0 });//0
	vertices.push_back({ p1 });//1
	vertices.push_back({ p2 });//2
	vertices.push_back({ p3 });//3
	// ��
	vertices.push_back({ p4 });//4
	vertices.push_back({ p5 });//5
	vertices.push_back({ p6 });//6
	vertices.push_back({ p7 });//7
	// ��
	vertices.push_back({ p4 });//8
	vertices.push_back({ p5 });//9
	vertices.push_back({ p0 });//10
	vertices.push_back({ p1 });//11
	// �E
	vertices.push_back({ p2 });//12
	vertices.push_back({ p3 });//13
	vertices.push_back({ p6 });//14
	vertices.push_back({ p7 });//15
	// ��
	vertices.push_back({ p1 });//16
	vertices.push_back({ p5 });//17
	vertices.push_back({ p3 });//18
	vertices.push_back({ p7 });//19
	// ��
	vertices.push_back({ p4 });//20
	vertices.push_back({ p0 });//21
	vertices.push_back({ p6 });//22
	vertices.push_back({ p2 });//23
	// �v24��

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}
}

void CollisionBox::CreateIndexBuffer()
{
	HRESULT result;
	// �C���f�b�N�X�̗v�f�����擾
	const int index_count = sizeof(unsigned short) * (vertNum / 2) * 3;

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(unsigned short) * index_count),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	std::vector<unsigned short> indices;
	indices.reserve(index_count);
	// �C���f�b�N�X�̌X�̗v�f���i�[(LIST)
	indices.push_back(1); indices.push_back(2); indices.push_back(0);
	indices.push_back(1); indices.push_back(3); indices.push_back(2);

	indices.push_back(7); indices.push_back(4); indices.push_back(6);
	indices.push_back(7); indices.push_back(5); indices.push_back(4);

	indices.push_back(9); indices.push_back(10); indices.push_back(8);
	indices.push_back(9); indices.push_back(11); indices.push_back(10);

	indices.push_back(13); indices.push_back(14); indices.push_back(12);
	indices.push_back(13); indices.push_back(15); indices.push_back(14);

	indices.push_back(17); indices.push_back(18); indices.push_back(16);
	indices.push_back(17); indices.push_back(19); indices.push_back(18);

	indices.push_back(21); indices.push_back(22); indices.push_back(20);
	indices.push_back(21); indices.push_back(23); indices.push_back(22);

	// STRIP��(�v�f�� : 24)
	//indices.push_back(0); indices.push_back(1); indices.push_back(2); indices.push_back(3);
	//indices.push_back(13); indices.push_back(15); indices.push_back(12); indices.push_back(14);
	//indices.push_back(6); indices.push_back(7); indices.push_back(4); indices.push_back(5);
	//indices.push_back(9); indices.push_back(11); indices.push_back(8); indices.push_back(10);
	//indices.push_back(21); indices.push_back(23); indices.push_back(20); indices.push_back(22);
	//indices.push_back(16); indices.push_back(18); indices.push_back(17); indices.push_back(19);
	// 0,1,2,3
	// 13,15,12,14
	// 6,7,4,5
	// 9,11,8,10
	// 21,23,20,22
	// 16,18,17,19


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
	ibView.SizeInBytes = sizeof(unsigned short) * index_count;
}

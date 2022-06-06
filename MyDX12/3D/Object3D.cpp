#include "Object3D.h"
#include "../Camera/DebugCamera.h"
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Object3D::device = nullptr;
ID3D12GraphicsCommandList* Object3D::cmdList = nullptr;
Object3D::PipelineSet Object3D::pipelineSet;
DebugCamera* Object3D::d_camera = nullptr;
LightGroup* Object3D::lightGroup = nullptr;

XMFLOAT3 TransXMF3(Math::Vector3 vec)
{
	XMFLOAT3 p = { vec.x,vec.y,vec.z };
	return p;
}

void Object3D::StaticInitialize()
{
	Object3D::device = DirectX12::GetDevice();
	Object3D::d_camera = d_camera;

	// グラフィックパイプラインの生成
	CreateGraphicsPipeline();

	// モデルの静的初期化
	Model::StaticInitialize();
}

void Object3D::CreateGraphicsPipeline()
{
	XIILib::GraphicsPipeline* plData = new XIILib::GraphicsPipeline();
	plData->AddInputLayout({ "POSITION",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "NORMAL",XIILib::RGB_FLOAT });
	plData->AddInputLayout({ "TEXCOORD",XIILib::RG_FLOAT });

	plData->AddShader(XIILib::VS, "OBJVertexShader.hlsl");
	plData->AddShader(XIILib::PS, "OBJPixelShader.hlsl");
	plData->AddRange(XIILib::SRV, 1, 0);

	plData->AddRootParam(0, XIILib::CBV);
	plData->AddRootParam(1, XIILib::CBV);
	plData->AddRootParam(1, XIILib::SRV,XIILib::SRV_MULTI_ONE);
	plData->AddRootParam(2, XIILib::CBV);

	plData->Init();

	pipelineSet.rootsignature = plData->GetRootSignature();
	pipelineSet.pipelinestate = plData->GetPipelineState();

	delete plData;
}

void Object3D::PreDraw()
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(Object3D::cmdList == nullptr);

	// コマンドリストをセット
	Object3D::cmdList = DirectX12::GetCmdList();

	// プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Object3D::PostDraw()
{
	// コマンドリストを解除
	Object3D::cmdList = nullptr;
}

Object3D* Object3D::Create(Model* model, float elec_is)
{
	// 3Dオブジェクトのインスタンスを生成
	Object3D* object3D = new Object3D();
	if (object3D == nullptr)
	{return nullptr;}

	// 初期化
	if (!object3D->Initialize())
	{
		delete object3D;
		assert(0);
	}

	object3D->SetShaderType(elec_is);

	if (model)
	{object3D->SetModel(model);}

	return object3D;
}

Object3D::~Object3D()
{
}

bool Object3D::Initialize()
{
	// nullptrチェック
	assert(device);

	HRESULT result;

	// クラス名の文字列を取得
	name = typeid(*this).name();

	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));

	return true;
}

void Object3D::Update()
{
	HRESULT result;
	
	// 行列の更新
	UpdateWorldMatrix();

	const XMMATRIX& matProjection = d_camera->GetMatProjection();
	const XMMATRIX& matViewProjection = d_camera->GetMatViewProjection();
	const XMFLOAT3& cameraPos = TransXMF3({ d_camera->GetEye_X(),d_camera->GetEye_Y(),d_camera->GetEye_Z() });

	XMMATRIX m_lightTran;

	m_lightTran = XMMatrixTranslation(lightGroup->GetLightPos().x, lightGroup->GetLightPos().y, lightGroup->GetLightPos().z);

	XMMATRIX m;
	m = XMMatrixRotationX(XMConvertToRadians(90));
	m = m * m_lightTran;

	XMMATRIX m_lightView;
	m_lightView = XMMatrixInverse(nullptr, m);

	shaderTimer += (1.0f / 60.0f);
	if (shaderTimer > 1.0f) {
		shaderTimer = 0.0f;
	}

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->mat = matWorld * matViewProjection;	// 行列の合成
	constMap->color = { color.x,color.y,color.z,alpha };
	constMap->viewproj = matViewProjection;
	constMap->world = matWorld;
	constMap->cameraPos = cameraPos;
	constMap->lightpos = XMFLOAT4(lightGroup->GetLightPos().x, lightGroup->GetLightPos().y, lightGroup->GetLightPos().z, 1);
	constMap->wlp = XMMatrixTranspose(matWorld * m_lightView * matProjection);

	constMap->_time = shaderTimer;
	constMap->isElec = _isElec;

	XMMATRIX clip;
	ZeroMemory(&clip, sizeof(XMMATRIX));
	clip.r[0].m128_f32[0] = 0.5;
	clip.r[1].m128_f32[1] = -0.5;
	clip.r[2].m128_f32[2] = 1;
	clip.r[3].m128_f32[0] = 0.5;
	clip.r[3].m128_f32[1] = 0.5;
	clip.r[3].m128_f32[3] = 1;

	constMap->wlpt = XMMatrixTranspose(matWorld * m_lightView * matProjection * clip);
	constBuffB0->Unmap(0, nullptr);

}

void Object3D::Draw()
{
	// nullptrチェック
	assert(device);
	assert(Object3D::cmdList);

	// モデルの割り当てがなければ描画しない
	if (model == nullptr) {
		return;
	}

	// パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	// ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//// ライトの描画
	lightGroup->Draw(cmdList, 3);
	// モデル描画
	model->Draw();
}

void Object3D::UpdateWorldMatrix()
{
	assert(d_camera);

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	// ワールド行列の合成
	if (isBillboard && d_camera) {
		const XMMATRIX& matBillboard = d_camera->GetMatBillboard();

		matWorld = XMMatrixIdentity();
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matBillboard;
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}
	else {
		matWorld = XMMatrixIdentity(); // 変形をリセット
		matWorld *= matScale; // ワールド行列にスケーリングを反映
		matWorld *= matRot; // ワールド行列に回転を反映
		matWorld *= matTrans; // ワールド行列に平行移動を反映
	}

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}
}
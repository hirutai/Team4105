#include "Pipeline.h"

#pragma comment(lib,"d3dcompiler.lib")
using namespace std;
using namespace Microsoft::WRL;

XIILib::GraphicsPipeline::GraphicsPipeline(int numRTarget,XII_CULLMODE cullMode, XII_MASK mask, XII_COMPARISON comp, RTARGET_TYPE rTarget)
	:numRenderTarget(numRTarget),cullType(cullMode),maskType(mask),compType(comp),rtType(rTarget)
{
	cout << "through!!" << endl;
}

bool XIILib::GraphicsPipeline::CheckShaderCompileResult(HRESULT result, ID3DBlob* error)
{
	if (FAILED(result)) {
		if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
		{
			::OutputDebugStringA("ファイルが見当たりません");
		}
		else
		{
			std::string errstr;
			errstr.resize(error->GetBufferSize());

			std::copy_n((char*)error->GetBufferPointer(),
				error->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
		}
		return false;
	}
	else
	{
		return true;
	}
}

void XIILib::GraphicsPipeline::Init()
{
	// 頂点レイアウト
	vector<D3D12_INPUT_ELEMENT_DESC> inputLayouts;
	for (auto& x : numILayout)
	{
		DXGI_FORMAT formatType;
		if (x.format == R_FLOAT)
		{
			formatType = DXGI_FORMAT_R32_FLOAT;
		}
		else if(x.format == RG_FLOAT)
		{
			formatType = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (x.format == RGB_FLOAT)
		{
			formatType = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (x.format == RGBA_FLOAT)
		{
			formatType = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (x.format == RGBA_UINT)
		{
			formatType = DXGI_FORMAT_R32G32B32A32_UINT;
		}
		inputLayouts.push_back(
			{
				x.semantName.c_str(),0,
				formatType,0,D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
		);
	}

	HRESULT result = S_FALSE;
	// シェーダ―読み込み
	ComPtr<ID3DBlob> vsBlob, psBlob, gsBlob;
	ComPtr<ID3DBlob> errorBlob;
	const string directPath = "Resources/Shaders/";
	string path;

	// VS
	if (shadeData.count(VS))
	{

		path = directPath + shadeData[VS].shadeFileName;
		result = D3DCompileFromFile(
			WcToString(path),    // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			shadeData[VS].shadeFuncName.c_str(), "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&vsBlob, &errorBlob);
		if (!CheckShaderCompileResult(result, errorBlob.Get())) {
			assert(0);
		}
	}
	// PS
	if (shadeData.count(PS))
	{
		if (shadeData[PS].shadeFileName != "")
		{
			path = directPath + shadeData[PS].shadeFileName;

			result = D3DCompileFromFile(
				WcToString(path),    // シェーダファイル名
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
				shadeData[PS].shadeFuncName.c_str(), "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
				0,
				&psBlob, &errorBlob);
			if (!CheckShaderCompileResult(result, errorBlob.Get())) {
				assert(0);
			}
		}
	}
	// GS
	if (shadeData.count(GS))
	{
		path = directPath + shadeData[GS].shadeFileName;

		result = D3DCompileFromFile(
			WcToString(path),    // シェーダファイル名
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
			shadeData[GS].shadeFuncName.c_str(), "gs_5_0",    // エントリーポイント名、シェーダーモデル指定
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
			0,
			&gsBlob, &errorBlob);
		if (!CheckShaderCompileResult(result, errorBlob.Get())) {
			assert(0);
		}
	}
	
	D3D12_GRAPHICS_PIPELINE_STATE_DESC datas{};
	datas.NumRenderTargets = numRenderTarget;// 描画対象
	for (int i = 0; i < numRenderTarget; i++)
	{
		datas.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	if(shadeData.count(VS))datas.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	if (shadeData.count(PS))
	{
		if (shadeData[PS].shadeFileName != "")
		{
			datas.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
		}
		else
		{
			datas.PS.BytecodeLength = 0;
			datas.PS.pShaderBytecode = nullptr;
			datas.NumRenderTargets = 0;
			datas.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			datas.RTVFormats[1] = DXGI_FORMAT_UNKNOWN;
			datas.RTVFormats[2] = DXGI_FORMAT_UNKNOWN;
		}
	}
	if(shadeData.count(GS))datas.GS = CD3DX12_SHADER_BYTECODE(gsBlob.Get());

	datas.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	datas.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	// Init案件
	if(cullType == CULL_NONE)datas.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	datas.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	// Init
	if(compType == COMP_ALWAYS)datas.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	// Init
	if(maskType == MASK_ZERO)datas.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	if (shadeData.count(GS))
	{
		blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
		blenddesc.SrcBlend = D3D12_BLEND_ONE;
		blenddesc.DestBlend = D3D12_BLEND_ONE;
	}

	// ブレンドステートの設定
	// 生成したブレンド設定を入れてく
	// numRT Init
	for (int i = 0; i < numRenderTarget; i++)
	{
		datas.BlendState.RenderTarget[i] = blenddesc;
	}

	datas.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	datas.InputLayout.pInputElementDescs = inputLayouts.data();
	datas.InputLayout.NumElements = inputLayouts.size();

	datas.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	if (shadeData.count(GS)) datas.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	
	datas.SampleDesc.Count = 1;

	ID3D12Device* dev = DirectX12::GetDevice();

	//CD3DX12_ROOT_PARAMETER rootparams[5];
	// ルートパラメータ生成
	vector<CD3DX12_ROOT_PARAMETER> rootparams;
	rootparams.resize(rootParam.size());
	// Rangeの設定
	vector<CD3DX12_DESCRIPTOR_RANGE> descRange;
	descRange.resize(ranges.size());
	for (int i = 0; i < ranges.size(); i++)
	{
		if (ranges[i].rangeType == SRV)
		{
			descRange[i].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, ranges[i].numDescriptor, ranges[i].baseShaderRegister);
		}
	}

	// ルートパラメータの設定
	for (int i = 0; i < rootParam.size(); i++)
	{
		if (rootParam[i].rangeType == CBV)
		{
			rootparams[i].InitAsConstantBufferView(rootParam[i].sRegister, 0, D3D12_SHADER_VISIBILITY_ALL);
		}
		else if(rootParam[i].rangeType == SRV)
		{
			if (rootParam[i].setType == SRV_SINGLE_ONE)
			{
				rootparams[i].InitAsDescriptorTable(rootParam[i].sRegister, &descRange[i-1], D3D12_SHADER_VISIBILITY_ALL);
			}
			else if (rootParam[i].setType == SRV_MULTI_ONE)
			{
				rootparams[i].InitAsDescriptorTable(descRange.size(), descRange.data(), D3D12_SHADER_VISIBILITY_ALL);
			}
		}
	}

	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
	if (rtType == RT_ON)
	{
		samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);
	}


	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(rootparams.size(), rootparams.data(), 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));

	datas.pRootSignature = rootsignature.Get();

	result = dev->CreateGraphicsPipelineState(&datas, IID_PPV_ARGS(&pipelinestate));
	assert(SUCCEEDED(result));
}

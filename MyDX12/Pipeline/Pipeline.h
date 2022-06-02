#pragma once
#include "../Base/DirectX12.h"
#include <d3dcompiler.h>
#include <string>
#include <vector>
#include <map>

namespace XIILib
{
	enum GI_FORMAT_DATA
	{
		R_FLOAT,
		RG_FLOAT,
		RGB_FLOAT,
		RGBA_FLOAT,
		RGBA_UINT
	};

	enum XII_CULLMODE
	{
		CULL_DEFAULT,
		CULL_NONE
	};

	enum XII_MASK
	{
		MASK_DEBUG = -1,
		MASK_ZERO,
		MASK_ALL
	};

	enum XII_COMPARISON
	{
		COMP_DEFAULT,
		COMP_ALWAYS
	};

	enum SHADE_TYPE
	{
		VS,
		PS,
		GS,
		HS,
		DS
	};

	enum RANGE_TYPE
	{
		CBV,
		SRV
	};

	enum RTARGET_TYPE
	{
		RT_OFF,
		RT_ON
	};

	enum SRV_ST_TYPE
	{
		SRV_NONE,
		SRV_SINGLE_ONE,
		SRV_MULTI_ONE
	};

	struct INPUT_LAYOUT
	{
		std::string semantName;
		GI_FORMAT_DATA format;
	};

	struct XII_RANGE
	{
		RANGE_TYPE rangeType;
		UINT numDescriptor;
		UINT baseShaderRegister;
	};

	struct XII_ROOT_PARAM
	{
		UINT sRegister;
		RANGE_TYPE rangeType;
		SRV_ST_TYPE setType;
	};

	struct ShaderInfo
	{
		std::string shadeFileName;
		std::string shadeFuncName;
	};

	class GraphicsPipeline
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
		std::vector<INPUT_LAYOUT> numILayout;
		std::map<SHADE_TYPE, ShaderInfo> shadeData;
		std::vector<XII_RANGE> ranges;
		std::vector<XII_ROOT_PARAM> rootParam;
		XII_CULLMODE cullType = CULL_DEFAULT;
		XII_MASK maskType = MASK_DEBUG;
		XII_COMPARISON compType = COMP_DEFAULT;
		RTARGET_TYPE rtType = RT_OFF;
		int numRenderTarget;

		wchar_t* WcToString(std::string s)
		{
			wchar_t* wchar = new wchar_t[128];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, wchar, 128);
			return wchar;
		}

	public:
		GraphicsPipeline(int numRTarget = 1,XII_CULLMODE cullMode = CULL_DEFAULT, XII_MASK mask = MASK_DEBUG,XII_COMPARISON comp = COMP_DEFAULT,RTARGET_TYPE rTarget = RT_OFF);
		~GraphicsPipeline(){}

		void AddInputLayout(const INPUT_LAYOUT& layout)
		{
			INPUT_LAYOUT clayout = layout;
			numILayout.push_back(clayout);
		}

		void AddShader(SHADE_TYPE type, std::string filename,std::string funcname = "main")
		{
			shadeData[type] = { filename,funcname };
		}

		void DestroyShader(SHADE_TYPE type)
		{
			shadeData.erase(type);
		}

		void AddRange(RANGE_TYPE rangeType, UINT nDescriptor = 1, UINT bsRegister = 0)
		{
			ranges.push_back({rangeType,nDescriptor,bsRegister});
		}

		void AddRootParam(UINT sRegister, RANGE_TYPE rangeType, SRV_ST_TYPE setType = SRV_NONE)
		{
			rootParam.push_back({ sRegister,rangeType,setType });
		}

		bool CheckShaderCompileResult(HRESULT result, ID3DBlob* error);

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature() { return rootsignature; }
		Microsoft::WRL::ComPtr<ID3D12PipelineState> GetPipelineState() { return pipelinestate; }

		/// <summary>
		/// èâä˙âª
		/// </summary>
		void Init();

	};
}
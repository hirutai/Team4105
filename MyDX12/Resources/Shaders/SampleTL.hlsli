cbuffer cbuff0 : register(b0)
{
	float4 cameraPos;
	matrix mat;
	matrix viewproj;
	float4 tessRange;
};

Texture2D texHeightMap : register(t0);
Texture2D texNormalMap : register(t1);
SamplerState mapSampler : register(s0);

struct VSInput
{
	float4 pos : POSITION; // システム用頂点座標
	float2 uv  :  TEXCOORD;
};

struct HSInput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :  TEXCOORD0;
};

struct DSInput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :  TEXCOORD0;
};

struct PSInput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :  TEXCOORD0;
	float3 normal : TEXCOORD1;
};

struct HSParameters
{
	float tessFactor[4] : SV_TessFactor;
	float insideFactor[2] : SV_insideTessFactor;
};
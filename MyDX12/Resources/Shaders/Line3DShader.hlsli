cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float3 st;
	matrix viewproj;
	float3 ed;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
};
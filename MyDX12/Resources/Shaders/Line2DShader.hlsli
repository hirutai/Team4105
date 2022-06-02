cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float2 st;
	float2 ed;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
};
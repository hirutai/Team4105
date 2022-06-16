cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix viewproj;
	matrix matbillboard;
	float4 color;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD; // uv値
};
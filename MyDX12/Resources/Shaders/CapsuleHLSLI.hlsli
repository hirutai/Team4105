cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix viewproj;
	float4 color;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
};
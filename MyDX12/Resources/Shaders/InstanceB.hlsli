static const int INSTANCE_NUM = 500;

struct InstanceData
{
	matrix mat;
	float4 color;
	uint active;
};

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix matbillboard;
	InstanceData data[INSTANCE_NUM];
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  :TEXCOORD; // uv値
	float4 color : COLOR;
};
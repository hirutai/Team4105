cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float3 st;
	matrix viewproj;
	float3 ed;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
};
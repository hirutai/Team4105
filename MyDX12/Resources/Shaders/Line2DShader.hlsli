cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float2 st;
	float2 ed;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
};
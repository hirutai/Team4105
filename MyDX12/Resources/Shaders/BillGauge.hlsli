cbuffer cbuff0 : register(b0)
{
	matrix mat;
	matrix viewproj;
	matrix matbillboard;
	float4 color;
};

struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv  :TEXCOORD; // uv�l
};
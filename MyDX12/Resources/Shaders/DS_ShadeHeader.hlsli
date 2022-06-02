cbuffer cbuff0 : register(b0)
{
	matrix mat;
};

struct VSInput
{
	float4 pos : POSITION; // �V�X�e���p���_���W
	float2 uv : TEXCOORD;
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv : TEXCOORD;
};
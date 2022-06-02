cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float _t;
	float _d;
	int _types;
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv  : TEXCOORD; // uv�l
};

struct BlurOutput {
	float4 highLum:SV_TARGET0;//���P�x(High Luminance)
	float4 col:SV_TARGET1;//�ʏ�̃����_�����O����
};
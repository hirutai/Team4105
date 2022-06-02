cbuffer cbuff0 : register(b0)
{
	matrix mat; // �r���[�v���W�F�N�V�����s��
	matrix matBillboard; // �r���{�[�h�s��
};

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 pos : POSITION; // ���_���W
	float scale : SCALE; // �X�P�[��
	float4 color : COLOR;
};

struct GSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float2 uv  :TEXCOORD; // uv�l
};
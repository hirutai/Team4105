#include "DS_ShadeHeader.hlsli"
Texture2D<float4> tex0 : register(t0);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> depthTex : register(t1);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float dep = pow(depthTex.Sample(smp,input.uv),20);
	return float4(dep, dep, dep, 1.0f);
}
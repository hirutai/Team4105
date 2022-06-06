#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(normal, 0)));
	float4 wpos = mul(world, pos);

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	//output.svpos = mul(mul(viewproj, world), pos);
	output.svpos = mul(viewproj,wpos);

	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = uv;
	output.light = normalize(lightpos);
	output.lighttexCoord = mul(pos, wlpt);
	output.lightViewpos = mul(pos, wlp);

	return output;
}
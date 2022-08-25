#include "InstanceB.hlsli"
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD, uint InstanceID : SV_InstanceID)
{
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	uint index = InstanceID;
	matrix world = data[index].mat;
	output.svpos = mul(matbillboard, pos);
	output.svpos = mul(mul(viewproj, world), output.svpos);
	output.uv = uv;
	output.color = data[index].color;
	return output;
}
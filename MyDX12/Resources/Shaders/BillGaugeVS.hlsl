#include "BillGauge.hlsli"
VSOutput main( float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(matbillboard, pos);
	output.svpos = mul(mul(viewproj, mat), output.svpos);
	output.uv = uv;
	return output;
}
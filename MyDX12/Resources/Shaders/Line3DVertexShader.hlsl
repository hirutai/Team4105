#include "Line3DShader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mul(viewproj, mat), pos);
	return output;
}
#include "DS_ShadeHeader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.svpos = mul(input.pos,mat);
	output.uv = input.uv;
	return output;
}
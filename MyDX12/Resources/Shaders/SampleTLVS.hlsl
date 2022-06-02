#include "SampleTL.hlsli"

HSInput main(VSInput input)
{
	HSInput result = (HSInput)0;
	matrix mtxWVP = mul(mat, viewproj);
	result.svpos = input.pos;
	result.uv = input.uv;
	return result;
}
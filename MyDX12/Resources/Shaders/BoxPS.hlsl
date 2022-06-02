#include "BoxPrimitive.hlsli"
float4 main(VSOutput input) : SV_TARGET
{
	float4 col = input.color;
	return col;
}
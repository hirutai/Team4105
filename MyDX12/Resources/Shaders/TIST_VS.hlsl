#include "TIST.hlsli"

struct VSInput
{
	float4 pos : POSITION;
	float2 uv  : TEXCOORD;
	uint InstanceID : SV_InstanceID;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	uint index = input.InstanceID;
	matrix world = data[index].mat;
	output.svpos = mul(world, input.pos);
	output.uv = input.uv;
	output.color = data[index].color;

	return output;
}
#include "PointShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION,uint InstanceID : SV_InstanceID)
{
	VSOutput output;
	uint index = InstanceID;
	matrix world = data[index].mat;
	output.svpos = mul(world, pos);
	output.color = data[index].color;

	return output;
}
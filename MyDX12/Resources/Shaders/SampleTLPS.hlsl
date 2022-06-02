#include "SampleTL.hlsli"

float4 main(PSInput In) : SV_TARGET
{
	float3 norm = normalize((texNormalMap.SampleLevel(mapSampler, In.uv, 0).xyz - 0.5));

	float3 lightDir = normalize(float3(1, 1, 0));
	float l = saturate(dot(norm, lightDir));

	float3 diffuse = float3(0.1, 0.9, 0.1) * l;
	return float4(diffuse, 1);
}
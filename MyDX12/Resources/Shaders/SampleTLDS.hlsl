#include "SampleTL.hlsli"

[domain("quad")]
PSInput main(HSParameters In,float2 loc : SV_DomainLocation,const OutputPatch<DSInput,4> patch)
{
	PSInput result;

	matrix mtxWVP = mul(mat, viewproj);

	float3 p0 = lerp(patch[0].svpos, patch[1].svpos, loc.x).xyz;
	float3 p1 = lerp(patch[2].svpos, patch[3].svpos, loc.x).xyz;
	float3 pos = lerp(p0, p1, loc.y);

	float2 c0 = lerp(patch[0].uv, patch[1].uv, loc.x);
	float2 c1 = lerp(patch[2].uv, patch[3].uv, loc.x);
	float2 uv = lerp(c0, c1, loc.y);

	float height = texHeightMap.SampleLevel(mapSampler, uv, 0).x;
	pos.y = height * 20;

	float3 n = normalize((texNormalMap.SampleLevel(mapSampler, uv, 0).xyz - 0.5));
	result.normal = n;
	result.uv = uv;
	result.svpos = mul(float4(pos.xyz, 1), mtxWVP);

	return result;
}

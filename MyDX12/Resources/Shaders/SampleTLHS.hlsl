#include "SampleTL.hlsli"

float CalcTessFactor(float4 v)
{
	float dist = length(mul(v, mat).xyz - cameraPos.xyz);
	float tessNear = tessRange.x;
	float tessFar = tessRange.y;

	const float MaxTessFactor = 32.0;
	float val = MaxTessFactor - (MaxTessFactor - 1) * (dist - tessNear) / (tessFar - tessNear);
	val = max(1, min(MaxTessFactor, val));
	return val;
}

float CalcNormalBias(float3 p, float3 n)
{
	const float normalThreshold = 0.85; // –ñ60“x
	float3 camPos = cameraPos.xyz;
	float3 fromCamera = normalize(p - camPos);
	float cos2 = dot(n, fromCamera);
	cos2 *= cos2;
	float normalFactor = 1.0 - cos2;
	float bias = max(normalFactor - normalThreshold, 0) / (1.0 - normalThreshold);
	return bias * clamp(tessRange.z, 0, 64);
}

HSParameters ComputePatchConstants
(InputPatch<HSInput, 4>patch)
{
	HSParameters outParams;

	{
		float4 v[4];
		float3 n[4];
		int indices[][2] =
		{
			{2,0},{0,1},{1,3},{2,3}
		};

		for (int i = 0; i < 4; ++i)
		{
			int idx0 = indices[i][0];
			int idx1 = indices[i][1];
			v[i] = 0.5 * (patch[idx0].svpos + patch[idx1].svpos);
			float2 uv = 0.5 * (patch[idx0].uv + patch[idx1].uv);
			n[i] = texNormalMap.SampleLevel(mapSampler, uv, 0).xyz;
			n[i] = normalize(n[i] - 0.5);
		}

		outParams.tessFactor[0] = CalcTessFactor(v[0]);
		outParams.tessFactor[2] = CalcTessFactor(v[2]);
		outParams.tessFactor[0] += CalcNormalBias(v[0].xyz, n[0].xyz);
		outParams.tessFactor[2] += CalcNormalBias(v[2].xyz, n[2].xyz);
		outParams.insideFactor[0] = 0.5 * (outParams.tessFactor[0] + outParams.tessFactor[2]);

		outParams.tessFactor[1] = CalcTessFactor(v[1]);
		outParams.tessFactor[3] = CalcTessFactor(v[3]);
		outParams.tessFactor[1] += CalcNormalBias(v[1].xyz, n[1].xyz);
		outParams.tessFactor[3] += CalcNormalBias(v[3].xyz, n[3].xyz);
		outParams.insideFactor[1] = 0.5 * (outParams.tessFactor[1] + outParams.tessFactor[3]);
	}

	return outParams;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ComputePatchConstants")]
DSInput main(InputPatch<HSInput, 4>inPatch,uint pointID : SV_OutputControlPointID)
{
	DSInput result;
	result.svpos = inPatch[pointID].svpos;
	result.uv = inPatch[pointID].uv;

	return result;
}

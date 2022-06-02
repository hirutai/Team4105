#include "PostEffectTest.hlsli"

Texture2D<float4> tex0 : register(t0);	// 0番スロットに設定されたテクスチャ
Texture2D<float> tex1 : register(t1);	// 0番スロットに設定されたテクスチャ
//Texture2D<float> shadowMap : register(t2);	// 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		// 0番スロットに設定されたサンプラー

float rand(float co)
{
	return frac(sin(co * 12.9898) * 43758.5453);
}

float rand(float2 co)
{
	return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

float rand(float3 co)
{
	return frac(sin(dot(co.xyz, float3(12.9898, 78.233, 56.787))) * 43758.5453);
}

float2 mod(float2 a, float2 b)
{
	return a - floor(a / b) * b;
}

float4 HighLumi(float2 uv)
{
	float4 col = tex0.Sample(smp, uv);
	float grayScale = col.r * 0.299 + col.g * 0.587 + col.b * 0.114;
	float extract = smoothstep(0.6, 0.9, grayScale);
	return col * extract;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sG, float sW)
{
	float totalWeight = 0, sigma = sG, stepWidth = sW;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			col += tex0.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	col.rgb = col.rgb / totalWeight;
	return col;
}

float4 GaussianGX(Texture2D<float4> tex, SamplerState smp, float2 uv, float d)
{
	float pixel_u;
	float pixel_v;

	if (d == 0.0f)
	{
		pixel_u = 0.0f;
		pixel_v = 0.0f;
	}
	else
	{
		pixel_u = 1.0f / d;
		pixel_v = 1.0f / d;
	}

	float4 dcolor = { 0,0,0,0 };

	for (int i = -1; i < 2; i++)
	{
		// 左上ピクセルの色をサンプリングし、合計に足す(U: -1pixel V:-1pixel)
		dcolor += tex.Sample(smp, uv + float2(-pixel_u, pixel_v * i));

		// 中上ピクセルの色をサンプリングし、合計に足す(U: +-0 V:-1pixel)
		dcolor += tex.Sample(smp, uv + float2(0.0f, pixel_v * i));

		// 右上ピクセルの色をサンプリングし、合計に足す(U: +1pixel V:-1pixel)
		dcolor += tex.Sample(smp, uv + float2(pixel_u, pixel_v * i));
	}

	return float4(dcolor.rgb / 9, 1);
}

float4 InverseColor(Texture2D<float4> tex, SamplerState smp, float2 uv)
{
	float4 texcolor = tex.Sample(smp,uv);
	return float4(1 - texcolor.rgb, 1);
}

float2 randomVec(float2 fact)
{
	float2 angle = float2(
		dot(fact, float2(127.1, 311.7)),
		dot(fact, float2(269.5, 183.3))
		);
	return frac(sin(angle) * 43758.5453123) * 2 - 1;
}

// ノイズの密度をdensityデ設定、uvにi.uvを代入
float PerlinNoise(float density, float2 uv)
{
	float2 uvFloor = floor(uv * density* float2(1280.0f/2,960.0f/2));
	float2 uvFrac = frac(uv * density * float2(1280.0f / 2, 960.0f / 2));

	float2 v00 = randomVec(uvFloor + float2(0, 0)); // ②各頂点のランダムなベクトルを取得
	float2 v01 = randomVec(uvFloor + float2(0, 1));
	float2 v10 = randomVec(uvFloor + float2(1, 0));
	float2 v11 = randomVec(uvFloor + float2(1, 1));

	float c00 = dot(v00, uvFrac - float2(0, 0)); // ②と③の内積を取って、④を作成
	float c01 = dot(v01, uvFrac - float2(0, 1));
	float c10 = dot(v10, uvFrac - float2(1, 0));
	float c11 = dot(v11, uvFrac - float2(1, 1));

	float2 u = uvFrac * uvFrac * (3 - 2 * uvFrac);

	float v0010 = lerp(c00, c10, u.x); // 描画するピクセルから⑤を求める
	float v0111 = lerp(c01, c11, u.x);

	return lerp(v0010, v0111, u.y) / 2 + 0.5;
}

float FractalSumNoise(float density, float2 uv)
{
	float fn;
	fn = PerlinNoise(density * 1, uv) * 1.0 / 2;
	fn += PerlinNoise(density * 2, uv) * 1.0 / 4;
	fn += PerlinNoise(density * 4, uv) * 1.0 / 8;
	fn += PerlinNoise(density * 8, uv) * 1.0 / 16;
	return fn;
}

float4 main(VSOutput input) : SV_TARGET
{
	float _density = _d;
	float pn = FractalSumNoise(_density, input.uv);
	float t = _t;
	float pn1 = FractalSumNoise(_density, input.uv + float2(t, t));
	float pn2 = FractalSumNoise(_density, input.uv + float2(-t, -t));

	float2 uv = input.uv + float2(pn1, pn2);
	float elec = step(0.985, 1 - abs(pn1 - pn2));
	
	float4 result = float4(1, 1, 1, 1);
	//float4 tpos = tex2.Sample(smp,input.uv);
	//float3 posFromLightVP = tpos.xyz / tpos.w;
	//float2 shadowUV = (posFromLightVP + float2(1, -1)) * float2(0.5f, -0.5f);

	//float depthFromLight = 
	//float shadowWeight = 1.0f;
	//if (depthFromLight < posFromLightVP.z)
	//{
	//	shadowWeight = 0.5f;
	//}

	//float4 texC = tex0.Sample(smp, input.uv);
	float depthT = tex1.Sample(smp, input.uv);
	float4 col = tex0.Sample(smp, input.uv);
	//float zV = shadowMap.Sample(smp, input.uv);
	//if (zV > depthT + 0.005f)
	//{
	//	col.rgb = col.rgb * 0.5f;
	//}
	//result = float4(depthT,0,0, depthT);
	result = col;
	//if (_types == 0)
	//{
	//	result = tex0.Sample(smp, input.uv);
	//}
	//else if (_types == 1)
	//{
	//	float4 texcolor = tex0.Sample(smp, input.uv);
	//	float4 highLumiTex = HighLumi(input.uv);
	//	float4 blurTex = GaussianBlur(input.uv, 0.005, 0.001) * highLumiTex;
	//	float4 compoTex = texcolor + blurTex;
	//	result = texcolor * compoTex;
	//}
	//else if (_types == 2)
	//{
	//	result = tex0.Sample(smp, uv);
	//}
	//else if (_types == 3)
	//{
	//	float4 texcolor = tex0.Sample(smp, input.uv);
	//	float3 rgb = { 1,1,1 };
	//	float4 col = float4(rgb * elec, 1);
	//	result = texcolor + col;
	//}
	//else if (_types == 4)
	//{
	//	float4 texcolor = tex0.Sample(smp,uv);
	//	float3 rgb = { 1,1,1 };
	//	float4 col = float4(rgb * elec, 1);
	//	result = texcolor + col;
	//}
	//else if (_types == 5)
	//{
	//	float depth = tex1.Sample(smp, input.uv);
	//	//float _Depth = 0.2;
	//	//float _NWidth = 0.1;
	//	//float _FWidth = 0.1;
	//	//float near = smoothstep(0, _NWidth / 2, abs(depth.r - _Depth));
	//	//float far = smoothstep(_NWidth / 2, _NWidth / 2 + _FWidth / 2, abs(depth.r - _Depth));
	//	////float4 pintColor = tex0.Sample(smp, input.uv);
	//	//float4 pintColor = { 1,0,0,1 };
	//	////float4 nearColor = GaussianBlur(input.uv, 0.01, 0.001);
	//	//float4 nearColor = { 0,1,0,1 };
	//	////float4 farColor = GaussianBlur(input.uv, 0.02, 0.001);
	//	//float4 farColor = { 0,0,1,1 };

	//	//result = (1 - near) * pintColor + near * ((1 - far) * nearColor + far * farColor);
	//	// 2000 : 1 = 800 : n  n = 8 / 20
	//	clip(depth - 0.1f);
	//	float4 boke = GaussianBlur(input.uv, 0.01, 0.001);

	//	boke.a = min(1.0f, (depth - 0.4f) / 0.25f);

	//	result = boke;
	//}
	//else if (_types == 6)
	//{
	//	//float4 col = tex2.Sample(smp, input.uv);

	//	//float4 pLVP = tex2.Sample(smp, input.uv);
	//	//float2 shadowMapUV = pLVP.xy / pLVP.w;
	//	//shadowMapUV *= float2(0.5f, -0.5f);
	//	//shadowMapUV += 0.5f;

	//	//float zLVP = pLVP.z / pLVP.w;
	//	//float zInShadowMap = tex1.Sample(smp, input.uv);
	//	//if (zLVP > zInShadowMap)
	//	//{
	//	//	//遮蔽されている
	//	//	col.xyz *= 0.5f;
	//	//}
	//	////pLVP = tex3.Sample(smp, input.uv);

	//	//result = col;
	//	//result = float4(zInShadowMap, zInShadowMap, zInShadowMap , 1);
	//}

	return result;
}
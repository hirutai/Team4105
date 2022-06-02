#include "FBX.hlsli"
// 0番スロットに設定されたテクスチャ
Texture2D<float4> baseTex : register(t0);
Texture2D<float4> metalnessTex : register(t1);
Texture2D<float4> normalTex : register(t2);
Texture2D<float4> roughnessTex : register(t3);
//Texture2D<float> lightDepthTex : register(t1);
// 0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

// π
static const float PI = 3.141592654f;
// 反射点の法線ベクトル
static float3 N;

// アルベド
static float3 s_baseColor;
// 金属度
static float s_metalness;
// 粗さ
static float s_roughness;

// Schlickによる近似
// f0 と f90の値を(1 - cosine)の5乗でlerpする
// f0 : 光が垂直に入射したときの反射率
// f90 : 光が平行に入射したときの反射率
// cosine : 2ベクトルのなす角のコサイン(内積値)
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

// UE4のGGX分布
// alpha : roughnessの2乗
// NdotH : 法線とハーフベクトルの内積
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH)
{
	// F項(フレネル:Fresnel)
	//輝度
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;
	// 色合い
	float3 tintColor = s_baseColor / luminance;
	// 非金属の鏡面反射色を計算
	float3 nonMetalColor = specular * 0.08f * tintColor;
	// metalnessによる色補間 金属の場合はベースカラー
	float3 specularColor = lerp(nonMetalColor, s_baseColor, s_metalness);
	// NdotHの割合でSchlickFresnel補間
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

float GeometricSmith(float cosine)
{
	float k = (s_roughness + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

// 鏡面反射の計算
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	// D項 (分布 : Distribution)
	float Ds = DistributionGGX(s_roughness * s_roughness,NdotH); // 仮(後で置き換える)

	// F項(フレネル : Fresnel)
	float3 Fs = DisneyFresnel(LdotH);

	// G項(幾何減衰 : Geometry attenuation)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	// m項(分母)
	float m = 4.0f * NdotL * NdotV;

	// 合成して鏡面反射の色を得る
	return Ds * Fs * Gs / m;
}

// 双方向反射分布関数
float3 BRDF(float3 L, float3 V)
{
	// 法線とライト方向の内積
	float NdotL = dot(N, L);
	// 法線とカメラ方向の内積
	float NdotV = dot(N, V);
	// どちらかが90°以上であれば真っ黒を返す
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	// ライト方向とカメラ方向の中間(ハーフベクトル)
	float3 H = normalize(L + V);
	// 法線とハーフベクトルの内積
	float NdotH = dot(N, H);
	// ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	// 拡散反射率
	float diffuseReflectance = 1.0f / PI;

	// 入射角が90度の場合の拡散反射率
	float energyBias = 0.5f * s_roughness;
	float Fd90 = energyBias + 2 * LdotH * LdotH * s_roughness;
	// 入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	// 出ていく時の拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	// 入って出ていくまでの拡散反射率
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, s_roughness);
	float Fd = FL * FV * energyFactor;

	// 拡散反射光
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness);

	// ここに鏡面反射の式を挿入する
	// 鏡面反射光
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor;
}

// エントリーポイント
float4 main(VSOutput input) : SV_TARGET
{
	// Lambert反射
	float3 light = normalize(float3(1, -1, 1)); // 右下奥 向きのライト
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	float node = 0.5f * dot(normalize(input.normal), light) + 0.5f;
	float s = step(0.35f, saturate(node + 0.0f)) * 0.5f;
	float f = 1 - step(0.35f, saturate(node + 0.0f)) * -0.5f;
	float toon = s + f;

	float4 sColor = float4(toon, toon, toon, 1);

	// 面の情報をstatic変数に代入し、関数から三章できるようにする
	N = input.normal;

	// パラメータをテクスチャのRGBから取得
	s_baseColor = baseColor + baseTex.Sample(smp, input.uv).rgb;
	s_metalness = metalness + metalnessTex.Sample(smp, input.uv).r;
	s_roughness = roughness + roughnessTex.Sample(smp, input.uv).r;

	// 最終的なRGB
	float3 finalRGB = float3(0, 0, 0);
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float4 lightVect = float4(1, 1, 1, 1);
	// 最終的な色(Not BRDF)
	float3 finiColor = baseTex.Sample(smp, input.uv).rgb;
	// 平行光源
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active) {
			continue;
		}
		//// ライトに向かうベクトルと法線の内積
		//float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		//// 反射光ベクトル
		//float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		//// 拡散反射光
		//float3 diffuse = dotlightnormal * baseColor;
		//// 光沢度(仮)
		//const float shininess = 1.0f;
		//// 鏡面反射光
		//float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		//// 全て加算する
		//finiColor += (diffuse + s_specular) * dirLights[i].lightcolor;

		lightVect.rgb = dirLights[i].lightv;
		// BRDFの結果とライト色を合成
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}
	// 点光源
	for (int i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (!pointLights[i].active){
			continue;
		}
		// ライトのベクトル
		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
		// ベクトルの長さ
		float d = length(lightv);
		// 正規化し、単位ベクトルにする
		lightv = normalize(lightv);
		// 距離減衰係数
		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
		// ライトに向かうベクトルと法線の内積
		float3 dotlightnormal = dot(lightv, input.normal);
		// 反射光ベクトル
		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		// 拡散反射光
		float3 diffuse = dotlightnormal * baseColor;
		// 光沢度(仮)
		const float shininess = 1.0f;
		// 鏡面反射光
		float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		// 全て加算する
		finiColor += atten * (diffuse + s_specular) * pointLights[i].lightcolor;
	}
	// スポットライト
	for (int i = 0; i < SPOTLIGHT_NUM; i++) {
		// ライトが有効でなければ飛ばす
		if (!spotLights[i].active) {
			continue;
		}

		// ライトへの方向ベクトル
		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
		float d = length(lightv);
		lightv = normalize(lightv);
		// 距離減衰係数
		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
		// 角度減衰
		float cos = dot(lightv, spotLights[i].lightv);
		// 減衰開始角度から、減衰終了角度にかけて減衰
		//減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
		// 角度減衰を乗算
		atten *= angleatten;
		// ライトに向かうベクトルと法線の内積
		float3 dotlightnormal = dot(lightv, input.normal);
		// 反射光ベクトル
		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		// 拡散反射光
		float3 diffuse = dotlightnormal * baseColor;
		// 光沢度(仮)
		const float shininess = 0.5f;
		// 鏡面反射光
		float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		// 全て加算する
		finiColor += atten * (diffuse + s_specular) * spotLights[i].lightcolor;
	}


	float zvalue = input.tpos.z / input.tpos.w;// Z値
	//float sm = shadowMap.Sample(smp, input.uv);
	//float sma = (zvalue - 0.005f < sm ) ? 1.0f : 0.5f;

	float4 result = float4(finalRGB, 1);
	return result;
}
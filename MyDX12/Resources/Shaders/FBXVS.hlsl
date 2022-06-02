#include "FBX.hlsli"

// スキニング後の頂点・法線が入る
struct SkinOutput
{
	float4 pos;
	float3 normal;
};

struct ShadowSkinOutput
{
	matrix conBone;
};

SkinOutput ComputeSkin(VSInput input)
{
	// ゼロクリア
	SkinOutput output = (SkinOutput)0;

	uint iBone; // 計算するボーン番号
	float weight; // ボーンウェイト(重み)
	matrix m; // スキニング行列

	// ボーン0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	// ボーン1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	// ボーン2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	
	output.normal += weight * mul((float3x3)m, input.normal);

	// ボーン3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	return output;
}

VSOutput main(VSInput input, uint instNo : SV_InstanceID)
{
	// スキニング計算
	SkinOutput skinned = ComputeSkin(input);
	// 法線にワールド行列によるスケーリング・回転を適用
	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	// ピクセルシェーダーに渡す値
	VSOutput output;
	// 行列による座標変換
	output.svpos = mul(world, skinned.pos);
	//if (instNo == 1)
	//{
	//	output.svpos = mul(shadow, output.svpos);
	//}
	output.svpos = mul(viewproj, output.svpos);
	//output.svpos = mul(shadow, output.svpos);
	// ワールド座標を次のステージに渡す
	output.worldpos = mul(world, input.pos).xyz;
	// ワールド法線を次のステージに渡す
	output.normal = wnormal.xyz;
	// 入力値をそのまま次のステージに渡す
	output.uv = input.uv;

	output.tpos = mul(lightCamera, output.svpos);

	output.instNo = instNo;
	//float4 sPos = mul(input.pos, world);
	//sPos = mul(sPos, lightVP);
	//sPos.xyz = sPos.xyz / sPos.w;
	//output.pos_sm.x = (1.0f + sPos.x) / 2.0f;
	//output.pos_sm.y = (1.0f - sPos.y) / 2.0f;
	//output.pos_sm.z = sPos.z;

	return output;
}

float4 ShadowVS(VSInput input, uint instNo : SV_InstanceID) : SV_POSITION
{
	SkinOutput skinned = ComputeSkin(input);
	float4 result;
	result = mul(world, skinned.pos);
	result = mul(lightCamera, result);
	float zValue = result.z / result.w;

	return float4(zValue, zValue, zValue, zValue);
}
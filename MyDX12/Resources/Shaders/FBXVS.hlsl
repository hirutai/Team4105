#include "FBX.hlsli"

// �X�L�j���O��̒��_�E�@��������
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
	// �[���N���A
	SkinOutput output = (SkinOutput)0;

	uint iBone; // �v�Z����{�[���ԍ�
	float weight; // �{�[���E�F�C�g(�d��)
	matrix m; // �X�L�j���O�s��

	// �{�[��0
	iBone = input.boneIndices.x;
	weight = input.boneWeights.x;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	// �{�[��1
	iBone = input.boneIndices.y;
	weight = input.boneWeights.y;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);

	// �{�[��2
	iBone = input.boneIndices.z;
	weight = input.boneWeights.z;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	
	output.normal += weight * mul((float3x3)m, input.normal);

	// �{�[��3
	iBone = input.boneIndices.w;
	weight = input.boneWeights.w;
	m = matSkinning[iBone];
	output.pos += weight * mul(m, input.pos);
	output.normal += weight * mul((float3x3)m, input.normal);
	return output;
}

VSOutput main(VSInput input, uint instNo : SV_InstanceID)
{
	// �X�L�j���O�v�Z
	SkinOutput skinned = ComputeSkin(input);
	// �@���Ƀ��[���h�s��ɂ��X�P�[�����O�E��]��K�p
	float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
	// �s�N�Z���V�F�[�_�[�ɓn���l
	VSOutput output;
	// �s��ɂ����W�ϊ�
	output.svpos = mul(world, skinned.pos);
	//if (instNo == 1)
	//{
	//	output.svpos = mul(shadow, output.svpos);
	//}
	output.svpos = mul(viewproj, output.svpos);
	//output.svpos = mul(shadow, output.svpos);
	// ���[���h���W�����̃X�e�[�W�ɓn��
	output.worldpos = mul(world, input.pos).xyz;
	// ���[���h�@�������̃X�e�[�W�ɓn��
	output.normal = wnormal.xyz;
	// ���͒l�����̂܂܎��̃X�e�[�W�ɓn��
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
#include "FBX.hlsli"
// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> baseTex : register(t0);
Texture2D<float4> metalnessTex : register(t1);
Texture2D<float4> normalTex : register(t2);
Texture2D<float4> roughnessTex : register(t3);
//Texture2D<float> lightDepthTex : register(t1);
// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

// ��
static const float PI = 3.141592654f;
// ���˓_�̖@���x�N�g��
static float3 N;

// �A���x�h
static float3 s_baseColor;
// �����x
static float s_metalness;
// �e��
static float s_roughness;

// Schlick�ɂ��ߎ�
// f0 �� f90�̒l��(1 - cosine)��5���lerp����
// f0 : ���������ɓ��˂����Ƃ��̔��˗�
// f90 : �������s�ɓ��˂����Ƃ��̔��˗�
// cosine : 2�x�N�g���̂Ȃ��p�̃R�T�C��(���ϒl)
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

// UE4��GGX���z
// alpha : roughness��2��
// NdotH : �@���ƃn�[�t�x�N�g���̓���
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// �f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH)
{
	// F��(�t���l��:Fresnel)
	//�P�x
	float luminance = 0.3f * s_baseColor.r + 0.6f * s_baseColor.g + 0.1f * s_baseColor.b;
	// �F����
	float3 tintColor = s_baseColor / luminance;
	// ������̋��ʔ��ːF���v�Z
	float3 nonMetalColor = specular * 0.08f * tintColor;
	// metalness�ɂ��F��� �����̏ꍇ�̓x�[�X�J���[
	float3 specularColor = lerp(nonMetalColor, s_baseColor, s_metalness);
	// NdotH�̊�����SchlickFresnel���
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

float GeometricSmith(float cosine)
{
	float k = (s_roughness + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

// ���ʔ��˂̌v�Z
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	// D�� (���z : Distribution)
	float Ds = DistributionGGX(s_roughness * s_roughness,NdotH); // ��(��Œu��������)

	// F��(�t���l�� : Fresnel)
	float3 Fs = DisneyFresnel(LdotH);

	// G��(�􉽌��� : Geometry attenuation)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	// m��(����)
	float m = 4.0f * NdotL * NdotV;

	// �������ċ��ʔ��˂̐F�𓾂�
	return Ds * Fs * Gs / m;
}

// �o�������˕��z�֐�
float3 BRDF(float3 L, float3 V)
{
	// �@���ƃ��C�g�����̓���
	float NdotL = dot(N, L);
	// �@���ƃJ���������̓���
	float NdotV = dot(N, V);
	// �ǂ��炩��90���ȏ�ł���ΐ^������Ԃ�
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	// ���C�g�����ƃJ���������̒���(�n�[�t�x�N�g��)
	float3 H = normalize(L + V);
	// �@���ƃn�[�t�x�N�g���̓���
	float NdotH = dot(N, H);
	// ���C�g�ƃn�[�t�x�N�g���̓���
	float LdotH = dot(L, H);

	// �g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	// ���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float energyBias = 0.5f * s_roughness;
	float Fd90 = energyBias + 2 * LdotH * LdotH * s_roughness;
	// �����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	// �o�Ă������̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	// �����ďo�Ă����܂ł̊g�U���˗�
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, s_roughness);
	float Fd = FL * FV * energyFactor;

	// �g�U���ˌ�
	float3 diffuseColor = diffuseReflectance * Fd * s_baseColor * (1 - s_metalness);

	// �����ɋ��ʔ��˂̎���}������
	// ���ʔ��ˌ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor;
}

// �G���g���[�|�C���g
float4 main(VSOutput input) : SV_TARGET
{
	// Lambert����
	float3 light = normalize(float3(1, -1, 1)); // �E���� �����̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);

	float node = 0.5f * dot(normalize(input.normal), light) + 0.5f;
	float s = step(0.35f, saturate(node + 0.0f)) * 0.5f;
	float f = 1 - step(0.35f, saturate(node + 0.0f)) * -0.5f;
	float toon = s + f;

	float4 sColor = float4(toon, toon, toon, 1);

	// �ʂ̏���static�ϐ��ɑ�����A�֐�����O�͂ł���悤�ɂ���
	N = input.normal;

	// �p�����[�^���e�N�X�`����RGB����擾
	s_baseColor = baseColor + baseTex.Sample(smp, input.uv).rgb;
	s_metalness = metalness + metalnessTex.Sample(smp, input.uv).r;
	s_roughness = roughness + roughnessTex.Sample(smp, input.uv).r;

	// �ŏI�I��RGB
	float3 finalRGB = float3(0, 0, 0);
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float4 lightVect = float4(1, 1, 1, 1);
	// �ŏI�I�ȐF(Not BRDF)
	float3 finiColor = baseTex.Sample(smp, input.uv).rgb;
	// ���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (!dirLights[i].active) {
			continue;
		}
		//// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
		//float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
		//// ���ˌ��x�N�g��
		//float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
		//// �g�U���ˌ�
		//float3 diffuse = dotlightnormal * baseColor;
		//// ����x(��)
		//const float shininess = 1.0f;
		//// ���ʔ��ˌ�
		//float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		//// �S�ĉ��Z����
		//finiColor += (diffuse + s_specular) * dirLights[i].lightcolor;

		lightVect.rgb = dirLights[i].lightv;
		// BRDF�̌��ʂƃ��C�g�F������
		finalRGB += BRDF(dirLights[i].lightv, eyedir) * dirLights[i].lightcolor;
	}
	// �_����
	for (int i = 0; i < POINTLIGHT_NUM; i++)
	{
		if (!pointLights[i].active){
			continue;
		}
		// ���C�g�̃x�N�g��
		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
		// �x�N�g���̒���
		float d = length(lightv);
		// ���K�����A�P�ʃx�N�g���ɂ���
		lightv = normalize(lightv);
		// ���������W��
		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
		float3 dotlightnormal = dot(lightv, input.normal);
		// ���ˌ��x�N�g��
		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		// �g�U���ˌ�
		float3 diffuse = dotlightnormal * baseColor;
		// ����x(��)
		const float shininess = 1.0f;
		// ���ʔ��ˌ�
		float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		// �S�ĉ��Z����
		finiColor += atten * (diffuse + s_specular) * pointLights[i].lightcolor;
	}
	// �X�|�b�g���C�g
	for (int i = 0; i < SPOTLIGHT_NUM; i++) {
		// ���C�g���L���łȂ���Δ�΂�
		if (!spotLights[i].active) {
			continue;
		}

		// ���C�g�ւ̕����x�N�g��
		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
		float d = length(lightv);
		lightv = normalize(lightv);
		// ���������W��
		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
		// �p�x����
		float cos = dot(lightv, spotLights[i].lightv);
		// �����J�n�p�x����A�����I���p�x�ɂ����Č���
		//�����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
		// �p�x��������Z
		atten *= angleatten;
		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
		float3 dotlightnormal = dot(lightv, input.normal);
		// ���ˌ��x�N�g��
		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
		// �g�U���ˌ�
		float3 diffuse = dotlightnormal * baseColor;
		// ����x(��)
		const float shininess = 0.5f;
		// ���ʔ��ˌ�
		float3 s_specular = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

		// �S�ĉ��Z����
		finiColor += atten * (diffuse + s_specular) * spotLights[i].lightcolor;
	}


	float zvalue = input.tpos.z / input.tpos.w;// Z�l
	//float sm = shadowMap.Sample(smp, input.uv);
	//float sma = (zvalue - 0.005f < sm ) ? 1.0f : 0.5f;

	float4 result = float4(finalRGB, 1);
	return result;
}
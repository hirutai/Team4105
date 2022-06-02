#include "Line3DShader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mul(viewproj, mat), pos);
	return output;
}
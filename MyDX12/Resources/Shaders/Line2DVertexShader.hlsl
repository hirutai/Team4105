#include "Line2DShader.hlsli"

VSOutput main(float4 pos : POSITION)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(mat, pos);

	return output;
}
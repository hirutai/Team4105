#include "BillGauge.hlsli"
VSOutput main( float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	output.svpos = mul(matbillboard, pos);
	output.svpos = mul(mul(viewproj, mat), output.svpos);
	output.uv = uv;
	return output;
}
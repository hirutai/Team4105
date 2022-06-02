#include "Line2DShader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	bool xx0 = st.x == ed.x;
	bool yy0 = st.y == ed.y;
	float3 col = xx0 ? float3(0, 1, 0) :
		yy0 ? float3(1, 0, 0) : float3(1, 1, 1);
	return float4(col, 1);
}
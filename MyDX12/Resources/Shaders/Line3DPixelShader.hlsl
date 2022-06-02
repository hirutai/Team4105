#include "Line3DShader.hlsli"

float4 main(VSOutput input) : SV_TARGET
{
	float4 col = float4(1,1,1,1);
	bool yz_se0 = st.y == 0.0f && ed.y == 0.0f && st.z == 0.0f && ed.z == 0.0f;// XŽ²
	bool xz_se0 = st.x == 0.0f && ed.x == 0.0f && st.z == 0.0f && ed.z == 0.0f;// YŽ²
	bool xy_se0 = st.x == 0.0f && ed.x == 0.0f && st.y == 0.0f && ed.y == 0.0f;// ZŽ²
	if (yz_se0)
	{
		col = float4(1, 0, 0, 1);
	}
	else if (xz_se0)
	{
		col = float4(0, 1, 0, 1);
	}
	else if (xy_se0)
	{
		col = float4(0, 0, 1, 1);
	}

	return col;
}
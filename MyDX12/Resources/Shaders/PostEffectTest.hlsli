cbuffer cbuff0 : register(b0)
{
	matrix mat;
	float _t;
	float _d;
	int _types;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float2 uv  : TEXCOORD; // uv値
};

struct BlurOutput {
	float4 highLum:SV_TARGET0;//高輝度(High Luminance)
	float4 col:SV_TARGET1;//通常のレンダリング結果
};
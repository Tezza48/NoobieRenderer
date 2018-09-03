cbuffer perObject
{
	float4x4 worldViewProj;
};

struct PixelIn
{
	float4 posH : SV_POSITION;
};

float4 main(PixelIn input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
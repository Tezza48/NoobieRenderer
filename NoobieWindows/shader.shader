cbuffer cbPerObject
{
	float4x4 world;
	float4x4 worldViewProj;
};

struct vertIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct fragIn
{
	float3 posH : SV_POSITION;
	float4 color : COLOR;
};

void vert(vertIn i, out fragIn o)
{
	o.posH = mul(float4(i.pos, 1.0f), worldViewProj);
	i.color = i.color;
}

float4 pixel(fragIn i) : SV_TARGET
{
	return i.color;
}
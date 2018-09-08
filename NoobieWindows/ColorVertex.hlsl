cbuffer perObject
{
	float4x4 world;
	float4x4 worldInverseTranspose;
	float4x4 worldViewProj;
};

struct VertIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct VertOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float3 normalW : NORMAL;
};

VertOut main(VertIn input)
{
	VertOut output;

	output.posH = mul(float4(input.pos, 1.0f), worldViewProj);
	output.color = input.color;
	output.normalW = mul(float4(input.normal, 1.0f), worldInverseTranspose);
	return output;
}
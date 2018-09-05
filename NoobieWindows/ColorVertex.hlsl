cbuffer perObject
{
	float4x4 worldViewProj;
};

struct VertIn
{
	float3 pos : POSITION;
};

struct VertOut
{
	float4 posH : SV_POSITION;
};

VertOut main(VertIn input)
{
	VertOut output;

	output.posH = mul(float4(input.pos, 1.0f), worldViewProj);
	//output.posH = float4(input.pos, 1.0f);
	return output;
}
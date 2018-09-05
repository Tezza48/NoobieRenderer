//cbuffer perObject
//{
//	float4x4 worldViewProj;
//};

struct VertIn
{
	float3 pos : POSITION;
};

struct VertOut
{
	float4 pos : SV_POSITION;
};

VertOut main(VertIn input)
{
	VertOut output;

	//output.posH = mul(float4(input.pos, 1.0f), worldViewProj);
	output.pos = float4(input.pos, 1.0f);
	return output;
}
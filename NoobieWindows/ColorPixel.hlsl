//cbuffer perObject
//{
//	float4x4 worldViewProj;
//};

//cbuffer LightBuffer
//{
//	float3 ldirection;
//	float pad;
//	float4 ldiffuseColor;
//};

struct PixelIn
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

//float DirectionalLightAttn(float3 lightDir, float3 normal)
//{
//	normal = normalize(normal);
//	float nDotL = dot(-lightDir, input.normalW);
//	return nDotL;
//}

float4 main(PixelIn input) : SV_TARGET
{
	//return float4(input.normal, 1.0f);
	return float4(abs(input.normal), 1);
}
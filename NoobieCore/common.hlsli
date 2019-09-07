struct DirectionalLight
{
	float4 direction;
	float4 color;
};

cbuffer perObject : register(b0)
{
	float4x4 gWorldViewProj;
	float4x4 gWorld;
	float4x4 gWorldInverseTranspose;
	float3 gEyePosW;
};

cbuffer perFrame : register(b1)
{
	float gEffectAmount;
}

//cbuffer LightBuffer: register(b1)
//{
//	DirectionalLight gDirLight;
//}
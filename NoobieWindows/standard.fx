//#include "lightHelper.fx"

cbuffer perObject
{
	float4x4 worldViewProj;
};

//cbuffer perFrame
//{
//	DirectionalLight dirLight;
//};

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	//float2 tex : TEXCOORD0;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	//float2 tex : TEXCOORD0;
};

VertexOut VS(VertexIn i)
{
	VertexOut o;
	o.posH = mul(float4(i.pos, 1.0), worldViewProj);
	o.color = i.color;
	o.normal = i.normal;
	//o.tex = i.tex;

	return o;
}

float4 PS(VertexOut i) :SV_TARGET
{
	float4 col;
	//float nDotL = dot(i.normal, -dirLight.direction);
	//col = nDotL * dirLight.color;

	float ndotl = dot(i.normal, -float3(-1.0, -1.0, 1.0));
	col = ndotl * float4(0.1, 0.6, 0.9, 1.0);

	return col;
}

technique11 Default
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
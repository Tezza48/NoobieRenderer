#include "lightHelper.fx"

struct Material
{
	float4 diffuse;
	float4 specular;
	float4 reflect;
};

cbuffer perObject
{
	float4x4 worldViewProj;
	float4x4 world;
	float4x4 worldInverseTranspose;
	float3 eyePosW;
	Material mat;
};

cbuffer perFrame
{
	float4 ambientLight;
};

cbuffer perLight
{
	DirectionalLight dirLight;
};

struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
	float3 normalL : NORMAL;
	//float2 tex : TEXCOORD0;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float4 color : COLOR;
	float3 normalW : NORMAL;
	//float2 tex : TEXCOORD0;
};

VertexOut VS(VertexIn i)
{
	VertexOut o;
	o.posH = mul(float4(i.posL, 1.0), worldViewProj);
	o.posW = mul(float4(i.posL, 1.0), world);
	o.color = i.color;
	o.normalW = mul(i.normalL, (float3x3)worldInverseTranspose);

	return o;
}

float4 PS(VertexOut i) :SV_TARGET
{
	float4 col = float4(0.0f, 0.0f, 0.0f, 0.0f);

	i.normalW = normalize(i.normalW);

	float3 toEyeW = normalize(eyePosW - i.posW.xyz);

	float3 lightVec = normalize(-float3(-1.0, -1.0, 1.0));

	float ndotl = dot(i.normalW, lightVec);

	[flatten]
	if (ndotl > 0.0f)
	{
		float3 diffuse = ndotl * float3(0.7, 0.69, 0.6);

		float3 v = reflect(-lightVec, i.normalW);
		float specFactor = pow(max(dot(v, toEyeW), 0.0f), mat.specular.a);
		col.rgb += diffuse.rgb * mat.diffuse.rgb;
		col.rgb += specFactor * mat.specular.rgb;
	}
	col.rgb += ambientLight.rgb * ambientLight.a;

	return col;
}

VertexOut VSWire(VertexIn i)
{
	VertexOut o;

	o.posH = mul(float4(i.posL, 1.0f), worldViewProj);
	o.posW = float4(0.0, 0.0, 0.0, 0.0);
	o.color = float4(0.0, 0.0, 0.0, 0.0);
	o.normalW = float3(0.0, 0.0, 0.0);

	return o;
}

float4 PSWire(VertexOut i) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

RasterizerState RSDefault
{
	FillMode = Solid;
	CullMode = back;
	FrontCounterClockwise = false;
	DepthBias = 100;
};

RasterizerState RSWireframe
{
	FillMode = Wireframe;
	CullMode = back;
	FrontCounterClockwise = false;
	DepthBias = -100;
};

technique11 Default
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(RSDefault);
	}
	//pass p1
	//{
	//	SetVertexShader(CompileShader(vs_5_0, VSWire()));
	//	SetPixelShader(CompileShader(ps_5_0, PSWire()));

	//	SetRasterizerState(RSWireframe);
	//}
}
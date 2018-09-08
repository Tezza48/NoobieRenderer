cbuffer perObject
{
	float4x4 worldViewProj;
};

struct VertexIn
{
	float3 pos : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

// Color
VertexOut VSColor(VertexIn i)
{
	VertexOut o;

	o.posH = mul(float4(i.pos, 1.0f), worldViewProj);

	o.color = i.color;

	return o;
}

float4 PSColor(VertexOut i) : SV_TARGET
{
	return i.color;
}

// Normal

VertexOut VSNormal(VertexIn i)
{
	VertexOut o;

	o.posH = mul(float4(i.pos, 1.0f), worldViewProj);

	o.color = i.color;
	o.normal = i.normal;

	return o;
}

float4 PSNormal(VertexOut i) : SV_TARGET
{
	return float4(i.normal / 2.0f + 0.5f, 1.0f);
}

technique11 ColorTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VSColor()));
		SetPixelShader(CompileShader(ps_5_0, PSColor()));

	}
}

technique11 NormalTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VSNormal()));
		SetPixelShader(CompileShader(ps_5_0, PSNormal()));
	}
}
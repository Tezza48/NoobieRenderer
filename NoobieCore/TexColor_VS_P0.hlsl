#include "common.hlsli"
#include "TexColor_Common.hlsli"

VS_OUT main(VS_IN i)
{
	float3 eyePosL = mul(gEyePosW, (float3x3)gWorldInverseTranspose);
	float explodeAmount = max(0, smoothstep(0, 5.0, (length(eyePosL - i.posL) / 2) - 0.5));

	//float3 dirToEye = normalize(i.posL - eyePosL);
	//i.posL = lerp(i.posL, i.posL + normalize(i.normalL) * 5.0, explodeAmount);


	//i.posL = lerp(i.posL, i.posL + dirToEye, explodeAmount);
	//i.posL += dirToEye * 5.0;

	VS_OUT o;
	o.posH = mul(float4(i.posL, 1.0), gWorldViewProj);
	o.posW = mul(float4(i.posL, 1.0), gWorld);
	o.color = i.color;
	o.normalW = mul(i.normalL, (float3x3)gWorldInverseTranspose);
	o.tex = i.tex;

	return o;
}
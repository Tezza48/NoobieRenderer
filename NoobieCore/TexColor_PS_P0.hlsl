#include "common.hlsli"
#include "TexColor_Common.hlsli"

Texture2D gDiffuseTexture;
SamplerState gDefaultSampler;

float4 main(VS_OUT i) : SV_TARGET
{
	return gDiffuseTexture.Sample(gDefaultSampler, i.tex);
}
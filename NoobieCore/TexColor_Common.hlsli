struct VS_IN {
	float3 posL : POSITION;
	float4 color : COLOR;
	float3 normalL : NORMAL;
	float2 tex : TEXCOORD0;
};

struct VS_OUT {
	float4 posH : SV_POSITION;
	float4 posW : POSITION;
	float4 color : COLOR;
	float3 normalW : NORMAL;
	float2 tex : TEXCOORD0;
};
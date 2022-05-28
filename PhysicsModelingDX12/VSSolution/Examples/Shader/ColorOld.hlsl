cbuffer cbObjConstants : register(b0)
{
	float4x4 gWorld;
}

cbuffer cbPassConstants : register(b1)
{
	float4x4 gProj;
	float4x4 gView;
	float4x4 gViewProj;
}

struct VertexIn
{
	float3 PosL		: POSITION;
	float4 Color	: COLOR;
};

struct VertexOut
{
	float3 PosW		: POSITION;
	float4 PosH		: SV_POSITION;
	float4 Color	: COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0.0f;

	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosW = posW.xyz;
	vout.PosH = mul(posW, gViewProj);
	vout.Color = vin.Color;

	return vout;
}

float4 PS(VertexOut vout) : SV_Target
{
	return vout.Color;
}
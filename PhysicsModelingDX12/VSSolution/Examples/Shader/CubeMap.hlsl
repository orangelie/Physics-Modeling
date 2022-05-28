#include "DefaultResource.hlsl"

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0.0f;

	vout.PosL = vin.PosL;
	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;

	vout.PosW += gEyePos;

	vout.PosH = mul(float4(vout.PosW, 1.0f), gViewProj).xyww;

	return vout;
}

float4 PS(VertexOut vout) : SV_Target
{
	return gCubeMap.Sample(gSamAnisotropicWrap, vout.PosL);
}
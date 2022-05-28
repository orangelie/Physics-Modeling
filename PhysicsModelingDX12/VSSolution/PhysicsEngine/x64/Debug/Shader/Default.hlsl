#include "DefaultResource.hlsl"

VertexOut VS(VertexIn vin)
{
	VertexOut vout = (VertexOut)0.0f;

	float4 posW = mul(float4(vin.PosL, 1.0f), gWorld);
	vout.PosW = posW.xyz;
	vout.PosH = mul(posW, gViewProj);

	vout.NormalC = mul(float4(vin.NormalC, 1.0f), gWorld).xyz;

	float4 texTrans = mul(float4(vin.TexC, 0.0f, 1.0f), gTexTransform);
	vout.TexC = mul(texTrans, gMaterials[gMatIndex].MatTransform).xy;

	return vout;
}

float4 PS(VertexOut vout) : SV_Target
{
	MaterialBuffer Matr = gMaterials[gMatIndex];
	float4 DiffuseAlbedo = Matr.DiffuseAlbedo;
	float3 R0 = Matr.R0;
	float Shiness = (1.0f - Matr.Roughness);
	uint SelectedColor = Matr.SelectedColor;
	float4 Color = Matr.Color;

	if (SelectedColor == 0) {
		DiffuseAlbedo = gTextures[Matr.SrvHeapIndex].Sample(gSamAnisotropicWrap, vout.TexC) * DiffuseAlbedo;
	}
	else if (SelectedColor == 1) {
		DiffuseAlbedo = Color;
	}

	vout.NormalC = normalize(vout.NormalC);
	float4 ambient = gAmbientLight * DiffuseAlbedo;
	float3 toEyeW = normalize(gEyePos - vout.PosW);
	float3 shadowFactor = 1.0f;

	Material mat = { DiffuseAlbedo, R0, Shiness };
	float4 directLight = ComputeLighting(gLights, mat, vout.NormalC, toEyeW, vout.PosW, shadowFactor);

	float4 litColor = ambient + directLight;

	float3 r = reflect(-toEyeW, vout.NormalC);
	float4 CubemapFactor = gCubeMap.Sample(gSamAnisotropicWrap, r);
	float3 frensnelFactor = SchlickFresnel(R0, vout.NormalC, r);
	litColor.rgb += Shiness * frensnelFactor * CubemapFactor.rgb;

	litColor.a = DiffuseAlbedo.a;

	return litColor;
}
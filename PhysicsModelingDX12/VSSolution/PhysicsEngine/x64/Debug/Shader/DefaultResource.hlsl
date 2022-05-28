#include "LightingUtils.hlsl"

struct MaterialBuffer
{
	float4 DiffuseAlbedo;
	float3 R0;
	float Roughness;
	float4x4 MatTransform;

	uint SrvHeapIndex;
	uint SelectedColor;
	uint pad2;
	uint pad3;

	float4 Color;
};

TextureCube gCubeMap : register(t0);
Texture2D gTextures[2] : register(t1);
StructuredBuffer<MaterialBuffer> gMaterials : register(t0, space1);

SamplerState gSamPointWrap			: register(s0);
SamplerState gSamLinearWrap			: register(s1);
SamplerState gSamPointClamp			: register(s2);
SamplerState gSamLinearClamp		: register(s3);
SamplerState gSamAnisotropicWrap	: register(s4);
SamplerState gSamAnisotropicClamp	: register(s5);

cbuffer cbObjConstants : register(b0)
{
	float4x4 gWorld;
	float4x4 gTexTransform;

	uint gMatIndex;
	uint pad1;
	uint pad2;
	uint pad3;
}

cbuffer cbPassConstants : register(b1)
{
	float4x4 gProj;
	float4x4 gView;
	float4x4 gViewProj;

	float4 gAmbientLight;
	float3 gEyePos;
	float pad4;

	Light gLights[16];
}

struct VertexIn
{
	float3 PosL		: POSITION;
	float3 Tangent	: TANGENT;
	float3 NormalC	: NORMAL;
	float2 TexC		: TEXCOORD;
};

struct VertexOut
{
	float4 PosH		: SV_POSITION;
	float3 PosL		: POSITION0;
	float3 PosW		: POSITION1;
	float3 Tangent	: TANGENT;
	float3 NormalC	: NORMAL;
	float2 TexC		: TEXCOORD;
};
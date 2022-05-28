#ifndef NUM_DIR_LIGHT
#define NUM_DIR_LIGHT 3
#endif
#ifndef NUM_POINT_LIGHT
#define NUM_POINT_LIGHT 0
#endif
#ifndef NUM_SPOT_LIGHT
#define NUM_SPOT_LIGHT 0
#endif


struct Light {
	float3 Strength;
	float FallofStart;
	float3 Position;
	float FallofEnd;
	float3 Direction;
	float SpotPower;
};

struct Material {
	float4 DiffuseAlbedo;
	float3 R0;
	float Shiness;
};

float CalcAttenuation(float d, float fallofStart, float fallofEnd) {
	return saturate((fallofEnd - d) / (fallofEnd - fallofStart));
}

float3 SchlickFresnel(float3 R0, float3 normal, float3 lightVec) {
	float cosIncidentAngle = saturate(dot(normal, lightVec));

	float f0 = 1.0f - cosIncidentAngle;
	float3 reflectPercent = R0 + (1.0f - R0) * (f0 * f0 * f0 * f0 * f0);

	return reflectPercent;
}

float3 BlinnPhong(Material mat, float3 normal, float3 toEyeW, float3 lightVec, float3 lightStrength) {
	const float m = mat.Shiness * 256.0f;
	float3 halfVec = normalize(toEyeW + lightVec);

	float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, normal), 0.0f), m) / 8.0f;
	float3 frensnelFactor = SchlickFresnel(mat.R0, halfVec, lightVec);

	float3 specular = frensnelFactor * roughnessFactor;
	specular = specular / (specular + 1.0f);

	return (mat.DiffuseAlbedo.rgb + specular) * lightStrength;
}

float3 ComputeDirectionalLight(Light L, Material mat, float3 normal, float3 toEyeW) {
	float3 lightVec = -L.Direction;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 LightStrength = L.Strength * ndotl;

	return BlinnPhong(mat, normal, toEyeW, lightVec, LightStrength);
}

float3 ComputePointLight(Light L, Material mat, float3 normal, float3 toEyeW, float3 pos) {
	float3 lightVec = L.Position - pos;
	float d = length(lightVec);

	if (d > L.FallofEnd) {
		return 0.0f;
	}

	lightVec /= d;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 LightStrength = L.Strength * ndotl;

	LightStrength *= CalcAttenuation(d, L.FallofStart, L.FallofEnd);

	return BlinnPhong(mat, normal, toEyeW, lightVec, LightStrength);
}

float3 ComputeSpotLight(Light L, Material mat, float3 normal, float3 toEyeW, float3 pos) {
	float3 lightVec = L.Position - pos;
	float d = length(lightVec);

	if (d > L.FallofEnd) {
		return 0.0f;
	}

	lightVec /= d;

	float ndotl = max(dot(lightVec, normal), 0.0f);
	float3 LightStrength = L.Strength * ndotl;

	LightStrength *= CalcAttenuation(d, L.FallofStart, L.FallofEnd);

	LightStrength *= pow(max(dot(-lightVec, L.Direction), 0.0f), L.SpotPower);

	return BlinnPhong(mat, normal, toEyeW, lightVec, LightStrength);
}

float4 ComputeLighting(Light L[16], Material mat, float3 normal, float3 toEyeW, float3 pos, float3 shadowFactor) {
	float3 result = 0.0f;
	int i = 0;

#if (NUM_DIR_LIGHT > 0)
	for (i = 0; i < NUM_DIR_LIGHT; i++) {
		result += shadowFactor[i] * ComputeDirectionalLight(L[i], mat, normal, toEyeW);
	}
#endif

#if NUM_POINT_LIGHT > 0
	for (i = NUM_DIR_LIGHT; i < NUM_DIR_LIGHT + NUM_POINT_LIGHT; i++) {
		result += ComputePointLight(L[i], mat, normal, toEyeW, pos);
	}
#endif

#if NUM_SPOT_LIGHT > 0
	for (i = NUM_DIR_LIGHT + NUM_POINT_LIGHT; i < NUM_DIR_LIGHT + NUM_POINT_LIGHT + NUM_SPOT_LIGHT; i++) {
		result += ComputeSpotLight(L[i], mat, normal, toEyeW, pos);
	}
#endif

	return float4(result, 0.0f);
}
#include "Light.hlsli"

#ifdef USE_NORMAL_MAP
	Texture2D normalMapTexture : TEXTURE: register(t1);
#endif

#ifdef USE_TEXTURE
	Texture2D objTexture : TEXTURE: register(t0);
	SamplerState objSamplerState : SAMPLER: register(s0);
#else
	cbuffer ColorBuffer : register (b2)
	{
		float4 color;
	};
#endif

struct PS_INPUT
{
	float4 pos : SV_POSITION;
#ifdef USE_NORMAL_MAP
	float3 tang : TANGENT;
#endif
#ifdef APPLY_LIGHT
	float3 normal : NORMAL;
	float4 worldPos : POSITION;
#endif
#ifdef USE_TEXTURE
	float2 texCoord : TEXCOORD;
#endif
};

float4 main(PS_INPUT input) : SV_Target0
{
#ifdef USE_TEXTURE
	float4 pixelColor = objTexture.Sample(objSamplerState, input.texCoord);
#else
	float4 pixelColor = color;
#endif

	float4 finalColor = pixelColor;
	
#ifdef APPLY_LIGHT

#ifdef USE_NORMAL_MAP
	float3 binorm = normalize(cross(input.normal, input.tang));
	float3 localNorm = normalMapTexture.Sample(objSamplerState, input.texCoord).xyz * 2.0 - float3(1.0, 1.0, 1.0);
	float3 norm = localNorm.x * normalize(input.tang) + localNorm.y * binorm + localNorm.z * normalize(input.normal);
#else
	float3 norm = normalize(pixel.norm);
#endif

	float3 lightColor = applyLight(norm, input.worldPos.xyz);
	finalColor = float4(finalColor.xyz * lightColor, finalColor[3]);

#endif

	return finalColor;
}
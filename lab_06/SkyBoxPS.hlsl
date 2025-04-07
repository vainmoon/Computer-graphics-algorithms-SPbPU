#include "Light.hlsli"

TextureCube colorTexture : register(t0);
SamplerState colorSampler : register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float3 localPos : POSITION1;
};

float4 main(PS_INPUT pixel) : SV_Target0
{
#ifdef APPLY_LIGHT
	float3 ambientColor = ambient.xyz * ambient[3];
	return float4(colorTexture.Sample(colorSampler, pixel.localPos).xyz * ambientColor, 1.0);
#else
	return float4(colorTexture.Sample(colorSampler, pixel.localPos).xyz, 1.0);
#endif
}

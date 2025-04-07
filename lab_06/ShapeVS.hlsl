#include "Scene.hlsli"

cbuffer GeomBuffer : register (b1)
{
	float4x4 model;
};

struct VS_INPUT
{
	float3 pos : POSITION;
#ifdef USE_NORMAL_MAP
	float3 tang : TANGENT;
#endif
#ifdef APPLY_LIGHT
	float3 normal : NORMAL;
#endif
#ifdef USE_TEXTURE
	float2 texCoord : TEXCOORD;
#endif
};

struct VS_OUTPUT
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


VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 world = mul(float4(input.pos, 1.0f), model);
	output.pos = mul(world, vp);
#ifdef USE_NORMAL_MAP
	output.tang = input.tang;
#endif
#ifdef APPLY_LIGHT
	output.normal = input.normal;
	output.worldPos = world;
#endif
#ifdef USE_TEXTURE
	output.texCoord = input.texCoord;
#endif
	return output;
}
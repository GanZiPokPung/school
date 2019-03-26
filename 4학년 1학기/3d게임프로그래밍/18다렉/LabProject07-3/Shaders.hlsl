
cbuffer cbPlayerInfo : register(b0)
{
	matrix		gmtxPlayerWorld : packoffset(c0);
};

cbuffer cbCameraInfo : register(b1)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
	float3		gvCameraPosition : packoffset(c8);
};

cbuffer cbGameObjectInfo : register(b2)
{
	matrix		gmtxGameObject : packoffset(c0);
	uint		gnMaterial : packoffset(c4);
};

#include "Light.hlsl"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXTURECOORD;
};

struct VS_OUTPUT
{
	float4 positionH : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 uv : TEXTURECOORD;
};

//#define _WITH_VERTEX_LIGHTING

VS_OUTPUT VSLighting(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = mul(float4(input.position, 1.0f), gmtxGameObject).xyz;
	output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	float3 normalW = mul(input.normal, (float3x3)gmtxGameObject);
#ifdef _WITH_VERTEX_LIGHTING
	output.color = Lighting(output.positionW, normalize(normalW));
#else
	output.normalW = normalW;
#endif
	output.uv = input.uv;

	return(output);
}

float4 PSLighting(VS_OUTPUT input) : SV_TARGET
{
#ifdef _WITH_VERTEX_LIGHTING
	return(input.color);
#else
	float3 normalW = normalize(input.normalW);
	float4 color = Lighting(input.positionW, normalW);
	return(color);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
VS_OUTPUT VSPlayer(VS_INPUT input)
{
	VS_OUTPUT output;

	output.positionW = mul(float4(input.position, 1.0f), gmtxPlayerWorld).xyz;
	output.positionH = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	float3 normalW = mul(input.normal, (float3x3)gmtxPlayerWorld);
#ifdef _WITH_VERTEX_LIGHTING
	output.color = Lighting(output.positionW, normalize(normalW));
#else
	output.normalW = normalW;
#endif
	output.uv = input.uv;

	return(output);
}

static float3 gDirectionalLight = float3(0.0f, -1.0f, 1.0f);
static float4 gLightColor = float4(0.5f, 0.5f, 0.7f, 1.0f);

float4 PSPlayer(VS_OUTPUT input) : SV_TARGET
{
	float4 color = saturate(float4(0.2f + abs(input.uv.g), 0.2f + abs(input.uv.r), 0.2f, 1.0f) + gLightColor * abs(dot(normalize(input.normalW), normalize(-gDirectionalLight))));
	//	float4 color = saturate(float4(0.2f + abs(input.uv.r), 0.2f + abs(input.uv.g), 0.2f, 1.0f) + gLightColor * dot(normalize(input.normalW), normalize(-gDirectionalLight)));
	//	float4 color = saturate(float4(0.2f, 0.2f, 0.2f, 1.0f) + gLightColor * dot(normalize(input.normalW), normalize(-gDirectionalLight)));
	//	float4 color = saturate(float4(0.2f, 0.2f, 0.2f, 1.0f) + gLightColor * abs(dot(normalize(input.normalW), normalize(-gDirectionalLight))));
	return(color);
}



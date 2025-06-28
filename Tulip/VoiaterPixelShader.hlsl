#include "Common_Includes.hlsli"

float4 main(PixelShaderInput ps_inp) : SV_TARGET
{
    float edgeWidth = 0.05f;
    float minBary = min(min(ps_inp.bary.x, ps_inp.bary.y), ps_inp.bary.z);
    
    if (minBary < edgeWidth)
    {
        // Apply a simple edge detection effect
        float edgeFactor = 1.0f - saturate((edgeWidth - minBary) / edgeWidth);
        return float4(float3(0.4392, 1.0000, 0.8980), 1.0);
    }
 
    return float4(float3(0.0314, 0.0039, 0.0784), 1.0);
}
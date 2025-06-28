#include "Common_Includes.hlsli"

float4 main(PixelShaderInput ps_inp) : SV_TARGET
{
    return float4(ps_inp.color.rgb * ps_inp.diffuse, 1.0);
}

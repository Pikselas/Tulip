#include "Common_Includes.hlsli"

float4 main(PixelShaderInput ps_inp) : SV_TARGET
{
    float4 background_color = float4(0.1176, 0.8392, 0.4549, 1.0f);
    float4 grid_color = float4(0.49, 0.549, 0.824, 0.4f);
    float2 grid = frac(ps_inp.worldPos.xz * 1.0f); // Create a repeating grid pattern
    float lineWidth = 0.15f;
    float line_f = step(grid.x, lineWidth) + step(grid.y, lineWidth); // Detect grid lines

    // Blend between background and grid colors based on the line value
    float4 interpolated_col = lerp(background_color, grid_color, line_f);
    return float4(interpolated_col.rgb * ps_inp.diffuse, interpolated_col.a);
}
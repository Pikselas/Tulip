float4 main(float4 col : COLOR, float4 pos : TEXCOORD0) : SV_TARGET
{
    float4 background_color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 grid_color = float4(0.471, 0.549, 0.824, 1.0f);
    float2 grid = frac(pos.xz * 1.0f); // Create a repeating grid pattern
    float lineWidth = 0.05f;
    float line_f = step(grid.x, lineWidth) + step(grid.y, lineWidth); // Detect grid lines

    // Blend between background and grid colors based on the line value
    return lerp(background_color, grid_color, line_f);
}
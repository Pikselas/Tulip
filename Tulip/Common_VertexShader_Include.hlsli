#include "Common_Includes.hlsli"

cbuffer cv : register(b0)
{
    matrix transformation_view;
    matrix projection_view;
};

VertexShaderOut ProcessVertex(VertexShaderInput vs_inp)
{
    VertexShaderOut Out;
    Out.pos = mul(float4(vs_inp.pos, 1.0f), projection_view);
    Out.worldPos = vs_inp.pos;
    Out.transformedPos = mul(float4(vs_inp.pos, 1.0), transformation_view);
    Out.color = vs_inp.col;
    return Out;
}
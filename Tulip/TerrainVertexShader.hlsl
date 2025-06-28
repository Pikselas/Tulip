#include "Common_VertexShader_Include.hlsli"

VertexShaderOut main(VertexShaderInput vs_inp)
{
    return ProcessVertex(vs_inp);
}
struct VertexShaderOut
{
    float4 color : COLOR;
    float3 worldPos : TEXCOORD0;
    float3 transformedPos : TEXCOORD1;
    float4 pos : SV_POSITION;
};

struct GeometryShaderOutput
{
    float4 color : COLOR;
    float3 worldPos : TEXCOORD0;
    float3 transformedPos : TEXCOORD1;
    float diffuse : DIFFUSE;
    float3 bary : BARY;
    float4 pos : SV_POSITION;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float4 col : COLOR;
};

struct PixelShaderInput
{
    float4 color : COLOR;
    float3 worldPos : TEXCOORD0;
    float3 transformedPos : TEXCOORD1;
    float diffuse : DIFFUSE;
    float3 bary : BARY;
};
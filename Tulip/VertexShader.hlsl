struct VertexShaderOut // this is the ouput type
{
	float4 color : COLOR; // this is the user defined semantic for color which will be passed to pixel shader
	float4 pos : SV_POSITION; // this is the SYSTEM_VALUE SEMANTIC this is fixed (and defined by the API) 
};

cbuffer cv
{
	matrix trans;
};

VertexShaderOut main(float3 pos : POSITION, float4 col : COLOR)
{
	VertexShaderOut Out;
	Out.pos = mul(float4(pos, 1.0f) , trans);
    Out.color = col;
	return Out;
}
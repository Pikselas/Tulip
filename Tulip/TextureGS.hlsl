#include "Common_Includes.hlsli"

[maxvertexcount(3)]
void main(
	triangle VertexShaderOut input[3],
	inout TriangleStream<GeometryShaderOutput> output
)
{	
    float3 normal = cross(input[1].transformedPos - input[0].transformedPos, input[2].transformedPos - input[0].transformedPos);
	normal = normalize(normal);
	
    float3 lightDir = normalize(float3(0.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDir), 0.01f);
	
    float3 barycentrics[3] =
    {
		float3(1, 0, 0),
		float3(0, 1, 0),
		float3(0, 0, 1)
    };
	
    //diffuse = 0.5f + 0.5f * dot(normal, lightDir); // Adjusted diffuse calculation for better lighting effect
	for (uint i = 0; i < 3; i++)
	{
		GeometryShaderOutput element;
		element.pos = input[i].pos;
		element.diffuse = diffuse;
		element.color = input[i].color;
        element.worldPos = input[i].worldPos;
        element.transformedPos = input[i].transformedPos;
        element.bary = barycentrics[i];
		output.Append(element);
	}
}
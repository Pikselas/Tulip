struct GSOutput
{
    float3 color : COLOR;
	float diffuse : DIFFUSE;
	float4 pos : SV_POSITION;
};

cbuffer lifgtBuff
{
    float4 lightDir;
};

[maxvertexcount(3)]
void main(
	triangle float3 color[3] : COLOR,
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	
    float3 normal = cross(input[1] - input[0], input[2] - input[0]);
	normal = normalize(normal);
	
	//float3 lightDir = normalize(float3(1.0f, 1.0f, 0.0f));
    float diffuse = max(dot(normal, lightDir), 0.0f);
	
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.pos = input[i];
		element.diffuse = 1.0f;
		element.color = color[i];
		output.Append(element);
	}
}
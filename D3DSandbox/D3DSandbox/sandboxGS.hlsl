struct GSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};
struct GSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};
/*
float4 colors[6] = {
float4(1.0f, 0.0f, 0.0f, 1.0f),
float4(0.0f, 1.0f, 0.0f, 1.0f),
float4(0.0f, 0.0f, 1.0f, 1.0f),
float4(1.0f, 1.0f, 1.0f, 1.0f),
float4(0.0f, 0.0f, 0.0f, 1.0f),
float4(1.0f, 1.0f, 0.0f, 1.0f),
	};
*/

[maxvertexcount(6)]
void main(point GSInput In[1], uint PrimitiveID : SV_PrimitiveID, inout TriangleStream<GSOutput> stream)
{
	GSOutput v0, v1, v2, v3;
	float size = 0.1;
	//float4 color = colors[PrimitiveID]; 
    float4 color = In[0].Color;
	v0.Position = In[0].Position + float4(-size, size, 0, 0);
	v0.Color = color;
	v1.Position = In[0].Position + float4(size, size, 0, 0);
	v1.Color = color;
	v2.Position = In[0].Position + float4(-size, -size, 0, 0);
	v2.Color = color;
	v3.Position = In[0].Position + float4(size, -size, 0, 0);
	v3.Color = color;

	stream.Append(v0);
	stream.Append(v1);
	stream.Append(v2);
	stream.Append(v3);
}
/*
struct GSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		element.Position = input[i];
		element.Color = float4(1.0f,1.0f,1.0f,1.0f);
		output.Append(element);
	}
}
*/

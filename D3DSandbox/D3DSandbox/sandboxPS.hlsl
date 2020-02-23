struct GSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(GSOutput In) : SV_TARGET
{
	return In.Color;
}
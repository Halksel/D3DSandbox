struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Color : COLOR;
};

struct VS_OUT
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
};

VS_OUT main( VS_INPUT input )
{
    VS_OUT output;
    output.Position = input.Position;
    output.Color = input.Color;
    return output;
}
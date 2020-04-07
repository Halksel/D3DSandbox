struct PS_IN
{
        float4 Position : SV_POSITION;
        float4 Color : COLOR0;
};
 
 
float4 main( PS_IN input ) : SV_Target
{
        return input.Color;
}
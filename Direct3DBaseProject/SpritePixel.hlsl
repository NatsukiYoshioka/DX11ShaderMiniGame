Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

float4 main(float4 color : COLOR0,
    float2 texCoord : TEXCOORD0):SV_Target0
{
    float4 Color;
    
    Color = Texture.Sample(TextureSampler, texCoord) * color;
    
    return Color;
}
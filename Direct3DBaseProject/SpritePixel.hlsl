Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

struct PSOut
{
    float4 BackBuffer : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Color : SV_Target2;
};

PSOut main(float4 color : COLOR0,
    float2 texCoord : TEXCOORD0)
{
    PSOut Out;
    
    Out.BackBuffer = Texture.Sample(TextureSampler, texCoord) * color;
    
    Out.Normal.xy = 0.f;
    Out.Normal.zw = color.a;
    
    Out.Color = Out.BackBuffer;
    
    return Out;
}
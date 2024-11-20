#include"Header.hlsli"

Texture2D<float4> texture0 : register(t0);
Texture2D<float4> texture1 : register(t1);
Texture2D<float4> texture2 : register(t2);
Texture2D<float4> texture3 : register(t3);
SamplerState Sampler : register(s0);

float4 main(PPPS pout) : SV_Target0
{
    float4 combineColor = texture0.Sample(Sampler, pout.TexCoord);
    combineColor += texture1.Sample(Sampler, pout.TexCoord);
    combineColor += texture2.Sample(Sampler, pout.TexCoord);
    combineColor += texture3.Sample(Sampler, pout.TexCoord);
    
    combineColor /= 4.f;
    combineColor.a = 1.f;
    
    return combineColor;
}
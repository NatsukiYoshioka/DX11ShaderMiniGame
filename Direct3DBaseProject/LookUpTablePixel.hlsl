#include"Header.hlsli"

Texture2D<float4> sceneTexture : register(t0);
Texture2D<float4> lutTexture : register(t1);
SamplerState Sampler : register(s0);

cbuffer LUTConstants : register(b4)
{
    float lutPower;
}

float4 main(PPPS pout):SV_Target0
{   
    float4 color = sceneTexture.Sample(Sampler, pout.TexCoord);
                
    //2DLUTの適用（G成分をV方向に、R,B成分をU方向に割り当て）
    float2 lutCoord;
    lutCoord.x = floor(color.b * 15.f) / 15.f * 240.f;
    lutCoord.x = (floor(color.r * 15.0) / 15.0 * 15.0) + lutCoord.x;
    lutCoord.x /= 255.f;
    lutCoord.y = (floor(color.g * 15.0) / 15.0);
    
    float3 left = lutTexture.SampleLevel(Sampler, lutCoord, 1).rgb;
        
    lutCoord.x = ceil(color.b * 15.f) / 15.f * 240.f;
    lutCoord.x = (ceil(color.r * 15.0) / 15.0 * 15.0) + lutCoord.x;
    lutCoord.x /= 255.f;
    lutCoord.y = (ceil(color.g * 15.0) / 15.0);
    
    float3 right = lutTexture.SampleLevel(Sampler, lutCoord, 1).rgb;
    
    float4 lutColor;
    lutColor.r = lerp(left.r, right.r, frac(color.r * 15.f));
    lutColor.g = lerp(left.g, right.g, frac(color.g * 15.f));
    lutColor.b = lerp(left.b, right.b, frac(color.b * 15.f));
    
    color.rgb = lerp(color.rgb, lutColor.rgb, lutPower);

    return color;
}
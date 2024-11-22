#include"Header.hlsli"

Texture2D<float4> sceneTexture : register(t0);
Texture2D<float4> lutTexture : register(t1);
SamplerState Sampler : register(s0);

struct PSOut
{
    float4 BackBuffer : SV_Target0;
    float4 Brightness : SV_Target1;
};

cbuffer LUTConstants : register(b4)
{
    float lutPower;
}

PSOut main(PPPS pout):SV_Target0
{   
    PSOut Out;
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

    Out.BackBuffer = color;
    
    //輝度出力
    float t = dot(color.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    if (t - 1.f < 0.f)
    {
        color = float4(0.f, 0.f, 0.f, 0.f);
    }
    Out.Brightness = color;
    return Out;
}
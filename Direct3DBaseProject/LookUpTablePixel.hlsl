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
    
    //2DLUTの適用（G成分をU方向に、R成分をV方向に、B成分を行に割り当て）
    float2 lutCoord;
    lutCoord.x = 64 * 1 + color.r * 63.f / 512.f;               //R成分をU方向の座標に
    lutCoord.y = color.g * 7.f / 8.f * color.b * 63.f / 64.f;   //B成分とG成分をV方向の座標に
    lutCoord /= float2(512.0, 512.0);                           //正規化

    float4 lutColor = lutTexture.Sample(Sampler, lutCoord);
    
    color.rgb = lerp(color.rgb, lutColor.rgb, lutPower);
    return color;
}
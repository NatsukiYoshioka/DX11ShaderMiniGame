#include"Header.hlsli"

cbuffer LightColor : register(b4)
{
    float3 color;
};

struct PSOut
{
    float4 BackBuffer : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Color : SV_Target2;
    float4 Brightness : SV_Target3;
};

PSOut main(PSOutput pout)
{
    PSOut Out;
    
    float4 finalColor = float4(color.xyz, 0.f);
    Out.BackBuffer = finalColor;
    
    //法線ベクトル計算要素の出力
    Out.Normal.xy = normalize(mul(float4(pout.Normal, 0), mul(World, View)).xyz).xy;
    //深度値を出力
    Out.Normal.zw = pout.Position.zw;
        
    //色素出力
    Out.Color = Out.BackBuffer;
    
    //輝度出力
    float t = dot(finalColor.xyz, float3(0.2125f, 0.7154f, 0.0721f));
    if (t - 1.f < 0.f)
    {
        finalColor = float4(0.f, 0.f, 0.f, 0.f);
    }
    Out.Brightness = finalColor;
    
    return Out;
}
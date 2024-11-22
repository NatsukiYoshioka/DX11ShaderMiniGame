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
        
    return Out;
}
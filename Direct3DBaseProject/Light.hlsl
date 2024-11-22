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
    
    //�@���x�N�g���v�Z�v�f�̏o��
    Out.Normal.xy = normalize(mul(float4(pout.Normal, 0), mul(World, View)).xyz).xy;
    //�[�x�l���o��
    Out.Normal.zw = pout.Position.zw;
        
    //�F�f�o��
    Out.Color = Out.BackBuffer;
        
    return Out;
}